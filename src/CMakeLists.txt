aux_source_directory(. SRC_LIST)

# 生成静态库
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)

