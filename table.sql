# user table
DROP TABLE IF EXISTS `t_user`;

CREATE TABLE t_user (
		 f_user_id  bigint(64) NOT NULL AUTO_INCREMENT COMMENT '用户ID',
		 f_username  varchar(64) NOT NULL COMMENT '用户名',
         f_password  varchar(64) NOT NULL COMMENT '用户密码',
		 f_signature  varchar(256) DEFAULT NULL COMMENT '个性签名',
		 f_owner_id  bigint(64) DEFAULT 0 COMMENT '群账号群主userid',
         PRIMARY KEY ( f_user_id )
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4