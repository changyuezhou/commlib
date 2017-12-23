// Copyright (c) 2013 zhou chang yue. All rights reserved.

#ifndef COMMLIB_DB_INC_ERR_H_
#define COMMLIB_DB_INC_ERR_H_

#include <errno.h>
#include <string>
#include <cstdio>
#include "commlib/public/inc/type.h"

namespace lib {
  namespace db {
    using std::string;

    class Err {
     public:
       // pool errors
       static const INT32 kERR_DB_SIGN_IN_FAILED = -100350;
       static const INT32 kERR_DB_QUERY_FAILED = -100351;
       static const INT32 kERR_DB_EXECUTE_FAILED = -100352;
       static const INT32 kERR_DB_DATABASE_TYPE_NOT_SUPPORT = -100353;
       static const INT32 kERR_DB_DATABASE_SPLIT_NOT_SUPPORT_EXECUTE_SQL = -100354;
       static const INT32 kERR_DB_DATABASE_CACHE_NOT_SUPPORT_EXECUTE_SQL = -100355;

       static const INT32 kERR_DB_MYSQL_INIT_FAILED = -100360;
       static const INT32 kERR_DB_MYSQL_SQL_INVALID = -100361;
       static const INT32 kERR_DB_MYSQL_HAS_NO_RECORD = -100362;
       static const INT32 kERR_DB_MYSQL_AFFECT_RECORDS = -100363;

       static const INT32 kERR_DB_REDIS_CONNECTED_FAILED = -100370;
       static const INT32 kERR_DB_REDIS_AUTH_FAILED = -100371;
       static const INT32 kERR_DB_REDIS_SADD_FAILED = -100372;

       static const INT32 kERR_DB_TABLE_DBA_OBJECT_EMPTY = -100380;
       static const INT32 kERR_DB_TABLE_DBA_DISCONNECTING = -100381;
       static const INT32 kERR_DB_TABLE_DBA_QUERY_FAILED = -100382;
       static const INT32 kERR_DB_TABLE_DBA_TABLE_OR_DB_NAME_INVALID = -100383;
       static const INT32 kERR_DB_TABLE_DBA_TABLE_NO_COLUMN = -100384;
       static const INT32 kERR_DB_TABLE_DBA_INSERT_FAILED = -100385;
       static const INT32 kERR_DB_TABLE_DBA_TABLE_NO_RECORDS = -100386;
       static const INT32 kERR_DB_TABLE_DBA_UPDATE_FAILED = -100387;
       static const INT32 kERR_DB_TABLE_DBA_DATABASE_ALLOCATE_FAILED = -100388;
       static const INT32 kERR_DB_TABLE_DBA_DATABASE_SIGN_IN_FAILED = -100389;
       static const INT32 kERR_DB_TABLE_DBA_DB_NAME_INVALID = -100390;
       static const INT32 kERR_DB_TABLE_DBA_DB_DESC_OBJECT_INVALID = -100391;
       static const INT32 kERR_DB_TABLE_DBA_DB_LOAD_DESC_FAILED = -100392;
       static const INT32 kERR_DB_TABLE_DBA_DB_INSERT_TABLE_NOT_FOUND = -100393;
       static const INT32 kERR_DB_TABLE_DBA_EXECUTE_FAILED = -100394;
       static const INT32 kERR_DB_TABLE_DBA_INSERT_HAS_NO_RECORDS = -100395;
       static const INT32 kERR_DB_TABLE_DBA_UPDATE_FIELDS_INCLUDE_HASH_COLUMN = -100396;
       static const INT32 kERR_DB_TABLE_DBA_DELETE_FAILED = -100397;
       static const INT32 kERR_DB_TABLE_DBA_CHECK_COLUMN_COUNT_NOT_EQ = -100398;
       static const INT32 kERR_DB_TABLE_DBA_CHECK_COLUMN_PRIMARY_KEY_CHANGED = -100399;
       static const INT32 kERR_DB_TABLE_DBA_INDEX_TOO_MANY = -100400;
       static const INT32 kERR_DB_TABLE_DBA_INDEX_IS_NOT_IN_DESC = -100401;
       static const INT32 kERR_DB_TABLE_DBA_INDEX_KEY_COUNT_IS_NOT_EQ = -100402;
       static const INT32 kERR_DB_TABLE_DBA_INDEX_COLUMN_IS_NOT_EQ = -100402;
       static const INT32 kERR_DB_TABLE_DBA_LOAD_KEYS_FAILED = -100403;
       static const INT32 kERR_DB_TABLE_DBA_ENCODER_RECORD_LENGTH_TOO_MANY = -100404;
       static const INT32 kERR_DB_TABLE_DBA_MUST_PRELOAD_DATA = -100405;
       static const INT32 kERR_DB_TABLE_DBA_RECORD_IS_INVALID = -100406;
       static const INT32 kERR_DB_TABLE_DBA_COLUMN_IS_AUTO_INC = -100407;
       static const INT32 kERR_DB_TABLE_DBA_COLUMN_IS_NOT_CACHE_ABLE = -100408;

