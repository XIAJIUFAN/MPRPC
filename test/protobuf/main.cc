#include "test.pb.h"
#include <iostream>
using namespace fixbug;

// int main()
// {
//     // 封装了login对象的数据
//     LoginRequest req;
//     req.set_name("zhang san");
//     req.set_pwd("123456");

//     // 对象数据序列化
//     std::string send_str;
//     if (req.SerializeToString(&send_str)) // 将对象数据序列化后存到send_str中
//     {
//         std::cout << send_str.c_str() << std::endl;        
//     }

//     // 从send_str反序列化一个login对象
//     LoginRequest reqB;
//     if (reqB.ParseFromString(send_str))
//     {
//         std::cout<< reqB.name() << std::endl;
//         std::cout<< reqB.pwd() << std::endl;
//     }
//     return 0;
// }

int main()
{
    // LoginResponse rsp;
    // ResultCode* rc = rsp.mutable_result(); 
    // rc->set_errcode(1);
    // rc->set_errmsg("登录处理失败!");

    GetFriendListsResponse rsp;
    //mutable_变量名()-->获取成员变量指针；变量名()-->获取成员变量的常引用
    ResultCode* rc = rsp.mutable_result();  
    rc->set_errcode(0);

    ////// 往list种添加user
    User* user1 = rsp.add_friend_list();
    user1->set_name("zhang san");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    User* user2 = rsp.add_friend_list();
    user1->set_name("li li");
    user1->set_age(22);
    user1->set_sex(User::WOMAN);

    std::cout << rsp.friend_list_size() << std::endl;

    ////// 获取列表第一个用户
    User user = rsp.friend_list(0); // 0是索引
    std::cout << user.name() << std::endl;
    std::cout << user.age() << std::endl;

    return 0;
}