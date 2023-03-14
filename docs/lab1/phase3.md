## 添加一个自定义的 Linux syscall

本节我们将为 Linux 添加一个自定义的 syscall，来获得一串字符串 "Hello, world!\n"。

首先回到 linux 源码文件夹 linux-5.16.17，添加一个文件 `custom/hello.c`：

```c
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

SYSCALL_DEFINE2(hello, char *, buf, size_t, buf_len)
{
    static const char s[] = "Hello, world!\n";
    if (strlen(s) <= buf_len) {
        return copy_to_user(buf, s, sizeof(s));
    } else {
        return -1;
    }
}
```

`SYSCALL_DEFINE2` 就是一个函数定义，此处能够很方便地同时为 32 bit 和 64 bit 声明对应函数。

此处可以直接使用 `memcpy`（内核态可以解引用用户态指针），不过 `copy_to_user` 可以协助校验目标地址是否可写。

接下来我们将此文件添加到 Linux 的编译工具链中。
打开 `Makefile` 文件，找到 `kernel/ certs/ mm/ fs/ ipc/ security/ crypto/` 所在的行，将 `custom/` 也添加到此列表中，变为：

```make
<...>
core-y                 += kernel/ certs/ mm/ fs/ ipc/ security/ crypto/ custom/
<...>
```

再在 `custom` 下创建文件 `Makefile`，内容仅为：

```make
obj-y := hello.o
```

之后编译时就会将我们自行添加的 `custom/hello.c` 文件纳入到编译和链接中了。

最后来解决 syscall 的部分。
既然我们的架构是 x86_64（或者说 amd64，兼容 x86），打开 `arch/x86/entry/syscalls/syscall_64.tbl` syscall 表文件在最后一行添加一个新 syscall：

```
<...>
548    common  hello   sys_hello
```

记住这个数字 548，之后我们会用此数字来进行调用。

然后在 `include/linux/syscalls.h` 中添加一行声明：

```c
// <...>
asmlinkage long sys_hello(char *buf, int buf_len);
// <...>
```

接下来重新编译内核，syscall 就添加完成了。

我们可以用一个 initrd 程序来进行测试。
请自行查询 [`syscall` Linux 文档](https://man7.org/linux/man-pages/man2/syscall.2.html)，编写程序测试以上自行添加的自定义 syscall，使得提供的 buffer 长度在充足和不足时均能输出希望的结果：

- 长度充足时，返回 0，buffer 中存放完整的 "Hello, world!\n"
- 长度不充足时，返回 -1

### 评分标准

- 提交编译好的内核 `bzImage`，保证能够在 buffer 长度充足时完成 syscall——2 分
- 编译好的内核 `bzImage` 也能保证在 buffer 长度不充足时完成 syscall——2 分
- 提交测试 syscall 的 initrd 源代码文件——2 分

### 文件路径

- 编译好的内核文件：置于 `/lab1/syscall/bzImage`
- 测试 syscall 的源代码文件 `/lab1/syscall/initrd.c`
