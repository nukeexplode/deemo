# Details

Date : 2022-05-06 19:17:11

Directory /home/nukeexplode/Desktop/deemo/src

Total : 50 files,  5069 codes, 699 comments, 1079 blanks, all 6847 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [src/base/ConfigFileReader.cpp](/src/base/ConfigFileReader.cpp) | C++ | 81 | 48 | 24 | 153 |
| [src/base/ConfigFileReader.h](/src/base/ConfigFileReader.h) | C++ | 19 | 0 | 4 | 23 |
| [src/base/Singleton.h](/src/base/Singleton.h) | C++ | 20 | 38 | 18 | 76 |
| [src/base/TcpSession.cpp](/src/base/TcpSession.cpp) | C++ | 14 | 75 | 21 | 110 |
| [src/base/TcpSession.h](/src/base/TcpSession.h) | C++ | 33 | 7 | 12 | 52 |
| [src/base/codec.cpp](/src/base/codec.cpp) | C++ | 165 | 29 | 25 | 219 |
| [src/base/codec.h](/src/base/codec.h) | C++ | 57 | 12 | 16 | 85 |
| [src/base/database/DatabaseConn.cpp](/src/base/database/DatabaseConn.cpp) | C++ | 66 | 0 | 13 | 79 |
| [src/base/database/DatabaseConn.h](/src/base/database/DatabaseConn.h) | C++ | 22 | 5 | 6 | 33 |
| [src/base/database/DatabaseMysql.cpp](/src/base/database/DatabaseMysql.cpp) | C++ | 56 | 0 | 11 | 67 |
| [src/base/database/DatabaseMysql.h](/src/base/database/DatabaseMysql.h) | C++ | 37 | 1 | 8 | 46 |
| [src/base/database/Field.cpp](/src/base/database/Field.cpp) | C++ | 23 | 1 | 7 | 31 |
| [src/base/database/Field.h](/src/base/database/Field.h) | C++ | 71 | 0 | 15 | 86 |
| [src/base/database/MysqlManager.cpp](/src/base/database/MysqlManager.cpp) | C++ | 40 | 0 | 12 | 52 |
| [src/base/database/MysqlManager.h](/src/base/database/MysqlManager.h) | C++ | 20 | 0 | 5 | 25 |
| [src/base/database/QueryResult.cpp](/src/base/database/QueryResult.cpp) | C++ | 79 | 1 | 13 | 93 |
| [src/base/database/QueryResult.h](/src/base/database/QueryResult.h) | C++ | 47 | 0 | 14 | 61 |
| [src/base/dispatcher.h](/src/base/dispatcher.h) | C++ | 74 | 0 | 16 | 90 |
| [src/base/google-inl.h](/src/base/google-inl.h) | C++ | 27 | 49 | 9 | 85 |
| [src/base/msg.h](/src/base/msg.h) | C++ | 16 | 0 | 1 | 17 |
| [src/base/pb/protocol/BaseDefine.pb.cc](/src/base/pb/protocol/BaseDefine.pb.cc) | C++ | 159 | 7 | 17 | 183 |
| [src/base/pb/protocol/BaseDefine.pb.h](/src/base/pb/protocol/BaseDefine.pb.h) | C++ | 175 | 9 | 24 | 208 |
| [src/base/pb/protocol/User.pb.cc](/src/base/pb/protocol/User.pb.cc) | C++ | 1,479 | 167 | 245 | 1,891 |
| [src/base/pb/protocol/User.pb.h](/src/base/pb/protocol/User.pb.h) | C++ | 871 | 130 | 199 | 1,200 |
| [src/base/redis/RedisConn.cpp](/src/base/redis/RedisConn.cpp) | C++ | 478 | 5 | 82 | 565 |
| [src/base/redis/RedisConn.h](/src/base/redis/RedisConn.h) | C++ | 41 | 5 | 11 | 57 |
| [src/base/redis/RedisManager.cpp](/src/base/redis/RedisManager.cpp) | C++ | 39 | 0 | 12 | 51 |
| [src/base/redis/RedisManager.h](/src/base/redis/RedisManager.h) | C++ | 17 | 0 | 4 | 21 |
| [src/base/redis/RedisPool.cpp](/src/base/redis/RedisPool.cpp) | C++ | 51 | 0 | 11 | 62 |
| [src/base/redis/RedisPool.h](/src/base/redis/RedisPool.h) | C++ | 28 | 3 | 5 | 36 |
| [src/base/tests/ConfigFileReader_test.cpp](/src/base/tests/ConfigFileReader_test.cpp) | C++ | 10 | 0 | 3 | 13 |
| [src/base/tests/DatabasePool_test.cpp](/src/base/tests/DatabasePool_test.cpp) | C++ | 41 | 0 | 10 | 51 |
| [src/base/tests/Singleton_test.cpp](/src/base/tests/Singleton_test.cpp) | C++ | 28 | 0 | 8 | 36 |
| [src/client/RegClient.cpp](/src/client/RegClient.cpp) | C++ | 12 | 0 | 2 | 14 |
| [src/client/RegClient.h](/src/client/RegClient.h) | C++ | 88 | 8 | 17 | 113 |
| [src/client/main.cpp](/src/client/main.cpp) | C++ | 30 | 27 | 12 | 69 |
| [src/proxy_server/ProxyServer.cpp](/src/proxy_server/ProxyServer.cpp) | C++ | 27 | 0 | 9 | 36 |
| [src/proxy_server/ProxyServer.h](/src/proxy_server/ProxyServer.h) | C++ | 28 | 0 | 8 | 36 |
| [src/proxy_server/ProxySession.cpp](/src/proxy_server/ProxySession.cpp) | C++ | 65 | 1 | 14 | 80 |
| [src/proxy_server/ProxySession.h](/src/proxy_server/ProxySession.h) | C++ | 39 | 3 | 12 | 54 |
| [src/proxy_server/main.cpp](/src/proxy_server/main.cpp) | C++ | 16 | 0 | 8 | 24 |
| [src/register_server/ProxyClient.cpp](/src/register_server/ProxyClient.cpp) | C++ | 14 | 0 | 3 | 17 |
| [src/register_server/ProxyClient.h](/src/register_server/ProxyClient.h) | C++ | 96 | 5 | 18 | 119 |
| [src/register_server/ProxyClientManager.cpp](/src/register_server/ProxyClientManager.cpp) | C++ | 41 | 0 | 13 | 54 |
| [src/register_server/ProxyClientManager.h](/src/register_server/ProxyClientManager.h) | C++ | 28 | 1 | 8 | 37 |
| [src/register_server/RegServer.cpp](/src/register_server/RegServer.cpp) | C++ | 38 | 3 | 14 | 55 |
| [src/register_server/RegServer.h](/src/register_server/RegServer.h) | C++ | 41 | 1 | 11 | 53 |
| [src/register_server/RegSession.cpp](/src/register_server/RegSession.cpp) | C++ | 54 | 53 | 23 | 130 |
| [src/register_server/RegSession.h](/src/register_server/RegSession.h) | C++ | 52 | 5 | 17 | 74 |
| [src/register_server/main.cpp](/src/register_server/main.cpp) | C++ | 16 | 0 | 9 | 25 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)