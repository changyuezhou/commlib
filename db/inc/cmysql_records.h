// Copyright (c) 2013 zhou chang yue. All rights reserved.

#ifndef COMMLIB_DB_INC_CMYSQL_RECORDS_H_
#define COMMLIB_DB_INC_CMYSQL_RECORDS_H_

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
#include "commlib/db/inc/crecords.h"
#include "commlib/db/inc/log.h"
#include "commlib/magic/inc/timeFormat.h"

namespace lib {
  namespace db {
    using std::string;
    using std::vector;  

    class CMYSQL_RECORDS: public CRECORDS {    
     public:
       CMYSQL_RECORDS(MYSQL_RES * result): result_(result) {}
       virtual ~CMYSQL_RECORDS () { Destroy(); }

     private:
       CMYSQL_RECORDS(const CMYSQL_RECORDS &);
       CMYSQL_RECORDS & operator=(const CMYSQL_RECORDS &);

     public:
       virtual ROW  operator [] (INT32 row) const {
         if (0 >= RecordSize() || 0 > row || row >= RecordSize()) {
           return NULL;
         }

         ::mysql_data_seek(result_, row);

         return ::mysql_fetch_row(result_);         
       }
       virtual ROW First() const;
       virtual ROW Next() const ;

     public:
       virtual INT32 RecordSize() const;

     protected:
       virtual VOID Destroy();

     private:
       MYSQL_RES * result_;
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_CMYSQL_RECORDS_H_