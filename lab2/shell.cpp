#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <limits>
#include <memory>
#include <pwd.h>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// 「命令」的抽象类
class Command
{
public:
    // execute 方法用于执行命令，在此是纯虚函数，所有派生类都要提供实现
    // wait 是该命令等待退出的方法
    // 析构函数，「= default」表示该析构函数由编译器默认给出

    virtual void execute() = 0;
    virtual void wait(){};
    virtual ~Command() = default;
};

// 「命令」的派生类：外部命令
class ExternalCommand : public Command
{
public:
    // explicit 关键字用来修饰类构造函数，它的作用是禁止隐式转换，避免忽略传参导致错误
    //
    // 「:」提供了一个初始化列表，其在构造函数创建对象时直接对成员变量进行初始化，避免了先定义再赋值的过程，提高了代码的效率和可读性
    // 比如，这里 func(T args) : args(std::move(args)) 中，第一/三个 args 是参数名称，第二个 args 是此类的成员对象名
    // 表示将参数 args 占有的资源转移给成员变量 args
    //
    // override 记号用于显式地指示一个派生类的虚函数覆盖了基类的虚函数，它可以帮助避免在虚函数重写过程中出现一些潜在的错误
    // 比如，如果对一个不是虚函数的函数名使用 override，编译期会报错
    //
    // 由于我们要使用的 syscall 是比较 low-level 的接口，我们需要将 std::string 转换成 char// 才可以被 execvp 函数接收
    // std::string 类提供 c_str() 接口提供 C 风格的字符串，但是返回值为 const char*
    // C++ 中已不推荐使用强制类型转换，取而代之的是 static_cast<T>, dynamic_cast<T> 等
    // 这里的 const_cast<char*> 将一个指向常量对象的指针转换为指向非常量对象的指针
    //
    // 你可以思考：为什么这里的 argv 没有 delete 掉以释放内存？这和 execvp 的运行逻辑密切相关。
    //
    // nullptr 标记参数列表字符串的结束
    //
    // 将参数交给 execvp 函数去执行，如果成功，将执行新的程序；如果失败，将执行 cout 和 exit 退出子进程

    explicit ExternalCommand(std::vector<std::string> args) : args(std::move(args)) {}

    void execute() override
    {
        char **argv = new char *[args.size() + 1];
        for (auto i = 0U; i < args.size(); i++)
        {
            argv[i] = const_cast<char *>(args[i].c_str());
        }
        argv[args.size()] = nullptr;
        execvp(argv[0], argv);
        std::cout << "command not found: " << argv[0] << std::endl;
        exit(-1);
    }

private:
    std::vector<std::string> args;
};

// 保存后台进程 PID 列表
std::vector<pid_t> background_pids;

// 「命令」的派生类：内建命令
class BuiltInCommand : public Command
{
public:
    // 内建命令通过 is_builtin 函数来识别
    //
    // std::find 函数定义于 <algorithm>，可以在一个支持迭代的容器中查找值相等的元素，并返回其迭代器
    // 如果返回 容器.end()，表示 find 没有找到对应的元素

    explicit BuiltInCommand(std::vector<std::string> args) : args(std::move(args)) {}
    static bool is_builtin(const std::string &command)
    {
        const std::vector<std::string> builtin_cmd{"cd", "exit", "pwd", "wait"};
        return std::find(builtin_cmd.begin(), builtin_cmd.end(), command) != builtin_cmd.end();
    }
    void execute() override
    {
        if (args[0] == "cd")
        {
            chdir(args.size() == 1 ? getenv("HOME") : args[1].c_str());
        }
        else if (args[0] == "exit")
        {
            exit(0);
        }
        else if (args[0] == "pwd")
        {
            std::cout << getcwd(0, 0) << std::endl;
        }
        else if (args[0] == "wait")
        {
            for (auto pid : background_pids)
            {
                waitpid(pid, nullptr, 0);
                std::cout << pid << " exited" << std::endl;
            }
            background_pids.clear();
        }
    }

private:
    std::vector<std::string> args;
};

