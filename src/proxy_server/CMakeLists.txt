aux_source_directory(. SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/base BASE_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/base/database MYSQL_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/base/redis REDIS_SRCS)

include_directories(${PROJECT_SOURCE_DIR}/src/base)
include_directories(${PROJECT_SOURCE_DIR}/src/base/database)
include_directories(${PROJECT_SOURCE_DIR}/src/base/redis)
include_directories(${PROJECT_SOURCE_DIR}/src/base/pb/protocol)


get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

foreach(file ${SRC_LIST})
  message("file='${file}'")
endforeach()

foreach(file ${BASE_SRCS})
    message("file='${file}'")
endforeach()

add_library(proxy_proto 
${PROJECT_SOURCE_DIR}/src/base/pb/protocol/BaseDefine.pb.cc
${PROJECT_SOURCE_DIR}/src/base/pb/protocol/User.pb.cc
)
target_link_libraries(proxy_proto protobuf pthread)

add_library(mysql_pool ${MYSQL_SRCS})
target_link_libraries(mysql_pool mysqlclient muduo_base pthread)

add_executable(proxy_server main.cpp ${SRC_LIST} ${BASE_SRCS})
target_link_libraries(proxy_server mysql_pool register_proto pthread muduo_base muduo_net)