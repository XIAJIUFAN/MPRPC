#include <iostream>

#include "mprpcapplication.h" // 把mprpc框架有关的头文件都包含到了这个头文件里
#include "friend.pb.h"
#include "logger.h"

int main(int argc, char** argv)
{
    // 整个程序启动后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    MprpcApplication::Init(argc, argv); 

    // 创建代理对象
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    fixbug::GetFriendListRequest request;
    request.set_userid(1000);
     
    // rpc方法的响应
    fixbug::GetFriendListResponse response;
    // 发起rpc方法的调用，同步的rpc调用过程 MprpcChannel::CallMethod
    MprpcController controller;
    stub.GetFriendList(&controller, &request, &response, nullptr); // RpcChannel->Rpcchannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    

    // 一次rpc调用完成，读调用的结果
    if (controller.Failed()) 
    {// 状态信息不对,说明没有正常发起一次rpc请求(发送请求到接收到响应，整个过程中没有出错)
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode()) // 只有rpc请求正常发起，接收响应数据，才对响应数据进行分析
        {
            std::cout << "rpc GetFriendList response success: " << std::endl;
            int size = response.friends_size();
            for (int i = 0; i < size; ++i) 
            {
                std::cout << "index:" << (i+1) << " name" << response.friends(i) << std::endl;
            }
        }
        else 
        {
            std::cout << "rpc GetFriendList response error: " << response.result().errmsg() << std::endl;
        }
    }
     
    return 0;
}