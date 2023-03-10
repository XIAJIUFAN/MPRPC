#include <iostream>
#include <string>

#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "logger.h"

/*
FriendService原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/
class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendList(uint32_t userid)
    {
        std::cout << "do GetFriendList Service! userid:" << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("gao yang");
        vec.push_back("liu hong");
        vec.push_back("wang shuo");
        return vec;
    }

    // 重写基类方法
    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t userid = request->userid();

        // 执行本地业务
        std::vector<std::string> friendList = GetFriendList(userid);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");

        for (std::string& name: friendList)
        {
            std::string* p = response->add_friends();
            *p = name;
        }
        done->Run();

    }

};

int main(int argc, char** argv)
{
    LOG_INFO("first log message!");
    LOG_ERR("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
    
    // 调用框架的初始化操作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把FriendService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    // 启动一个rpc服务节点（run以后，进程进入阻塞状态，等待远程的rpc调用请求）
    provider.Run();

    return 0;
}
