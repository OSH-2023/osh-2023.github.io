> <p style="font-family:楷体">　　夫既心识其所以然而不能然者，内外不一，心手不相应，不学之过也。故凡有见于中而操之不熟者，平居自视了然，而临事忽焉丧之，岂独竹乎？</p>

**注意：非正式版本！！！**

## 本学期的实验安排

本学期的实验设计和之前几届的大致相同，但会结合前几届的经验和反馈进行调整和优化。实验的目的是让大家加深对操作系统诸概念和技术的理解。请大家务必认真完成实验，这是大家掌握操作系统重难点的绝佳机会。我们会在每个实验文档中尽可能给到精炼而有用的引导，帮助大家顺利完成实验。

- 每个实验页面开通了评论区，欢迎大家在评论区留下你对实验的思考或见解；
- 每个实验的评分标准（包含 bonus 在内）以各个实验文档内的说明为准；
- 每个实验的提交截止时间以课程主页首页为准，如无特殊情况，迟交将会影响当次实验的最终成绩；
- 本学期各实验要求使用 C/C++/Rust 语言编写，不允许使用 Go 语言，具体要求将在后文详述；
- 本学期各实验的考察内容和技能要求如下，实验所考察的知识和课程进度同步，所需的编程技能可能需要靠自己掌握。

|  实验  |              主题              |                 考察知识                  |              所需技能              |
| :----: | :----------------------------: | :---------------------------------------: | :--------------------------------: |
| 实验零 |       OS 课程工具链教学        |     Git、markdown、Rust/C++/C、Linux      |           阅读文档的能力           |
| 实验一 |        裁剪 Linux 内核         |         Linux 启动过程、内核模块          |        make、makefile 基础         |
| 实验二 |        编写 Shell 程序         | fork 等系统调用、I/O 重定向、多线程、中断 | 多线程编程、使用 syscall、中断处理 |
| 实验三 |        操作系统的高并发        |            进程调度、多路复用             |      多线程、I/O 相关 syscall      |
| 实验四 | 分布式计算系统或存储系统的部署 |   系统集群的配置、性能度量、安全性监控    |     虚拟技术、设计性能分析方案     |

现在，欢迎大家来到 OSH 2023 的实验零。这个实验的目的是教会大家使用 OS 课程的工具链，主要包含如下技术：

- 使用 Linux 作为实验的主要环境
- 使用 Git 维护自己的代码版本
- 使用 markdown 来编写美观、规范的文档
- 使用 C/C++/Rust 编写程序的要求、规范和注意事项

在每个技术讲解文档之后，可能有若干个小任务需要完成，建议在阅读「Git 版本维护」这一节后进行提交。

<p style="color:red">注意：除了标明「必做」的部分外，实验零的完成情况不计入最终实验得分，但是助教会随机抽查，针对完成的较差的同学给予提醒和帮助。你可以通过在本页面搜索「必做」来查询所有的此类事项。</p>

## Linux

OSH 几乎所有实验都需要在 Linux 平台上完成、实现，所以本节期望你有一个可用的 Linux 工作环境和基本的 Linux 使用能力。如果你是一个熟练的 Linux 用户，你可以跳过本节。

### 获得 Linux 环境