// 「命令」的派生类：重定向命令
class RedirectCommand : public Command
{
public:
    // 一条重定向命令由「去掉重定向之后的命令」、「重定向输入文件」、「重定向输出文件」组成
    // 考虑到 > 和 >> 的区别，还需要指定是否追加
    // 这可以通过调用 open 时传的参数来做到，O_TRUNC 表示打开文件时清除原来的内容，O_APPEND 则顾名思义
    //
    // 0644 是文件权限设置，它表示拥有者 (用户) 有读写权限、拥有者组和其他用户有读权限
    //
    // 以输出重定向为例，首先我们打开需要重定向到的文件，获得其文件描述符 fd
    // 然后使用 dup2，把标准输出代表的文件描述符指向的文件重定向至 fd
    // 此时标准输出和 fd 都指向刚刚打开的文件，为了防止出现问题，我们关闭 fd 这个文件描述符（不影响打开的标准输出）
    //
    // 在执行了重定向之后，就可以把剩下的命令部分「递归」地执行，只需要调用其 execute 方法即可
    // 注意，这里的 command 是指向抽象类的指针，所以并不需要管它是 buildin or external or others，这就是类型继承的好处

    explicit RedirectCommand(
        std::unique_ptr<Command> cmd,
        std::string out_file,
        std::string in_file, bool append) : cmd(std::move(cmd)),
                                            out_file(std::move(out_file)),
                                            in_file(std::move(in_file)),
                                            append(append) {}
    void execute() override
    {
        if (!out_file.empty())
        {
            auto fd = open(out_file.c_str(), O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if (!in_file.empty())
        {
            auto fd = open(in_file.c_str(), O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        cmd->execute();
    }

private:
    std::unique_ptr<Command> cmd;
    std::string out_file;
    std::string in_file;
    bool append;
};

// 「命令」的派生类：管道命令
class PipeCommand : public Command
{
public:
    // 因为我们要求管道的各部分是并发执行，不能阻塞（如 sleep 3 | echo osh | cat 并不会在三秒之后才输出 osh）
    // 所以管道的各部分都必须是单独在一个进程中执行的
    //
    // 我们将一个管道命令从左往右处理，左边 cmd1 是一个不含管道的语句，右边 cmd2 是余下的部分，可能含有管道
    // 我们 fork 一个子进程，在子进程中将标准输出重定向到管道的写端，接下来关闭管道读端（用不到）、关闭管道写端（已重定向）
    // 接下来，在子进程中执行 cmd1 即可
    //
    // 在当前进程中，继续将剩下的 cmd2 作为一个新的（可能含有管道的）命令来执行
    // 在执行之前，因为 cmd2 是后部的命令，我们需要将其标准输入重定向到管道的读端
    //
    // 你可以试试在纸上画出这一过程，标出进程、子进程关系和标准输入、标准输出关系，这可能会让你获得更好的理解
    //
    // 虽然所有的子命令都是并发的，这条命令仍然需要等到所有子命令都结束后才可退出
    // 这也是 wait 的实现方向，因为管道命令的执行 fork 了若干子进程

    explicit PipeCommand(std::unique_ptr<Command> cmd1,
                         std::unique_ptr<Command> cmd2)
        : cmd1(std::move(cmd1)),
          cmd2(std::move(cmd2)) {}

    void execute() override
    {
        int pipefd[2];
        pipe(pipefd);
        pid = fork();
        if (pid == 0)
        {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            cmd1->execute();
        }
        else
        {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            cmd2->execute();
        }
    }
    void wait() override
    {
        cmd2->wait();
        waitpid(pid, nullptr, 0);
    }

private:
    pid_t pid;
    std::unique_ptr<Command> cmd1;
    std::unique_ptr<Command> cmd2;
};

// 「命令」的派生类：shell 命令（处理前后台）
class ShellCommand : Command
{
public:
    // shell command 是我们的 shell 的最上层的命令类型，这一层额外做的判断是其是否是后台命令
    // 由于我们解析 shell command 时，因为只需要考虑命令行末尾有一个 &，不需要考虑嵌套，所以我们使用一个 background_pid 作为记录
    //
    // 我们一开始屏蔽了 SIGTTOU 信号，子进程执行时需要重新恢复默认信号处理函数
    // 注：exec 会自动将设置为非 SIG_DFL 且非 SIG_IGN 的信号处理函数重置为 SIG_DFL
    // 因为信号处理函数是在原地址空间的，exec 后是无法调用自定义的信号处理函数的

    explicit ShellCommand(std::unique_ptr<Command> cmd, bool is_bg) : cmd(std::move(cmd)),
                                                                      is_bg(is_bg) {}
    void execute() override
    {
        // 首先需要看这个命令是否是内建命令，内建命令不会 fork 新的进程，也就无所谓后台运行
        auto is_built_in = dynamic_cast<BuiltInCommand *>(cmd.get()) != nullptr;

        if (is_built_in)
            return cmd->execute();

        // 执行到这里，说明是外部命令，需要 fork 一个子进程进行处理
        // 注意这里的 pid 是成员变量
        // 父进程（shell）的操作是，如果需要这条命令在后台执行，则直接在表中记录一下即可返回
        // 如果需要这条命令在前台执行，则必须等待命令结束
        pid = fork();
        if (pid != 0)
        {
            return is_bg ? background_pids.push_back(pid) : wait();
        }

        // 执行到这里，是在 fork 的子进程中
        setpgid(0, 0);              // 把当前进程的 pgid 设置为当前进程的 pid
        if (!is_bg)                 // 如果是前台进程
            tcsetpgrp(0, getpid()); // 将终端的前台 pgid 设置为当前进程的 pid，确保前台进程可以接收终端的 IO
        signal(SIGTTOU, SIG_DFL);
        cmd->execute();
    }
    void wait() override
    {
        cmd->wait();
        waitpid(pid, nullptr, 0);
        tcsetpgrp(0, getpid());
    }

private:
    std::unique_ptr<Command> cmd;
    bool is_bg;
    pid_t pid;
};

// 命令解析器
class CommandParser
{
public:
    // istringstream 是一个输入的字符串流，它的作用类似于 C 中 sscanf, sprintf，但功能更为强大
    // 如 iss >> word，表示从字符串流中读取一个 token 到 word 中

    static std::vector<std::string> split(const std::string &str)
    {
        std::vector<std::string> words;
        std::istringstream iss(str);
        std::string word;
        while (iss >> word)
        {
            // 做环境变量和根目录的文字替换
            if (word[0] == '$')
            {
                auto *env_var = getenv(word.substr(1).c_str());
                word = env_var != nullptr ? env_var : "";
            }
            else if (word[0] == '~')
            {
                auto username = word.substr(1);
                if (username.empty())
                {
                    word = getenv("HOME");
                }
                else
                {
                    struct passwd *pw = getpwnam(username.c_str());
                    word = pw != nullptr ? pw->pw_dir : "";
                }
            }
            words.push_back(word);
        }
        return words;
    }
    // 内建命令还是外部命令
    static std::unique_ptr<Command> parseBase(std::vector<std::string> words)
    {
        if (words.empty())
            return nullptr;
        if (BuiltInCommand::is_builtin(words[0]))
            return std::make_unique<BuiltInCommand>(words);
        return std::make_unique<ExternalCommand>(words);
    }
    // 重定向命令
    static std::unique_ptr<Command> parseRedir(std::vector<std::string> words)
    {
        std::string out_file;
        std::string in_file;
        bool append;

        // 下面没有处理错误，比如 > 在命令最后会导致越界
        auto cmd_idx = std::numeric_limits<unsigned>::max();
        for (auto i = 0U; i < words.size(); ++i)
        {
            if (words[i] == ">")
            {
                cmd_idx = std::min(cmd_idx, i);
                out_file = std::move(words.at(i + 1));
                append = false;
            }
            else if (words[i] == ">>")
            {
                cmd_idx = std::min(cmd_idx, i);
                out_file = std::move(words.at(i + 1));
                append = true;
            }
            else if (words[i] == "<")
            {
                cmd_idx = std::min(cmd_idx, i);
                in_file = std::move(words.at(i + 1));
            }
        }

        if (cmd_idx != std::numeric_limits<unsigned>::max())
        {
            // 我们认为重定向符号仅出现在命令结尾
            // 对迭代器 + 一个整数 n，就是把迭代器向后移动 n 次
            auto baseCommand = parseBase(
                std::vector<std::string>{words.begin(), words.begin() + cmd_idx});
            return std::make_unique<RedirectCommand>(std::move(baseCommand),
                                                     std::move(out_file),
                                                     std::move(in_file), append);
        }
        return parseBase(std::move(words));
    }
    // 管道命令
    static std::unique_ptr<Command> parsePipe(std::vector<std::string> words)
    {
        // 从左往右（find 默认）找第一个管道符号，并获得迭代器
        auto it = std::find(words.begin(), words.end(), "|");
        if (it == words.end())
        {
            return parseRedir(std::move(words));
        }

        // 计算管道符号左部和右部的 words
        auto index = std::distance(words.begin(), it);
        auto left_words =
            std::vector<std::string>{words.begin(), words.begin() + index};
        auto right_words =
            std::vector<std::string>{words.begin() + index + 1, words.end()};

        auto left_command = parsePipe(std::move(left_words));
        auto right_command = parsePipe(std::move(right_words));
        if (!left_command || !right_command)
            return nullptr;

        return std::make_unique<PipeCommand>(std::move(left_command), std::move(right_command));
    }
    // 终端命令
    static std::unique_ptr<ShellCommand> parse(const std::string &line)
    {
        auto words = split(line);
        if (words.empty())
            return nullptr;
        auto bg = words.back() == "&";
        if (bg)
            words.pop_back(); // 记录信号，将 & 从 words 中删除
        return std::make_unique<ShellCommand>(parsePipe(std::move(words)), bg);
    }
};

// 处理信号
void init_signal()
{
    // signal 函数是将「信号」的处理方法设置成第二个参数 handler（可以理解成回调函数）
    // handler 需要是函数指针，而不带捕获的 lambda 表达式可以隐式类型转换成函数指针
    // 这里忽略 SIGTTOU 信号；自定义 SIGINT 以实现丢弃命令；自定义 SIGCHLD 以回收后台进程
    signal(SIGTTOU, SIG_IGN);
    signal(SIGINT, [](int)
           { std::cout << "\n$ " << std::flush; });
    signal(SIGCHLD, [](int)
           {
        pid_t pid;
        while ((pid = waitpid(-1, nullptr, WNOHANG | WUNTRACED)) > 0) {
            auto it = std::find(background_pids.begin(), background_pids.end(), pid);
            if (it != background_pids.end()) {
                background_pids.erase(it);
                std::cout << '\n' << pid << " exited\n$ " << std::flush;
            }
        } });
}

std::string read_line()
{
    std::cout << "$ ";
    std::string line;
    if (!std::getline(std::cin, line))
    {
        std::cout << std::endl;
        exit(0);
    }
    return line;
}

int main()
{
    init_signal();
    while (true)
    {
        auto line = read_line();
        if (line.empty())
            continue;
        auto cmd = CommandParser::parse(line);
        if (cmd == nullptr)
            continue;
        cmd->execute();
    }
    return 0;
}
