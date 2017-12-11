// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include "commlib/db/inc/crecords.h"
#include "commlib/db/inc/log.h"

namespace lib {
  namespace db {
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

    VOID CRECORDS::Destroy() {
      INT32 size = result_list_.size();
      for (INT32 i = 0; i < size; ++i) {
      	if (NULL != result_list_[i]) {
      	  ::mysql_free_result(result_list_[i]);
      	}
      }

      result_list_.clear();
      records_list_.clear();
    }
  }  // namespace db
}  // namespace lib