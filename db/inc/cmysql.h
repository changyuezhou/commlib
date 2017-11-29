// Copyright (c) 2013 zhou chang yue. All rights reserved.

#ifndef COMMLIB_DB_INC_CMYSQL_H_
#define COMMLIB_DB_INC_CMYSQL_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <mysql.h>
#include "commlib/public/inc/type.h"
#include "commlib/db/inc/err.h"
#include "commlib/db/inc/dba.h"
#include "commlib/db/inc/csql.h"

namespace lib {
  namespace db {
    using std::string;
    using std::map;
    using std::vector;

    class CMySQL: public DBA {
     public:
       CMySQL(const string & host, INT32 port,
           const string & db, const string & user,
           const string & pwd): DBA(host, port, db, user, pwd) {}
       CMySQL(): DBA() {}
       virtual ~CMySQL() { SignOut(); }

     public:
       virtual INT32 SignIn(const string & host, INT32 port, const string & db, const string & user, const string & pwd);
       virtual INT32 SignIn();

       virtual INT32 Execute(const CSQL & sql, UINT32 affect = 0);
       virtual INT32 Query(CRECORDS & records, const CSQL & sql);

       virtual INT32 SignOut();

       virtual BOOL IsConnecting();
       virtual DBA::DB_TYPE Type() { return DBA::MYSQL_TYPE; }
       virtual const string GetErrMsg();
       virtual VOID SetErrMsg(const string & err_msg) { err_msg_ = err_msg; }

     protected:
       INT32 reconnect();

     private:
       CMySQL(const CMySQL &);
       CMySQL & operator=(const CMySQL &);

     private:
       MYSQL connection_;
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_CMYSQL_H_