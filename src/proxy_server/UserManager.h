#pragma once

#include <string>
#include <list>

#include "BaseDefine.pb.h"

#include "Singleton.h"
#include "MysqlManager.h"
#include "DatabaseConn.h"
#include "RedisManager.h"
#include "RedisConn.h"

class UserManager final
{
public:
    UserManager() = delete;
    ~UserManager() = delete;

    UserManager(const UserManager& rhs) = delete;
    UserManager& operator=(const UserManager& rhs) = delete;

    static bool addUser(BaseDefine::UserInfo& u);
    static bool makeFriendRelationshipInDB(uint32_t smallUserid, uint32_t greaterUserid);
    static bool releaseFriendRelationshipInDBAndMemory(uint32_t smallUserid, uint32_t greaterUserid);
    static bool updateUserRelationshipInMemory(uint32_t userid, uint32_t target, BaseDefine::UserOperation operation);
    static bool addFriendToUser(uint32_t userid, uint32_t friendid);
    static bool deleteFriendToUser(uint32_t userid, uint32_t friendid);

    static bool isFriend(uint32_t userid, uint32_t friendid);
    
    static bool updateUserInfoInDb(uint32_t userid, const BaseDefine::UserInfo& newuserinfo);
    static bool modifyUserPassword(uint32_t userid, const std::string& newpassword);
    static bool updateUserTeamInfoInDbAndMemory(uint32_t userid, const std::string& newteaminfo);
    static bool deleteTeam(uint32_t userid, const std::string& deletedteamname);
    static bool modifyTeamName(uint32_t userid, const std::string& newteamname, const std::string& oldteamname);

    static bool updateMarknameInDb(uint32_t userid, uint32_t friendid, const std::string& newmarkname);
    static bool moveFriendToOtherTeam(uint32_t userid, uint32_t friendid, const std::string& newteamname);

    static bool addGroup(const char* groupname, uint32_t ownerid, uint32_t& groupid);

    static bool saveChatMsgToDb(uint32_t senderid, uint32_t targetid, const std::string& chatmsg);

    static bool getUserInfoByUsername(const std::string& username, BaseDefine::UserInfo& u);
    static bool getUserInfoByUserId(uint32_t userid, BaseDefine::UserInfo& u);
    static bool getUserInfoByUserId(uint32_t userid, BaseDefine::UserInfo*& u);
    static bool getFriendInfoByUserId(uint32_t userid, std::list<BaseDefine::UserInfo>& friends);

    static bool getFriendMarknameByUserId(uint32_t userid1, uint32_t friendid, std::string& markname);
    static bool getTeamInfoByUserId(uint32_t userid, std::string& teaminfo);

    static BaseDefine::ResultType verifyUser(uint32_t user_id, std::string password, BaseDefine::UserInfo* user_info, std::string& retData);
};