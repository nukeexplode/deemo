aux_source_directory(. SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/base BASE_SRCS)

include_directories(${PROJECT_SOURCE_DIR}/src/base)
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

add_library(register_proto 
${PROJECT_SOURCE_DIR}/src/base/pb/protocol/BaseDefine.pb.cc
${PROJECT_SOURCE_DIR}/src/base/pb/protocol/User.pb.cc
)
target_link_libraries(register_proto protobuf pthread)

add_executable(register_server main.cpp ${SRC_LIST} ${BASE_SRCS})
target_link_libraries(register_server register_proto pthread muduo_base muduo_net)