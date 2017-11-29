// Copyright (c) 2013 zhou chang yue. All rights reserved.

#ifndef COMMLIB_DB_INC_DBA_H_
#define COMMLIB_DB_INC_DBA_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "commlib/public/inc/type.h"
#include "commlib/db/inc/err.h"
#include "commlib/db/inc/csql.h"
#include "commlib/db/inc/crecords.h"
#include "commlib/magic/inc/timeFormat.h"

namespace lib {
  namespace db {
    using std::string;
    using std::map;
    using std::make_pair;
    using std::vector;
    using lib::magic::TimeFormat;

    class DBA {
     public:
       enum DB_TYPE {
         MYSQL_TYPE = 1,
         ORACLE = 2,
         CACHE = 3,
         CLIENT = 4
       };

     public:
       DBA(const string & host, INT32 port,
           const string & db, const string & user,
           const string & pwd): host_(host), port_(port),
            db_(db), user_(user), pwd_(pwd), err_msg_("") {}
       DBA(): host_(""), port_(0), db_(""), user_(""), pwd_(""), err_msg_("") {}
       virtual ~DBA() {}

     public:
       virtual INT32 SignIn(const string & host, INT32 port, const string & db, const string & user, const string & pwd);
       virtual INT32 SignIn() = 0;

       virtual INT32 Execute(const CSQL & sql, UINT32 affect = 0) = 0;
       virtual INT32 Query(CRECORDS & records, const CSQL & sql) = 0;

       virtual INT32 SignOut() = 0;
       virtual BOOL IsConnecting() = 0;
       virtual DB_TYPE Type() = 0;

       virtual const string GetErrMsg() = 0;
       virtual VOID SetErrMsg(const string & err_msg) = 0;

     private:
       DBA(const DBA &);
       DBA & operator=(const DBA &);

     protected:
       string host_;
       INT32 port_;
       string db_;
       string user_;
       string pwd_;
       string err_msg_;
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_DBA_H_