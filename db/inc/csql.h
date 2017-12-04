// Copyright (c) 2013 zhou chang yue. All rights reserved.

#ifndef COMMLIB_DB_INC_CSQL_H_
#define COMMLIB_DB_INC_CSQL_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "commlib/public/inc/type.h"
#include "commlib/db/inc/err.h"
#include "commlib/magic/inc/timeFormat.h"

namespace lib {
  namespace db {
    using std::string;
    
    class CSQL {
     public:
       enum OP {
         INSERT = 1,
         UPDATE = 2,
         SELECT = 3,
         DELETE = 4,
         OTHERS = 99
       };
       
     public:
       CSQL() {}
       explicit CSQL(const string & sql): sql_(sql) {}
       virtual ~CSQL () {}
       
     public:
       INT32 ParseSQL(const string & sql);  
      
     public:
       const string & SQL() const { return sql_; }
       const string & TableName() { return table_name_; }
       
     public:
       BOOL IsConditionHasColumns(const string & columns) { return FALSE; }
       BOOL IsUpdateHasColumns(const string & columns) { return FALSE; }
       
     private:
       CSQL(const CSQL &);
       CSQL & operator=(const CSQL &);
       
     private:
       const string sql_;
       const string table_name_;
       const string select_columns_;
       const string conditions_columns_;
       const string update_columns_;
       const string pages_;
       OP op_;
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_CSQL_H_