#set(SRC_LIST userservice.cc ../user.pb.cc)
set(SRC_LIST friendservice.cc ../friend.pb.cc)
add_executable(provider ${SRC_LIST})
target_link_libraries(provider mprpc protobuf) # 生成provider需要链接mprpc 和 protobuf