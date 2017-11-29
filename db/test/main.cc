// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include <string>
#include <vector>
#include "commlib/public/inc/type.h"
#include "commlib/db/inc/cmysql.h"
#include "commlib/db/inc/database.h"
#include "commlib/db/inc/config.h"
#include "commlib/magic/inc/timeFormat.h"
#include "commlib/magic/inc/str.h"
#include "commlib/log/inc/handleManager.h"

using lib::db::CMySQL;
using lib::db::DATABASE;
using lib::db::SQL;
using lib::magic::TimeFormat;
using lib::magic::String;
using lib::db::Config;
using lib::log::HandleManager;

using std::string;
using std::vector;

INT32 main(INT32 argc, CHAR ** argv) {
  if (2 != argc) {
    printf("Usage:[config file]\r\n");
    return 0;
  }

  INT32 result = HandleManager::GetInstance()->Initial(argv[1]);
  if (0 != result) {
    printf("initial log file failed\r\n");
    return -1;
  }

  Config config;
  result = config.Loading(argv[1], "db");
  if (0 != result) {
    LIB_DB_LOG_ERROR("Loading database to memory initial config failed result:" << result);
    return result;
  }

  DATABASE * db = new DATABASE();
  if (NULL == db) {
    LIB_DB_LOG_ERROR("Loading database to memory allocated database object failed");
    return -1;
  }

  result = db->Initial(config.GetDBConfig());
  if (0 != result) {
    LIB_DB_LOG_ERROR("Loading database to memory initial failed result:" << result);
    delete db;
    delete HandleManager::GetInstance();

    return result;
  }


  SQL::Condition::Items items;
  SQL::Condition::Items::Item item_db;
  item_db.column_ = "id";
  item_db.value_ = "1";
  item_db.op_ = SQL::Condition::Items::Item::EQ;
  items.item_list_.push_back(item_db);

  SQL::Condition condition;
  condition.items_list_.push_back(items);

  SQL::ColumnList column_list;
  column_list.push_back("*");

  DATABASE::RECORDS records;
  records.clear();

  INT64 start = TimeFormat::GetCurTimestampLong();
  for (INT32 i = 0; i < 1; ++i) {
    result = db->Query(records, "t_session", &column_list, &condition);
  }
  INT64 end = TimeFormat::GetCurTimestampLong();
  printf("query cost time:%ld\r\n", (end - start));
  if (0 != result) {
    printf("query error code:%d\r\n", result);

    return -1;
  }

  for (UINT32 i = 0; i < records.size(); ++i) {
    DATABASE::RECORD & r = records[i];
    printf("*********************************************************************\n");
    for (DATABASE::RECORD::const_iterator it = r.cbegin(); it != r.cend(); ++it) {
      printf("%s:%s\n", it->first.c_str(), it->second.c_str());
    }
  }

  condition.items_list_.clear();
  items.item_list_.clear();
  records.clear();

  item_db.column_ = "system_id";
  item_db.value_ = "1";
  item_db.op_ = SQL::Condition::Items::Item::EQ;
  items.item_list_.push_back(item_db);

  condition.items_list_.push_back(items);

  start = TimeFormat::GetCurTimestampLong();
  for (INT32 i = 0; i < 1; ++i) {
    result = db->Query(records, "t_system", &column_list, &condition);
  }
  end = TimeFormat::GetCurTimestampLong();
  printf("query cost time:%ld\r\n", (end - start));
  if (0 != result) {
    printf("query error code:%d\r\n", result);

    return -1;
  }

  for (UINT32 i = 0; i < records.size(); ++i) {
    DATABASE::RECORD & r = records[i];
    printf("*********************************************************************\n");
    for (DATABASE::RECORD::const_iterator it = r.cbegin(); it != r.cend(); ++it) {
      printf("%s:%s\n", it->first.c_str(), it->second.c_str());
    }
  }

  delete db;
  delete HandleManager::GetInstance();

  return 0;
}