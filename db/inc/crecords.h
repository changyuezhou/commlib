// Copyright (c) 2013 zhou chang yue. All rights reserved.

#ifndef COMMLIB_DB_INC_CRECORDS_H_
#define COMMLIB_DB_INC_CRECORDS_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "commlib/public/inc/type.h"
#include "commlib/db/inc/err.h"
#include "commlib/db/inc/log.h"
#include "commlib/magic/inc/timeFormat.h"

namespace lib {
  namespace db {
    using std::string;
    using std::vector;  

    class CRECORDS {
     class CRECORD {
      public:
        CRECORD(MYSQL_ROW row, MYSQL_FIELD * fields, INT32 fields_num): row_(row), fields_(fields), fields_num_(fields_num) {}
        CRECORD(const CRECORD & r): row_(r.row_), fields_(r.fields_), fields_num_(r.fields_num_) {}
        virtual ~CRECORD() {}

      public:
        BOOL IsEmpty() const { return 0 >= fields_num_; }  

      public:
        const string operator [] (INT32 index) const {
          if (IsEmpty()) {
            return "";
          }

          return row_[index]; 
        }
        const string operator [] (const string & column) const {
          if (IsEmpty()) {
            return "";
          }

          INT32 index = GetFieldIndex(column);
          if (-1 == index) {
            return "";
          }

          return row_[index];
        }

      protected:
        INT32 GetFieldIndex(const string & column) const {
          for (INT32 i = 0; i < fields_num_; ++i) {
            if ((::strlen(fields_[i].name) == column.length()) && (0 == ::strncasecmp(fields_[i].name, column.c_str(), column.length()))) {
              return i;
            }
          }

          return -1;
        }

      public:
        const CRECORD & operator=(const CRECORD & r) {
          this->row_ = r.row_;
          this->fields_ = r.fields_;
          this->fields_num_ = r.fields_num_;

          return *this;
        }

      public:
        VOID Dump() const {
          LIB_DB_LOG_DEBUG("Fields:");
          for (INT32 i = 0; i < fields_num_; ++i) {
            LIB_DB_LOG_DEBUG("Field name:" << fields_[i].name);
            LIB_DB_LOG_DEBUG("Field value:" << (*row_)[i]);
          }

          LIB_DB_LOG_DEBUG("ROW:" << row_);
        }

      private:
        MYSQL_ROW row_;
        MYSQL_FIELD * fields_;
        INT32 fields_num_;
     };       
     public:
       CRECORDS() {}
       virtual ~CRECORDS () { Destroy(); }

     private:
       CRECORDS(const CRECORDS &);
       CRECORDS & operator=(const CRECORDS &);

     public:
        typedef vector<MYSQL_RES *> RESULT_LIST;
        typedef vector<CRECORD> RECORDS_LIST;

     public:
       INT32 AddResult(MYSQL_RES * result);

     public:
       const CRECORD operator [] (INT32 index) {
         INT32 records_size = records_list_.size();
         if (0 >= records_size || 0 > index || index >= records_size) {
           return CRECORD(NULL, NULL, 0);
         }

         return records_list_[index];
       }

     public:
       INT32 RecordsSize() { return records_list_.size(); }

     private:
       VOID Destroy();
       
     private:
        RESULT_LIST result_list_;
        RECORDS_LIST records_list_;
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_CRECORDS_H_