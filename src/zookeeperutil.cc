#include "mprpcapplication.h"
#include "zookeeperutil.h"

#include <iostream>

// 全局的watcher观察器 zkserver给zkclient的通知
void global_watcher(zhandle_t *zh, int type, 
        int state, const char *path,void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT) // 回调的消息类型是与会话相关的消息类型
    {
        if (state == ZOO_CONNECTED_STATE) // zkclient和zkserver连接成功
        {
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem); // 给信号量资源加1
        }
    }
}

ZkClient::ZkClient()
    :m_zhandle(nullptr)
{
}

ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle); // 关闭句柄，释放资源
    }
}

// zkclient启动连接zkserver
void ZkClient::start()
{
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;

    /*
    zookeeper_mt: 多线程版本
    zookeeper的客户端API提供了三个线程
        API调用线程
        网络IO线程 (底层使用Poll)
        watcher回调线程
    */

    // zookeeper_init是以异步方式创建会话
    // 启动了两个线程：网络IO线程和watcher回调线程
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == m_zhandle) 
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }// m_zhandle句柄资源创建成功，并不意味着连接成功

    sem_t sem;
    sem_init(&sem, 0, 0); // 初始化信号量资源为0
    zoo_set_context(m_zhandle, &sem); // 在句柄资源上设置上下文，即给指定的句柄添加一些额外的信息

    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}


// 在zkserver上根据指定的path创建znode节点
void ZkClient::Create(const char* path, const char* data, int datalen, int state)
{
    char path_buffer[128] = { 0 };
    int bufferlen = sizeof(path_buffer);
    int flag;
    // 先判断path表示的znode节点是否存在,如果存在，就不再重复创建了
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (ZNONODE == flag) // 表示path的znode节点不存在
    {
        // 创建指定path的znode节点(通过state指定是永久性节点还是临时性节点，默认永久性节点)
        flag = zoo_create(m_zhandle, path, data, datalen,
                    &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);

        if (flag == ZOK) 
        {
            std::cout << "znode create success...path:" << path << std::endl;
        }
        else
        {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error...path:" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    } 
}

// 根据参数指定的节点路径，获取znode节点的值
std::string ZkClient::GetData(const char* path)
{
    char buffer[64] = { 0 };
    int bufferlen = sizeof(buffer);

    int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
    if (flag != ZOK)
    {
        std::cout << "get znode error...path:" << path << std::endl;
        return "";
    }
    else 
    {
        return buffer;
    }
}
