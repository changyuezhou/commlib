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
#include <algorithm>
#include "commlib/public/inc/type.h"
#include "commlib/db/inc/err.h"
#include "commlib/magic/inc/timeFormat.h"

namespace lib {
  namespace db {
    using std::string;
    using std::vector;
    using std::map;
    using std::transform;
    using std::find;
    using std::make_pair;
    
    class CSQL {
     public:
       enum OP {
         INSERT = 1,                         // 创建
         UPDATE = 2,                         // 更新
         DELETE = 3,                         // 删除
         SELECT = 4,                         // 查询
         TRANSACTION = 5,                    // 事务操作
         OTHERS = 99                         // 其他
       };

       enum CONDITION_OP
       {
          EQ = 1,
          NE = 2,
          LT = 3,
          ELT = 4,
          GT = 5,
          EGT = 6,
          RANGE = 7,
          COND_UNKNOWN = 99
       };

     public:
       typedef vector<string> SQL_SPLIT_ARRAY;
       typedef vector<string> COLUMN_LIST;
       typedef vector<vector<string>> COLUMN_VALUES;
       typedef vector<string> COLUMN_VALUE;
       typedef vector<string> COLUMN_GROUP_KEY;

       typedef struct _CONDITION_ITEM {
         CONDITION_OP condition_op_;
         string value_;
       } CONDITION_ITEM;

       typedef vector<CONDITION_ITEM> CONDITION_ITEM_LIST;
       typedef map<string, CONDITION_ITEM_LIST> CONDITIONS_LIST;

     public:
       CSQL(): sql_(""), op_(OTHERS), is_access_cache_(TRUE) {}
       explicit CSQL(const string & sql): sql_(sql), op_(OTHERS), is_access_cache_(TRUE) {}
       virtual ~CSQL () {}
       
     public:
       INT32 ParseSQL();
       INT32 ParseSQL(const string & sql); 
      
     public:
       const string & SQL() const { return sql_; }
       OP GetOP() const { return op_; }
       const string & ConditionsStr() const { return conditions_str_; }
       const string & TableName() const { return table_name_; }
       BOOL IsAccessCache() const { return is_access_cache_; }

     public:
       const COLUMN_VALUES & GetInsertValues() const { return insert_values_; }
       const COLUMN_LIST & GetInsertColumns() const { return insert_columns_; }
       const COLUMN_LIST & GetUpdateColumns() const { return update_columns_; }
       const COLUMN_VALUE & GetUpdateValues() const { return update_values_; }

       const COLUMN_LIST & GetSelectColumns() const { return select_columns_; }

       const COLUMN_LIST GetConditionsColumns() const;
       BOOL GetConditionsItems(const string & column, CONDITION_ITEM_LIST & item_list) const;

     public:
       BOOL IsConditionHasColumns(const string & column) const;
       BOOL IsUpdateHasColumns(const string & column) const;
       BOOL IsConditionsOPEQ(const string & column) const;

     public:  
       INT32 ConditionsColumnCount() const { return conditions_columns_.size(); }

     public:
       VOID Dump();  

     protected:
       INT32 ParseInsert(const string & sql);
       INT32 ParseUpdate(const string & sql);
       INT32 ParseDelete(const string & sql);
       INT32 ParseSelect(const string & sql);

     protected:
       const string FindTableName(const string & sql, const string & begin_key, const string & end_key);

     protected:  
       INT32 FindInsertColumn(const string & sql);
       INT32 FindColumnValues(const string & sql);

     protected:
       INT32 FindUpdateColumnAndValue(const string & sql);

     protected:
       const string FindSelectColumns(const string & sql);

     protected:
       INT32 FindConditions(const string & sql, const string & end_key, BOOL need_parsing = FALSE);
       INT32 FindConditionsItems(const string & conditions);
       INT32 DoBetween(const string & conditions);
       INT32 DoOthers(const string & conditions);
       INT32 DoConditionItems(const string & item, const string & key);
       INT32 FetchConditionColumnValue(string & item, string & value);
       BOOL CheckBracketIsAfterIN(const string & conditions);
       
     private:
       CSQL(const CSQL &);
       CSQL & operator=(const CSQL &);
       
     protected:
       string sql_;
       string db_name_;
       string table_name_;
       string conditions_str_;
       COLUMN_LIST select_columns_;
       CONDITIONS_LIST conditions_columns_;
       COLUMN_LIST update_columns_;
       COLUMN_VALUE update_values_;
       COLUMN_LIST insert_columns_;
       COLUMN_VALUES insert_values_;
       string pages_;
       OP op_;
       BOOL is_access_cache_;
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_CSQL_H_