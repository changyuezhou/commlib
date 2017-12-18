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

    typedef MYSQL_ROW ROW;

    class CRECORDS {    
     public:
       CRECORDS() {}
       virtual ~CRECORDS () {}

     private:
       CRECORDS(const CRECORDS &);
       CRECORDS & operator=(const CRECORDS &);

     public:
       virtual ROW  operator [] (INT32 row) const = 0;
       virtual ROW First() const = 0;
       virtual ROW Next() const = 0;

     public:
       virtual INT32 RecordSize() const = 0;

     protected:
       virtual VOID Destroy() = 0;
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_CRECORDS_H_