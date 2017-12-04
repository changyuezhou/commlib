// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include <string>
#include <vector>
#include "commlib/public/inc/type.h"
#include "commlib/db/inc/cmysql.h"
#include "commlib/db/inc/crecords.h"
#include "commlib/magic/inc/timeFormat.h"
#include "commlib/magic/inc/str.h"
#include "commlib/log/inc/handleManager.h"

using lib::db::CMySQL;
using lib::db::CSQL;
using lib::db::CRECORDS;
using lib::magic::TimeFormat;
using lib::magic::String;
using lib::log::HandleManager;

using std::string;
using std::vector;

INT32 main(INT32 argc, CHAR ** argv) {
  if (2 != argc) {
    printf("Usage:[config file]\r\n");
    return 0;
  }
/*
  INT32 result = HandleManager::GetInstance()->Initial(argv[1]);
  if (0 != result) {
    printf("initial log file failed\r\n");
    return -1;
  }
  */
  CMySQL mysql("127.0.0.1", 3306, "db_auth", "root", "1qaz2wsx@2017");
  if (0 != mysql.SignIn()) {
    printf("mysql sign in failed\r\n");
    return -1;
  }

  CSQL sql("select * from t_system");
  CRECORDS records;
  mysql.Query(records, sql);

  //delete HandleManager::GetInstance();

  return 0;
}