       static const INT32 kERR_DB_TABLE_DBA_CACHE_UNIQUE_KEY_EXISTS = -100400;
       static const INT32 kERR_DB_TABLE_DBA_UNIQUE_KEY_GET_KEY_FROM_RECORD_FAILED = -100401;

       static const INT32 kERR_DB_DATABASE_CREATE_SHARED_MEMORY_FAILED = -100402;
       static const INT32 kERR_DB_DATABASE_INITIAL_LOCK_FAILED = -100403;
       static const INT32 kERR_DB_DATABASE_TABLES_IN_MEMORY_INVALID = -100404;
       static const INT32 kERR_DB_DATABASE_HAS_NO_TABLE = -100405;
       static const INT32 kERR_DB_DATABASE_DATA_IS_NOT_READY = -100406;
       static const INT32 kERR_DB_DATABASE_TABLES_IS_TOO_MANY = -100407;
       static const INT32 kERR_DB_DATABASE_SHARED_MEMORY_IS_EXISTS = -100408;
       static const INT32 kERR_DB_DATABASE_DO_NOT_NEED_CACHE = -100409;
       static const INT32 kERR_DB_DATABASE_SHARED_MEMORY_NOT_EXISTS = -100410;
       static const INT32 kERR_DB_DATABASE_TABLE_NODE_MEM_NOT_USABLE = -100411;
       static const INT32 kERR_DB_DATABASE_SHARED_MEMORY_CACHE_IS_EXPIRED = -100412;

       static const INT32 kERR_DB_DATABASE_TABLES_INDEX_GET_KEY_FROM_RECORD_FAILED = -100420;
       static const INT32 kERR_DB_DATABASE_TABLES_INDEX_GET_KEY_CONDITION_INVALID = -100421;
       static const INT32 kERR_DB_DATABASE_SPLIT_TABLES_NUMBER_INVALID = -100422;


       static const INT32 kERR_DB_SQL_INSERT_HAS_NO_REQUIRED_KEY_WORDS = -100430;
       static const INT32 kERR_DB_SQL_INSERT_HAS_NO_TABLE_NAME = -100431;
       static const INT32 kERR_DB_SQL_INSERT_HAS_NO_RIGHT_BRACKET = -100432;
       static const INT32 kERR_DB_SQL_INSERT_HAS_NO_LEFT_BRACKET = -100433;
       static const INT32 kERR_DB_SQL_INSERT_HAS_NO_VALUE = -100434;
       static const INT32 kERR_DB_SQL_INSERT_VALUES_BRACKET_INVALID = -100435;

       static const INT32 kERR_DB_SQL_UPDATE_HAS_NO_REQUIRED_KEY_WORDS = -100440;
       static const INT32 kERR_DB_SQL_UPDATE_COLUMN_VALUE_INVALID = -100441;

       static const INT32 kERR_DB_SQL_SELECT_HAS_NO_REQUIRED_KEY_WORDS = -100450;

       static const INT32 kERR_DB_SQL_CONDITION_DO_BETWEEN_INVALID = -100460;
       static const INT32 kERR_DB_SQL_CONDITION_DO_ITEM_INVALID = -100461;
       static const INT32 kERR_DB_SQL_CONDITION_INVALID = -100462;
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_ERR_H_
