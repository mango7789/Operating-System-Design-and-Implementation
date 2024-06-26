**Copy-on-write 测试**：fork() 在进行状态机复制时，虽然理论上需要做出完整的状态复制，但本着 “复制的数据可能立即被浪费掉” 的观察，操作系统希望只在 “不得不复制” 时才复制。

现代操作系统会维护页面的共享情况，并在 fork 后将父子进程的地址空间都标记为 read-only，而当双方中的任何一个进程写入发生 page fault 时，才复制一页。对于连续的 fork()，则会产生多个进程共享页面的情况。
