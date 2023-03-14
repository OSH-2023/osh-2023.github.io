## 创建初始内存盘

Linux 在启动时，会首先加载初始内存盘（initrd，init ram disk）进行初始化的操作。
下面我们讲解如何创建一个最小化的 initrd。

我们首先创建一个 C 程序，代码如下：

```c
#include <stdio.h>

int main() {
    printf("Hello, Linux!\n");
    while (1) {}
}
```

**更新**：`return 0;` 被换为了 `while (1) {}`

保存为 init.c。

之后编译，**静态链接**为可执行程序：

```bash
gcc -static init.c -o init
```

创建一个新的目录用于暂存文件。
在新的目录下打包 initrd：

```bash
find . | cpio --quiet -H newc -o | gzip -9 -n > ../initrd.cpio.gz
```

这会在目录外创建压缩后的 initrd.cpio.gz 初始内存盘。

同样，我们使用 QEMU 测试效果：

```bash
qemu-system-x86_64 -kernel linux-5.16.17/arch/x86_64/boot/bzImage -initrd initrd.cpio.gz
```

当你在屏幕上看到 "Hello, Linux!" 的时候，就成功了。
如果你看不清输出的信息，又发现无法上翻，可以使用以下指令：

```bash
qemu-system-x86_64 -kernel linux-5.16.17/arch/x86_64/boot/bzImage -initrd initrd.cpio.gz -nographic -append console=ttyS0
```

注意此命令执行后 Ctrl+C 无法终止，需要关闭 Terminal 或者 kill 对应进程。

**更新**：注意：init 将会作为第一个用户态进程被启动，成为所有后续进程的父进程。此进程如果退出会导致 kernel panic。
**如果你遇到了 kernel panic，又认为自己的操作没有问题，不妨使用上方指令查看 log 来确认一下。**

### 评分标准

- 提交编译好的初始内存盘 `initrd.cpio.gz`，保证其能够显示 "Hello, Linux!"——2 分

### 文件路径

- 编译好的初始内存盘文件：置于 `/lab1/initrd.cpio.gz`
