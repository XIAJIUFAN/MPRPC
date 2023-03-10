# MPRPC
### 介绍

基于google::protobuf中提供的RPC服务实现了一个用于RPC通信的分布式网络通信框架

### 技术栈

- 集群和分布式概念以及原理
- RPC远程过程调用原理以及实现
- ProtoBuf数据序列化和反序列化协议
- ZooKeeper分布式一致性协调服务应用以及编程
- muduo网络库编程
- conf配置文件读取
- CMake构建项目集成编译环境
- github管理项目

### 项目工程目录

> bin: 可执行文件
>
> build: 项目编译文件
>
> lib: 项目库文件
>
> src: 源文件
>
> test: 测试代码
>
> example: 框架代码使用范例
>
> CMakeLists.txt: 顶层的cmake文件
>
> README.md: 项目自述文件
>
> autobuild.sh: 一键编译脚本

### 开发工具

Linux下VScode平台开发，gdb调试，Cmake构建，Linux shell输出项目编译脚本

### 项目要点

- 项目整体基于google::protobuf中提供的RPC服务实现了一个用于RPC通信的分布式网络通信框架
- 使用muduo开源网络库实现高并发的RPC同步调用请求处理
- 网络数据发送格式采用自定义协议解决粘包问题（头部为4字节长度信息，中间存放请求字符串，末尾存放请求的RPC方法调用所需的参数）
- 使用protobuf进行参数的序列化和反序列化
- 设计基于线程安全的缓冲队列实现异步日志的同步输出
- 采用zookeeper作为服务配置中心，用于记录发布RPC服务的服务站点配置信息
