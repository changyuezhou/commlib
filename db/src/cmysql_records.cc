// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include "commlib/db/inc/cmysql_records.h"
#include "commlib/db/inc/log.h"
#include "commlib/db/inc/err.h"

namespace lib {
  namespace db {
    INT32 CMYSQL_RECORDS::RecordSize() const {
      if (NULL == result_) {
        return Err::kERR_DB_MYSQL_HAS_NO_RECORD;
      }

      return ::mysql_num_rows(result_);
    }

    ROW CMYSQL_RECORDS::First() const {
      if (0 >= RecordSize()) {
        return NULL;
      }

      ::mysql_data_seek(result_, 0);

      return ::mysql_fetch_row(result_);
    }

    ROW CMYSQL_RECORDS::Next() const {
      if (0 >= RecordSize()) {
        return NULL;
      }

      return ::mysql_fetch_row(result_);
    }    
    /*
    INT32 CRECORDS::AddResult(MYSQL_RES * result) {
      if (NULL == result || 0 >= ::mysql_num_rows(result)) {
      	return 0;
      } 

      result_list_.push_back(result);
      INT32 fields_num = ::mysql_num_fields(result);
      MYSQL_FIELD * fields = ::mysql_fetch_fields(result);
      MYSQL_ROW row;
      while ((row = ::mysql_fetch_row(result))) {
      	records_list_.push_back(CRECORD(row, fields, fields_num));
      }
      return 0;
    }
    */

    VOID CMYSQL_RECORDS::Destroy() {
      if (NULL != result_) {
      	::mysql_free_result(result_);
      }
    }
  }  // namespace db
}  // namespace lib