- 在 [USTC Vlab](https://vlab.ustc.edu.cn/vm/) 上得到一个 Linux 的容器环境（并非虚拟机，*可能*有部分实验无法在其上面完成），可以通过图形界面或 VSCode 远程 SSH 等多种方式连接；
- 使用虚拟机，Windows 用户推荐 WSL2 或 VMware，macOS 用户可以参考 [在 macOS 中使用虚拟机（By Taoky）](https://blog.taoky.moe/2019-02-23/installing-os-on-vm.html)；
- 本地系统换成 Linux 或双系统。**实机安装可能会存在一定的风险**，如愿意尝试请根据自己想要安装的发行版搜索相应教程。

我们会保证 Ubuntu 虚拟机/实体机不出现兼容性问题，并尽量保证使用 Vlab 可以完成实验。

### Linux 入门

???+ tip "终端/shell/命令行/软件源/换源是什么意思？"

    如果你还不知道这些词语的意思，请自行上网搜索。这会让你对 Linux 建立基本的了解。
    
    安装好 Linux 后推荐先更换软件源。如对于 Ubuntu 系统，参考 [USTC Mirror Ubuntu 源使用帮助](https://mirrors.ustc.edu.cn/help/ubuntu.html)。

推荐以下资源：

- [LUG Linux 101 教程](https://101.lug.ustc.edu.cn/)（**关注第三章的文件操作、第六章的重定向和第七章 Linux 上的编程**）；
- [Debian 教程](https://www.debian.org/doc/manuals/debian-reference/ch01.zh-cn.html)；
- [Arch Linux 文章索引](<https://wiki.archlinux.org/index.php/General_recommendations_(%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87)>)。

你至少需要理解命令行的目录导航、文件操作、重定向以及 Makefile。

??? question "为什么要使用 Linux"

    参考 [南京大学 ICS PA](https://nju-projectn.github.io/ics-pa-gitbook/ics2022/0.5.html)（License: CC BY-NC-SA 3.0 CN）。
    
    我们先来看两个例子。
    
    **如何比较两个文件是否完全相同？**这个例子看上去非常简单，在 Linux 下使用 `diff` 命令就可以实现。如果文件很大，那不妨用 `md5sum` 来计算并比较它们的 MD5。对一个 Linux 用户来说，键入这些命令只需要花费大约 3 秒的时间。但在 Windows 下，这件事要做起来就不那么容易了。也许你下载了一个 MD5 计算工具，但你需要点击多少次鼠标才能完成比较呢? 也许你觉得一次好像也省不了多少时间，<span style="color: red">然而真相是，你的开发效率就是这样一点点降低的。</span>
    
    **如何列出一个C语言项目中所有被包含过的头文件？**这个例子比刚才的稍微复杂一些，但在 Windows 下你几乎无法高效地做到它。在 Linux 中，我们只需要通过一行命令就可以做到了：
    
    ```shell
    find . -name "*.[ch]" | xargs grep "#include" | sort | uniq
    ```
    
    通过查阅 `man`，你应该不难理解上述命令是如何实现所需功能的。这个例子再次体现了 Unix 哲学：
    
    - 每个工具只做一件事情，但做到极致；
    - 工具采用文件方式进行输入输出，从而易于使用；
    - 通过工具之间的组合来解决复杂问题。
    
    Unix 哲学的最后一点最能体现 Linux 和 Windows 的区别：<span style="color:blue">编程创造</span>。如果把工具比作代码中的函数，工具之间的组合就是一种编程。而 Windows 的工具之间几乎无法组合，因为面向普通用户的 Windows 需要强调易用性。
    
    所以，你应该使用 Linux 的原因非常简单：<span style="color:red">作为一个码农，Windows 一直在阻碍你思想，能力和效率的提升。</span>
    
    上面这段文字是从一个程序员的角度来说的。对于操作系统课程而言，也有必须熟悉与使用 Linux 的理由：作为目前最流行的开源操作系统，Linux 在足够易用，实用的同时也能帮助你将操作系统课程的理论与实践相结合。你不仅可以使用系统调用或系统调用封装而来的函数库，也可以通过源代码了解到每一个系统调用和数据结构的具体实现。

???+ info "The Missing Semester of Your CS Education"

    [The Missing Semester of Your CS Education](https://missing.csail.mit.edu/) 是 MIT 的一门 Linux 工具系列教程，官网上有简体中文的翻译版。如果你想 **进阶** 自己的 Linux 使用技巧，不妨看看这套课程。


## Git 版本维护

Git 是一个免费、开源的分布式版本控制系统，它为团队协作和软件开发提供了许多重要的优势和必要性，包括：

- **版本控制**：Git 可以跟踪代码的变化，允许开发人员随时访问以前的版本。这意味着您可以轻松地撤消错误更改，恢复文件状态，比较代码差异等等。在团队协作中，Git 可以允许团队成员协同工作，对同一份代码进行修改和版本管控，保证了代码的稳定性与质量。
- **分支管理**：Git 的分支管理可以使多个团队成员同时在代码库上开发不同的功能和新特性，简化了并行开发的过程。开发人员可以使用 Git 分支来隔离他们的工作，不影响其他开发人员的工作，同时可以方便地合并多个分支，使开发环节更清晰明确。
- **回溯和还原**：Git 允许开发人员回到任何一个历史版本，并进行修改和还原操作，这是非常简单、快速和可靠的。这意味着开发人员可以找到系统中的问题，快速定位问题源头，并进行解决。
- **远程协作**：Git 可以让团队成员在不同的地点协作开发，而不会受到距离和时间限制。团队成员可以在本地上建立与远程代码库的连接，并轻松推送和拉取变更。这使得团队内成员的协作更加自然、高效。

非常推荐大家掌握必要的 Git 操作，这对大家协作进行大作业的开发非常有效。下面的教学是一个较为精炼的版本，且仅给出 Linux 上的执行命令，更多细节可点击文章末尾的详细教程链接查询。

### 创建 Git 可管理的仓库

从已有文件夹创建：

```shell
$ pwd
/Users/OSH/learngit
$ git init
Initialized empty Git repository in /Users/OSH/learngit/.git/
```

版本库由 `.git` 文件夹来管理，请不要对此文件夹进行手动操作！

### 进行 Git 版本库的提交和修改

在刚刚创建好的版本库里，假设有一个 `readme.txt` 文件，内容如下：

```
Git is a version control system.
Git is free software.
```

用命令 `git add` 把文件添加到仓库：

```shell
$ git add readme.txt
```

用命令 `git commit` 把文件提交到仓库：

```shell
$ git commit -m "wrote a readme file"
[master (root-commit) eaadf4e] wrote a readme file
 1 file changed, 2 insertions(+)
 create mode 100644 readme.txt
```

这个命令的完整含义是：在本地 Git 代码库中提交了一次更新，这次更新的说明信息是「wrote a readme file」。它会将本次的修改实际保存在本地 Git 代码库中，但不会影响到远程分支。

`commit` 可以一次提交很多文件，所以可以多次 `add` 不同的文件，一般会用到下面这条命令来添加当前目录的所有文件。不过，你可以在仓库目录下编写一个名为 `.gitignore` 的文本文件，并将不需要维护版本的文件/文件夹的相对路径逐行写入，这可以避免维护对项目版本无影响的文件，如人工智能项目中的模型文件就可以避免维护。

```shell
$ git add .
```

### Git 版本库的回溯和撤销回溯

每次 `commit` 之后，新状态会以一个哈希值的形式标识。使用 `git log` 命令可以查看所有的提交记录。

```shell
$ git log
commit 1094adb7b9b3807259d8cb349e7df1d4d6477073 (HEAD -> master)
Author: OSH-TA <fake-email@qq.com>
Date:   Fri May 18 21:06:15 2022 +0800

    append message

commit e475afc93c209a690c39c13a46716e8fa000c366
Author: OSH-TA <fake-email@qq.com>
Date:   Fri May 18 21:03:36 2022 +0800

    add docs
```

如上面的 `1094adb7b9b3807259d8cb349e7df1d4d6477073` 就代表最新的状态，以 `HEAD` 标识。实际上这一长串用前面几位就可以标识，在使用时不需要复制整个串。

注：`HEAD` 的上一个版本是 `HEAD^`，上上一个版本就是 `HEAD^^`，如果往上 100 个版本可以写成 `HEAD~100`，也可以用哈希值来标识。

如果现在要把当前版本 `append message` 回退到上一个版本 `add docs`，可以使用`git reset`命令：

```shell
$ git reset --hard HEAD^
HEAD is now at e475afc add docs
```

如果现在又想撤销回退，并且上面的命令行窗口还没有被关掉，就可以顺着往上找到目标版本的哈希值，指定回到未来的某个版本。

```shell
$ git reset --hard 1094a
HEAD is now at 83b0afe append message
```

但如果回退到了某个版本，关掉了电脑，之后想恢复到新版本而找不到新版本的 `commit id` 怎么办？Git 提供了命令 `git reflog` 用来记录你的每一次命令：

```shell
$ git reflog
e475afc HEAD@{1}: reset: moving to HEAD^
1094adb (HEAD -> master) HEAD@{2}: commit: append message
e475afc HEAD@{3}: commit: add docs
eaadf4e HEAD@{4}: commit (initial): wrote a readme file
```

从输出可知，`append message` 的 commit id 是 `1094adb`。

### Git 版本库的分支的创建与切换

一般来说，`main` / `master` 分支中的内容是程序员认为稳定、可靠的版本。如果想要在此基础上进行新特性的开发，又担心产生 bug，则可以创建一个新的分支，在新的分支上改动，等测试稳定后再与主分支合并，或者因 bug 而直接丢弃新分支。这比起回退版本来说更加可靠和容易。

首先，我们创建 `dev` 分支，然后切换到 `dev` 分支：

```shell
$ git checkout -b dev
Switched to a new branch 'dev'
```

`git checkout` 命令加上 `-b` 参数表示创建并切换。然后，用`git branch`命令查看当前分支：

```shell
$ git branch
* dev
  master
```

`git branch` 命令会列出所有分支，当前分支前面会标一个 `*` 号。对新分支的改动不会影响原来的分支，当新分支的工作完成后，就可以切换回原来的分支：

```shell
$ git checkout master
Switched to branch 'master'
```

然后，把 `dev` 分支的工作成果合并到 `master` 分支上：

```shell
$ git merge dev
Updating d46f35e..b17d20e
Fast-forward
 readme.txt | 1 +
 1 file changed, 1 insertion(+)
```

合并后，`readme.txt` 的内容就和 `dev` 分支的最新提交是完全一样的。合并完成后，就可以放心地删除 `dev` 分支了：

```shell
$ git branch -d dev
Deleted branch dev (was b17d20e).
```

### Git 版本库的远程仓库

这一小节的远程仓库的托管方是指 GitHub。本地 Git 仓库和 GitHub 仓库之间的传输是通过 SSH 加密的，请按照如下步骤进行设置：

- 创建 SSH Key。在用户主目录下，查找有没有 `.ssh` 目录，如果有，再查找这个目录下有没有 `id_rsa` 和 `id_rsa.pub `这两个文件。如果已经有了，可直接跳到下一步。如果没有，打开 Shell 创建 SSH Key：

  ```shell
  $ ssh-keygen -t rsa -C "youremail@example.com"
  ```

  `id_rsa` 是私钥，不能泄露出去，`id_rsa.pub` 是公钥，可以放心地告诉任何人。

- 点击 [Account Settings - SSH keys](https://github.com/settings/keys) - Add SSH key，填上任意 Title，在 Key 文本框里粘贴 `id_rsa.pub` 文件的内容，然后 Add Key 即可；

- 假定你有若干电脑，只要把每台电脑的 Key 都添加到 GitHub，就可以在每台电脑上往 GitHub 推送了。

#### 创建一个新的 GitHub 远程仓库

这一部分留做练习，请大家自行探索完成。

#### 将一个空的 GitHub 远程仓库与本地仓库关联

在本地的 `learngit` 仓库下运行命令：

```shell
$ git remote add origin git@github.com:OSH-TA/learngit.git
```

请注意把上面的 `OSH-TA` 替换成你自己的 GitHub 账户名，添加后，远程库的名字为 `origin`，这是 Git 默认的叫法。下一步，就可以把本地库的所有内容推送到远程库上：

```shell
$ git push -u origin master
Counting objects: 20, done.
Delta compression using up to 4 threads.
Compressing objects: 100% (15/15), done.
Writing objects: 100% (20/20), 1.64 KiB | 560.00 KiB/s, done.
Total 20 (delta 5), reused 0 (delta 0)
remote: Resolving deltas: 100% (5/5), done.
To github.com:michaelliao/learngit.git
 * [new branch]      master -> master
Branch 'master' set up to track remote branch 'master' from 'origin'.
```

把本地库的内容推送到远程，用 `git push` 命令，实际上是把当前分支 `master` 推送到远程。

由于远程库是空的，我们第一次推送 `master` 分支时，加上了 `-u` 参数，Git 不但会把本地的 `master` 分支内容推送的远程新的 `master` 分支，还会把本地的 `master` 分支和远程的 `master` 分支关联起来，在以后的推送或者拉取时就可以简化命令。推送成功后，可以立刻在 GitHub 页面中看到远程库的内容已经和本地一模一样。从现在起，只要本地作了提交，就可以通过命令：

```shell
$ git push origin master
```

把本地 `master` 分支的最新修改推送至 GitHub。

#### 从已有的 GitHub 仓库克隆到本地

用命令 `git clone` 克隆一个本地库：

```shell
$ git clone git@github.com:OSH-TA/gitskills.git
Cloning into 'gitskills'...
remote: Counting objects: 3, done.
remote: Total 3 (delta 0), reused 0 (delta 0), pack-reused 3
Receiving objects: 100% (3/3), done.
```

如果有多个人协作开发，那么每个人各自从远程克隆一份就可以了。

#### 协作时，将新的远程更改合并到本地

多人协作时，大家都会往分支上推送各自的修改。现在，模拟一个你的小伙伴 A 也参与你们的项目。如果 A 已经向远程分支推送了他的提交，而碰巧你也对同样的文件作了修改，并试图推送：

```shell
$ cat env.txt
env
$ git add env.txt
$ git commit -m "add new env"
[dev 7bd91f1] add new env
 1 file changed, 1 insertion(+)
 create mode 100644 env.txt
$ git push origin dev
To github.com:OSH-TA/learngit.git
 ! [rejected]        dev -> dev (non-fast-forward)
error: failed to push some refs to 'git@github.com:OSH-TA/learngit.git'
hint: Updates were rejected because the tip of your current branch is behind
hint: its remote counterpart. Integrate the remote changes (e.g.
hint: 'git pull ...') before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.
```

推送失败，因为你的小伙伴的最新提交和你试图推送的提交**有冲突**，Git 提示我们先用 `git pull` 把最新的提交抓下来，然后，在本地合并，解决冲突，再推送。

多人协作的工作模式通常是这样：

1. 首先，可以试图用 `git push origin <branch-name>` 推送自己的修改；
2. 如果推送失败，则因为远程分支比你的本地更新，需要先用 `git pull` 试图合并；
3. 如果合并有冲突，则解决冲突，并在本地提交；
4. 没有冲突或者解决掉冲突后，再用 `git push origin <branch-name>` 推送就能成功！

### 练习

- Git 如何做到在不同提交状态快速切换的？可参考 [Git 教程](https://www.liaoxuefeng.com/wiki/896043488029600)。

- **「必做」**在你自己的账号下（而不是 OSH-2023 组织）创建一个新的、**私有的** GitHub 远程仓库，将其命名为 osh-2023-labs，然后邀请三位助教（id 分别为：liuly0322，SproutNan，IrisesD）成为这个仓库的共同贡献者（collaborators），以便后续实验的批改。这个仓库的目录应具有类似如下清晰的结构：

  ```shell
  - osh-2023-labs
    - lab0
      - phase1.md
      - phase2.md
      - ...
    - lab1
    - lab2
    - lab3
    - lab4
  ```

- 尝试编写一个文本文件和一个二进制文件（如 word 文档），执行本节所述的教程。

- 学习如何用精简的语言描述一次 commit。可以从分类的角度考虑，如：

  ```
  docs: add finial report
  feat: add fork
  fix: typo
  ...
  ```

## Markdown

我们 **强烈建议** 同学们使用 Markdown 撰写实验报告。因此，本节将推荐一些好用的 Markdown 编辑器，并介绍一些 Markdown 常用语法。如果你已有用着顺手的编辑器并掌握了基本语法，可以直接开始练习。

### Markdown编辑器

- Typora：支持Windows， OS X和Linux，支持即时渲染以及多种文档格式转换
- VSCode：推荐插件：Markdown All in One, Markdown Preview Enhanced, Markdown Preview Github Styling

### Markdown常用语法

#### 标题

在行首插入 1 到 6 个 # ，对应到标题 1 到 6 阶，例如：

```text
# this is H1
## this is H2
###### this is H6
```

#### 字体

Markdown 使用星号（*）和底线（_）作为标记强调字词的符号，你可以随便用你喜欢的样式，唯一的限制是，你用什么符号开启标签，就要用什么符号结束。但个人感觉写中文时还是（\*）比较好用，因为它不区分全角半角，不用切换输入法。 示例：

```text
**这是加粗**
__这也是加粗__
*这是倾斜*
_这也是倾斜_
***这是加粗倾斜***
~~这是加删除线~~
```

效果如下： **这是加粗** **这也是加粗** *这是倾斜* *这也是倾斜* ***这是加粗倾斜\*** ~~这是加删除线~~

注意：强调也可以直接插在文字中间，但是如果你的 * 和 _ 两边都有空白的话，它们就只会被当成普通的符号。 如果要在文字前后直接插入普通的星号或底线，你可以用反斜线 \ 。

#### 引用

在引用的文字前加 > 即可。 在 Markdown 文件中建立一个区块引用，那会看起来像是你自己先断好行，然后在每行的最前面加上 > ：

```text
> This is a blockquote with two paragraphs. Lorem ipsum dolor sit amet,
> consectetuer adipiscing elit. Aliquam hendrerit mi posuere lectus.
> Vestibulum enim wisi, viverra nec, fringilla in, laoreet vitae, risus.
> 
> Donec sit amet nisl. Aliquam semper ipsum sit amet velit. Suspendisse
> id sem consectetuer libero luctus adipiscing.
```

效果如下：

>  This is a blockquote with two paragraphs. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aliquam hendrerit mi posuere lectus. Vestibulum enim wisi, viverra nec, fringilla in, laoreet vitae, risus.
>  Donec sit amet nisl. Aliquam semper ipsum sit amet velit. Suspendisse id sem consectetuer libero luctus adipiscing.

引用的区块内也可以使用其他的 Markdown 语法，包括标题、列表、代码区块等。

#### 列表

Markdown 支持有序列表和无序列表。 无序列表使用星号、加号或是减号作为列表标记。 示例：

```text
- 列表内容
+ 列表内容
* 列表内容

注意：- + * 跟内容之间都要有一个空格
```

有序列表则使用数字接着一个英文句点作为标记。 示例：

```text
1. 列表内容
2. 列表内容
3. 列表内容

注意：序号跟内容之间要有空格
```

列表可以嵌套，上一级和下一级之间敲三个空格即可。

```text
* 一级无序列表内容

   * 二级无序列表内容
   * 二级无序列表内容
   * 二级无序列表内容
```

要让列表看起来更漂亮，你可以把内容用固定的缩进整理好：

```text
*   Lorem ipsum dolor sit amet, consectetuer adipiscing elit.
    Aliquam hendrerit mi posuere lectus. Vestibulum enim wisi,
    viverra nec, fringilla in, laoreet vitae, risus.
*   Donec sit amet nisl. Aliquam semper ipsum sit amet velit.
    Suspendisse id sem consectetuer libero luctus adipiscing
```

#### 代码

在 Markdown 中加入代码块有两种方式： 第一种，只要简单地缩进 4 个空格或是 1 个制表符就可以，

```text
这是一个普通段落：

    这是一个代码区块。

(当然，前面要有一个空行和前面的文字分隔开)
```

效果如下：

这是一个普通段落：

```text
这是一个代码区块。
```

第二种方法似乎是更为常用:

**单行代码**：代码之间分别用一个反引号包起来即可；

```text
这里有一句代码`int i;`。
```

效果如下： 这里有一句代码`int i;`。 

**代码块**：代码之间分别用三个反引号包起来，且两边的反引号单独占一行

```text
\```
  代码...
  代码...
  代码...
\```
\ 是为了防止转译，实际是没有的。
```

效果如下：

```text
代码...
  代码...
  代码...
```

还可以在上面的 ``` 后面注明你的代码类型，可以产生相应的代码高亮。

#### 图片

很明显地，要在纯文字应用中设计一个「自然」的语法来插入图片是有一定难度的。 Markdown 使用一种和链接很相似的语法来标记图片，同样也允许两种样式： **行内式**和**参考式**。

**行内式**的图片语法看起来像是：

```text
![Alt pic](/path/to/img.jpg)

![Alt pic](/path/to/img.jpg "Optional title")
```

详细叙述如下：一个感叹号！ 接着一个方括号，里面放上图片的替代文字 * 接着一个普通括号，里面放上图片的网址或本地路径（可以是相对路径或绝对路径），最后还可以用引号包住并加上 选择性的 '标题' 文字。

**参考式**的图片语法则长得像这样：

```text
![Alt pic][id]
```

「id」是图片参考的名称，图片参考的定义方式则和连结参考一样：

```text
[id]: url/to/image  "Optional title attribute"
```

**但是在这里有两个很大的问题：** 1. 如果使用本地路径插入本地图片，不灵活不好分享，本地图片的路径更改或丢失都会造成md文档调不出图。 2. 插入网络图片，则非常依赖网络，如果是本地图片，还需要先上传到服务器上。

解决方法：

- 将本地图片上传到 Github ：在当前文档同目录或仓库根目录下新建 src 文件夹，将需要引用的图片上传至其中，在文档中使用其路径引用即可。

- 把图片存入 markdown 文件：用 base64 转码工具把图片转成一段字符串，把 base64 字符串放在文档末尾，然后在文档中通过一个id来调用。

#### 公式

Markdown 支持 LaTeX 公式。 \$  表示行内公式 \$$ 表示整行公式 访问 [MathJax](https://math.meta.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference) 参考更多使用方法。

例如：

```text
$ X\stackrel{F}{\longrightarrow}Y $
```

效果如下： $ X\stackrel{F}{\longrightarrow}Y $

受篇幅所限，这里仅介绍 Markdown 一些常用语法，想要了解更多的同学可以自行 Search。

### 练习

- **「必做」** 在你自己的仓库的lab0目录下新建文档 markdown.md 以及目录 src

- **「必做」** 在src目录中上传一张你喜欢的图片并在 markdown.md 中对其进行引用

- **「必做」** 在 markdown.md 中写下你最喜欢的一个数学公式

- **「必做」** 在 markdown.md 中加入一段C/C++代码块并对其注明代码类型以高亮

## 编程语言要求

本学期实验如无特殊声明均允许使用 C、C++、Rust 其中任意一种语言完成。本章将简单介绍需要掌握或了解的 C、C++、Rust 语言知识。这些知识并不一定会在实验中用到，但是理解它们或许能够指导你的实际开发过程并减少在各个语言文档中探索的时间。

### C 的要求

- 了解 C99 标准下几乎所有的 C 用法，这是考虑到 C 的标准语言特性并不太多，更进阶的用法也更倾向于依赖编译器特性或是标准用法的巧用；
- 了解基本的宏知识，例如宏函数中参数使用时需要括号包裹，这是因为调用 Linux API 时需要用到宏；
- 掌握查询 Linux API 文档的方法，无论是使用 `man`、`tldr` 或是直接查看网页版 man 文档。

### C++ 的要求

- 掌握基本的 C 用法；
- 宏，如 C 要求所述；
- Linux API 文档，如 C 要求所述;
- 了解一些 Morden C++ 的用法，可以为你减少编程工作量。

??? tips "关于 Modern C++"

    如果你选择使用 C++ 完成实验，那么<span style="color:red">请不要将 C++ 代码写得宛如 C 一样</span>。现代的、C++11 标准后的 C++ 增添了众多强有力的工具，能够帮助你以更高效、更简洁的手段达成目标。
    
    如需全方位的了解 Modern C++，可以阅读《A Tour of C++》以及进阶的例如《Modern Effective C++》等书籍，其中详细讲解了 Modern C++ 中的一些设计理念和最佳实践。也可以参考《[现代 C++ 教程](https://changkun.de/modern-cpp/zh-cn/00-preface/)》。
    
    下面我们也给出一些 tips：
    
    - （STL）栈上的固定大小数组 `int a[12]` 使用 `std::array` 代替；
    - （STL）堆上的不定长数组 `new int[n]` 使用 `std::vector` 代替；
    - （STL）使用 `std::string` 代替 `chat buf[N]`。用 `s.c_str()` 或是 `&s[0]` 获取 C 字符串的不可变或可变指针。
    
    以下是一些进阶内容，可以选择性了解：
    
    - （STL）`<memory>` 中 `unique_ptr`、`shared_ptr` 和 `weak_ptr` 智能指针可以实现自动在合适的时刻释放资源，可以配合 `std::thread` 等使用。<span style="color:blue">可以尝试使用智能指针替换所有裸指针</span>；
    - （性能）了解左值，右值和将亡值的概念。了解移动语义、`std::move` 的使用和完美转发；
    - （设计）了解 lambda 表达式及其捕获方式；
    - （设计）了解虚函数及如何通过继承实现多态。

### Rust 的要求

??? tips "关于 Rust"

    安装教程可以在 [官网安装界面](https://www.rust-lang.org/tools/install) 查看。
    详细语言教程也可以在 [官网教程汇总界面](https://www.rust-lang.org/learn) 按需查阅。
    其中重点是 [Rust Book](https://doc.rust-lang.org/book/) 这份教程，其在高低层次上都对 Rust 进行了详细的解析。
    而如果你之前只了解过 C 开发，可以着重关注 Rust Book 中的以下概念：
    
    - Ownership，lifetime，borrow，这些是 Rust 最核心的概念之一；
      - 对应章节是 [Understanding Ownership](https://doc.rust-lang.org/book/ch04-00-understanding-ownership.html) 和 [Validating References with Lifetimes](https://doc.rust-lang.org/book/ch10-03-lifetime-syntax.html)。
    - 模式匹配（pattern matching），如 `match` 和 `if let` 的用法；
      - <span style="color:blue">用好模式匹配可以极大简化你的代码</span>
    - Cargo 的使用，从而完成 Rust 的依赖管理及编译等工作；
      - 安利：`cargo clippy` 可以对代码进行 <span style="color:blue">静态检查</span>，帮助你 <span style="color:blue">重构代码</span>。C++ 也有类似的工具 clangd（是 CLion 的默认分析器，也和 VSCode 配合的很好），可以参照官网安装使用。
    - 并发编程，包括多线程（互斥锁，channel）、协程（async）等；
    - 函数式编程（functional programming），如 `map`、`filter` 的用法；
    - 基本的 unsafe Rust 用法，可能会在调用外部库时用到。
    
    在本课程实验之外，也可以拓展了解以下内容：
    
    - 智能指针（smart pointer），C++ `<memory>` 库中也有相似实现；
    - 错误处理（error handling），Rust 错误处理与 Go 有类似之处，如有经验可以对照学习，但本次实验应该不会在错误处理上有较高要求，把握概念即可；
    - Traits（有译作特质的），类似于接口，可用于实现多态。
    
    其他的例如 [Rust by Example](https://doc.rust-lang.org/rust-by-example/) 也是很优质的教程，也能让你迅速找到自己所需要的写法。

实验自愿使用 Rust 语言，不会做进一步的要求。
如果你选择在实验中使用 Rust 语言，你依然可以从助教处获取一些语言使用上的基础帮助，并与其他同学在同一标准下进行评测。

#### 实验中的优缺点

选择 Rust 可能会在本轮实验中遇见以下优缺点：

优点：

- Rust 的基础库和包管理比 C++ 更全面而更易用；
- Rust 的内存安全保证可以尽量让你避免遇到难以 debug 的 segmentation fault 问题；
- Rust 对于一些比较现代的技术，例如协程，有相对更好的支持，在实验中需要遇到类似情况时会更方便。

缺点：

- Rust 的学习曲线稍显陡峭，尤其是从原汁原味的 C 出发的话；
- Rust 不能直接使用 Linux API（由众多 C header 文件提供），下文我们将详细讨论这个问题。

#### 外部库的要求

如优缺点中所言，尽管 Rust 标准库相对较丰富，但需要调用特定 Linux API 时，Linux 提供的是 C header 文件。从头文件到 Rust 的绑定较为麻烦，所以如无特地声明，我们默认允许使用 Rust 语言时调用以下这些外部库（后续可能会增充此列表，但不会删减）：

- `nix`
- `libc`

如你不满足于这些外部库、希望使用其他外部库时，请尽量提前询问助教。在最后验收实验时，助教会参考以下条件对除上述列表以外的外部库进行评定：

- 此外部库是否仅是 Linux C API FFI？如是，则允许；
- 此外部库使用方法是否与原本 C 中调用的方法相类似？如是，参考相似程度允许；
- 此外部库是否破坏了原本的实验设计目标？如是，则禁止；如否，参考和实验目标相关程度允许。

如果一个外部库被禁止，实验中对应项目将可能被酬情扣分。

### 调试

???+ tips "预期目标"

    了解 `-g` 编译选项的作用，至少掌握 GDB/VSCode 调试中的一种。

在本学期的实验中，你可能会写出各式各样的 bug，用好 debugger 往往可以大幅提升你的调试效率。本部分主要介绍 C++ 程序的调试。

#### GDB 调试

本节会对 C/C++ 常用的 [GDB (GNU Project debugger)](https://www.sourceware.org/gdb/) 进行介绍。

例如，在以下的程序中，运行到 `throw 20` 时会抛出一个异常

```c++
#include <iostream>
void foo()
{
    throw 20;
}
int main()
{
    std::cout << "testing";
    foo();
    return 0;
}
```

如果你直接编译（`g++ example.cpp`）并运行，会得到以下输出：

```bash
./a.out
terminate called after throwing an instance of 'int'
[1]    12079 IOT instruction (core dumped)  ./a.out
```

此时只知道程序发生了错误，但并不知道是哪里出了问题。这时候就可以使用 gdb 来进行调试（取代简单的 print）：

在编译时使用 `g++ example.cpp -g`，完成编译后，运行 `gdb a.out`，会进入以下界面：

```
<some intro here>
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from a.out...
(gdb)
```

此时输入 `run` 然后回车，gdb 就会运行你的程序，得到：

```
Starting program: /home/catoverflow/Projects/tmp/a.out
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib/libthread_db.so.1".
terminate called after throwing an instance of 'int'
Program received signal SIGABRT, Aborted.
0x00007ffff7ae534c in __pthread_kill_implementation () from /usr/lib/libc.so.6
(gdb)
```

说明程序发生了错误，并回到终端。输入 `bt` (back trace) 并回车就可以看到程序的调用栈：

```
(gdb) bt
#0  0x00007ffff7ae534c in __pthread_kill_implementation () from /usr/lib/libc.so.6
#1  0x00007ffff7a984b8 in raise () from /usr/lib/libc.so.6
#2  0x00007ffff7a82534 in abort () from /usr/lib/libc.so.6
#3  0x00007ffff7dfc7ee in __gnu_cxx::__verbose_terminate_handler () at /usr/src/debug/gcc/libstdc++-v3/libsupc++/vterminate.cc:95
#4  0x00007ffff7e08c4c in __cxxabiv1::__terminate (handler=<optimized out>) at /usr/src/debug/gcc/libstdc++-v3/libsupc++/eh_terminate.cc:48
#5  0x00007ffff7e08cb9 in std::terminate () at /usr/src/debug/gcc/libstdc++-v3/libsupc++/eh_terminate.cc:58
#6  0x00007ffff7e08f5e in __cxxabiv1::__cxa_throw (obj=<optimized out>, tinfo=0x555555557db0 <typeinfo for int@CXXABI_1.3>, dest=0x0)
    at /usr/src/debug/gcc/libstdc++-v3/libsupc++/eh_throw.cc:95
#7  0x00005555555551a4 in foo () at example.cpp:5
#8  0x00005555555551c6 in main () at example.cpp:10
```

调用栈将函数调用自底向上显示，最上面的就是最后被调用的函数，在这里上面的都是链接的系统库，下面的两个则是我们自己的函数。这时就很容易发现错误发生在 `foo` 中，代码的第五行。

最后，输入 `q` 并回车就可以退出了。除了上面的 bt，常用的指令还有 `break`（添加断点），用 `attach` 命令连接到线程（在多线程调试中非常有用）等等，具体的用法可以查阅相关文档。

??? tips "编译时参数 -g 的作用？"

    在终端输入 `man gdb` 并找到 `-g` 的帮助文档，可以看到：
    
    ````
    -g  Produce debugging information in the operating system's native format (stabs, COFF, XCOFF, or DWARF).  GDB can work with this
           debugging information.
    ````
    
    简单来说，这个参数会在编译的时候加入额外的调试信息，比如代码所在的行号（在 gdb 报错的时候非常有用，还可以拿来插入断点）等等。否则 gdb 调试的就是一个简单的二进制文件，能输出的信息会少很多。
    
    你也可以不加 `-g`，然后对比一下 gdb 的输出。

#### VSCode 调试

在命令行里用 gdb debug 虽然简单，但是很多时候不够直观方便。你也可以尝试用 IDE 自带的 debug 功能（有些是基于 gdb 的，但是提供了非常方便的操作界面）。这里以 VSCode 为例：

VSCode 调试的核心是工作区的 `launch.json` 文件。在安装了 C/C++ 插件后，点击调试，应该可以自动在工作区的 `.vscode` 文件夹下生成一份与下面类似的 `launch.json` 文件。正确配置后，点击调试即可启动断点调试功能。

```json
{
  "version": "2.0.0",
  "configurations": [
    {
      "name": "(gdb) Launch",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/可执行文件名",
      "args": [],
      "cwd": "${workspaceFolder}",
      "environment": [],
      ...... // 其他配置选项，这里略去
    }
  ]
}
```

以下配置选项需要关注：

- `cwd`：当前的工作路径，调试若需要用到命令行参数，其中的文件路径就是相对工作路径的；
- `program`：程序位置。注意这里的程序需要是带 `-g` 选项编译出来的；
- `args`：命令行参数数组。例如，如果我想调试 `./main -o output.txt input.txt`：
  ```json
  ["-o", "output.txt", "input.txt"]
  ```
- `environment`：参考 [官方文档](https://code.visualstudio.com/docs/cpp/launch-json-reference#_environment)。

???+ question "每次修改代码后需要再次手动编译？"

    是的，对于单个文件或 CMake 项目等具体情况，VSCode 都有很方便的一键编译调试功能，但这里介绍的是一般情况，需要先编译出一个带有调试信息的可执行文件，再用 VSCode 对该可执行文件调试。所以每次修改源文件后都需要再次手动编译。

在完成了 lab2 后，你会对 `args` 和 `environment` 选项有更深刻的理解。

IDE 的断点调试往往都比较直观，这里就不再介绍了，同学们可以自行探索。
