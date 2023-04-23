# 测试及评分说明

## 对程序要求

编程语言：C/C++，使用其他语言（如 Rust）请和助教联系确认，核心功能部分的代码不能使用库。

监听 IP：`0.0.0.0`

监听端口：`8000`

文件根目录：程序运行时的当前目录

## 测试环境

- Web Server 运行于树莓派 3B+ (Raspbian GNU/Linux 9, Linux raspberrypi 4.14.98-v7+)
- 测试机器在树莓派同一局域网下另一台 Linux 机器（同学们在测试时注意关闭树莓派的各类防火墙）

## 测试指标

测试由正确性测试和性能测试组成。

### 正确性测试（40%）

- 主要考察指标：
  - 是否可以正确响应
  - 返回头部是否正确
  - 返回内容是否正确

注意：正确性测试过程中，可能会有文件的删改。

### 性能测试（60%）

- 使用 benchmark 工具 [Siege](https://github.com/JoeDog/siege) (v4.0.4) 测试；
- 例子：`siege -c 50 -r 10 http://127.0.0.1:8000/index.html`
- 主要考察指标：
  - Availability
  - Throughput
  - Concurrency

## 关于 URL 规范

HTTP 中的 URL 有着非常复杂而详细的定义，可以通过查阅参考资料中的 RFC 1738 了解。本实验中我们不要求（也不推荐）去完善实现 URL 规范（不是本实验的重点）。我们的测试仅包含最常见的情况，并保证只包含大小写英文字母，数字，连字符下划线和斜线，如：

- http://127.0.0.1/dir/index0.html
- http://127.0.0.1/.a/
- http://127.0.0.1/b/
- http://127.0.0.1/ind.html
- http://127.0.0.1/ind.html/