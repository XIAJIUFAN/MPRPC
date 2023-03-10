#set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callfriendservice.cc ../friend.pb.cc)

add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf) # 生成provider需要链接mprpc 和 protobuf