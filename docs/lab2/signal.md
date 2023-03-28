# 信号处理

这一部分需要处理 Ctrl-C 的按键。

在使用 shell 的时候按下 Ctrl-C 可以丢弃当前输入到一半的命令，重新显示提示符并接受新的命令输入。当有程序运行时，按下 Ctrl-C 可以终结运行中的程序，立即回到 shell 开始新的命令输入（shell 没有随程序一起结束）。

例如（`^C` 表示遇到 Ctrl-C 的输入）：

```shell
$ echo taokystrong
taokystrong
$ echo taokystrong^C
$ sleep 5  # 几秒之后
^C
$          # sleep 没有运行完
$ ^C
```

请为你的 shell 实现对 Ctrl-C 的处理。

## 信号

按下 `CTRL-C` 实际是向进程发送了 `SIGINT` 信号，这一信号通常会终止当前正在运行的程序。

回想一下，你的终端窗口本身正在运行一个 shell 程序，需要确保 `CTRL-C` 不会停止 shell 程序本身。例如，如果你现在在自制的 shell 中尝试 `CTRL-C`，信号将直接发送给 shell 进程，并停止 shell，而我们希望信号只影响我们的 shell 创建的子进程。

## 进程组

每个进程都有自己的独特的 `pid`，标识进程 id。除此之外，每个进程还有一个 `pgid`，即进程组 id。一个进程的默认 `pgid` 与它父进程相同。可以通过 `getpgid()`, `setpgid()`, `getpgrp()`, 或 `setpgrp()` 来获取和设置进程组 id。

你的 shell 可能会打开一个需要多个进程的程序，但这些进程都会从最初的一个进程继承相同的 `pgid`。所以你可以尝试设置每个 shell 子进程到其对应的进程组。设置时，`pgid` 应该等同于 `pid`。

## 前台进程组

每个终端都有一个关联的前台进程组 ID。当输入 `CTRL-C` 时，终端会向前台进程组内的每个进程发送一个 `SIGINT` 信号。可以使用 `tcsetpgrp(int fd, pid_t pgrp)` 更改哪个进程组在终端的前台。`fd` 参数应设置为标准输入 0。

|         要求          | 满分 |
| :-------------------: | :--: |
| CTRL+C 正确终止子进程 | 10%  |
|  CTRL+C 可以丢弃命令  | 10%  |