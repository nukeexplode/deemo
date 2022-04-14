include_directories(${PROJECT_SOURCE_DIR}/src/base)
include_directories(${PROJECT_SOURCE_DIR}/src/base/database)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

set(base_src
    ../ConfigFileReader.cpp
)

foreach(file ${base_src})
  message("file='${file}'")
endforeach()

aux_source_directory(../database database_src)

foreach(file ${database_src})
    message("file='${file}'")
endforeach()

add_executable(singleton_test Singleton_test.cpp ${base_src})
target_link_libraries(singleton_test pthread)

add_executable(ConfigFileReader_test ConfigFileReader_test.cpp ${base_src})
target_link_libraries(ConfigFileReader_test)

add_executable(DatabasePool_test DatabasePool_test.cpp ${database_src} ${base_src})
target_link_libraries(DatabasePool_test mysqlclient muduo_base pthread)