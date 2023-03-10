#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include <unordered_map>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Timestamp.h>


// 框架提供的专门发布rpc服务的网络对象类
class RpcProvider
{
public:
    // 这里框架提供给外部使用，可以发布rpc方法的函数接口
    // ::google::protobuf::Service是一个抽象类，protobuf生成的服务类都继承它
    void NotifyService(::google::protobuf::Service* service);

    // q启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();

private:
    // 组合EventLoop
    muduo::net::EventLoop m_eventLoop;
    struct ServiceInfo
    {
        google::protobuf::Service* m_service; // 保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; // 保存服务方法 
    };
    // 存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
    
    // 新的socket回调
    void onConnection(const muduo::net::TcpConnectionPtr&);

    // 已建立连接用户的读写事件回调
    void onMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);

    // Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const::muduo::net::TcpConnectionPtr&, google::protobuf::Message*);
};

 