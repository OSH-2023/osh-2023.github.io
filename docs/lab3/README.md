# 简介

本实验通过 socket 编程，利用多线程技术，实现一个简易的支持并发的 HTTP/1.0 服务器，并可以利用线程复用、I/O 复用、缓存、预测等机制，提高 HTTP 服务器的性能。

???+ warning "重要"

    本实验作为代码编程实验，**会进行查重**，一切抄袭行为后果自负！

???+ question "如何提问？"

    [提问的智慧](https://lug.ustc.edu.cn/wiki/doc/smart-questions/)

    以下是一些要点：

    - 提问前，请先 [STFW 和 RTFM](https://lug.ustc.edu.cn/wiki/doc/smart-questions/#rtfm-%E5%92%8C-stfw%E5%A6%82%E4%BD%95%E7%9F%A5%E9%81%93%E4%BD%A0%E5%B7%B2%E5%AE%8C%E5%85%A8%E6%90%9E%E7%A0%B8%E4%BA%86)，ChatGPT 也是一个可考虑的选择；
    - 尽量使你的问题是可复现的、明确的，并在此基础上裁剪定位到会导致出问题的核心代码；
    - 详细描述问题，并**提供相关指令及相关问题的报错截图**，如果涉及系统问题请提供系统版本。

    **优先**在实验文档评论区、[Issue 区](https://github.com/OSH-2023/osh-2023.github.io/issues) 及课程群内讨论实验的相关问题以便解决共性问题。

## 实验要求

编程语言：C/C++/Rust，核心功能部分的代码不能使用外部网络库。

本次实验评分从正确性和性能两方面评价，具体测试和评分标准详见「测试及评分」部分。

## 实验提交材料

请按照以下目录结构组织你的 GitHub 仓库：

```
.                       // Git 根目录
├── lab3                // 实验三根目录
│   ├── README.md       // 实验三实验报告
│   └── src             // 实验三文件根目录
│       ├── Makefile    // Makefile 或其他文件
│       ├── server      // 编译出的可执行文件，这里只是说明文件位置，不要提交
│       └── server.c    // 源代码
└── ...                 // 其他文件
```

注意：

- 实验报告中请注明编译的方法和运行方法，使得 `./lab3/src` 目录下源代码在测试环境中编译出文件名为 `server` 的二进制可执行文件，否则可能导致测试失败。
- 不要提交任何编译中间文件和编译出的可执行二进制文件。

## 评分规则和要点

本次实验满分为 100%，其中：

- 实验报告占 20%，包括：
  - 编译和运行方法说明；
  - 描述整体设计，使用的技术；
  - 使用 siege 测试的结果和分析；
- 正确性测试占 60%；
- 选做部分额外加分占 20%，超出部分将被截断至 20%。

注意：

- **合理控制实验报告字数，介绍清楚即可；**
- **对 Git 的使用要求及扣分说明参照 lab2；**
- **如果确认有抄袭情况，该实验分数记为 0 分。**

## 参考资料

- RFC 2616: <https://tools.ietf.org/html/rfc2616>
- RFC 1738: <https://tools.ietf.org/html/rfc1738>
- socket(2): <http://man7.org/linux/man-pages/man2/socket.2.html>
- Unix Socket Tutorial: <https://www.tutorialspoint.com/unix_sockets/>
