// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include "commlib/db/inc/cmysql.h"
#include "commlib/db/inc/log.h"
#include "commlib/db/inc/err.h"

namespace lib {
  namespace db {
    INT32 CMySQL::SignIn(const string & host, INT32 port, const string & db, const string & user, const string & pwd) {
      DBA::SignIn(host, port, db, user, pwd);

      return SignIn();
    }

    INT32 CMySQL::SignIn() {
      if (NULL == ::mysql_init(&connection_)) {
        LIB_DB_LOG_ERROR("mysql initial connection failed");

        return Err::kERR_DB_MYSQL_INIT_FAILED;
      }

      unsigned long client_flag = CLIENT_MULTI_STATEMENTS;

      if (NULL == ::mysql_real_connect(&connection_, host_.c_str(), user_.c_str(), pwd_.c_str(), db_.c_str(), port_,
                                       NULL, client_flag)) {
        LIB_DB_LOG_ERROR("mysql connect to service failed msg:" << ::mysql_error(&connection_));

        return Err::kERR_DB_SIGN_IN_FAILED;
      }

      INT8 flag = 1;
      if (0 != ::mysql_options(&connection_, MYSQL_OPT_RECONNECT, &flag)) {
        LIB_DB_LOG_WARN("mysql set reconnect failed");
      }

      return 0;
    }

    INT32 CMySQL::reconnect() {
      SignOut();
      if (NULL == ::mysql_real_connect(&connection_, host_.c_str(), user_.c_str(), pwd_.c_str(), db_.c_str(), port_, NULL, 0)) {
        LIB_DB_LOG_ERROR("mysql connect to service failed msg:" << ::mysql_error(&connection_)
                         << " host:" << host_ << " port:" << port_
                         << "db:" << db_ << " user:" << user_);

        return Err::kERR_DB_SIGN_IN_FAILED;
      }

      return 0;
    }

    BOOL CMySQL::IsConnecting() {
      return (0 == ::mysql_ping(&connection_));
    }

    INT32 CMySQL::Execute(const CSQL & sql, UINT32 affect) {
      if (0 != ::mysql_real_query(&connection_, sql.SQL().c_str(), sql.SQL().length())) {
        if ((1184 == ::mysql_errno(&connection_)) && (0 == reconnect())) {
          if (0 != ::mysql_real_query(&connection_, sql.SQL().c_str(), sql.SQL().length())) {
            LIB_DB_LOG_ERROR("mysql execute failed msg: " << ::mysql_error(&connection_));

            return -1;
          }
        } else {
          LIB_DB_LOG_ERROR("mysql execute failed msg: " << ::mysql_error(&connection_));

          return -1;
        }
      }

      if (0 != affect && affect != ::mysql_affected_rows(&connection_)) {
        LIB_DB_LOG_WARN("mysql affected rows is not equal");

        return Err::kERR_DB_MYSQL_AFFECT_RECORDS;
      }

      do {
        MYSQL_RES * result = ::mysql_store_result(&connection_);
        ::mysql_free_result(result);
      } while( !::mysql_next_result(&connection_) );

      return 0;
    }

    INT32 CMySQL::Query(CRECORDS & records, const CSQL & sql) {
      if (0 != ::mysql_real_query(&connection_, sql.SQL().c_str(), sql.SQL().length())) {
        if ((1184 == ::mysql_errno(&connection_)) && (0 == reconnect())) {
          if (0 != ::mysql_real_query(&connection_, sql.SQL().c_str(), sql.SQL().length())) {
            LIB_DB_LOG_ERROR("mysql query failed msg: " << ::mysql_error(&connection_));

            return -1;
          }
        } else {
          LIB_DB_LOG_ERROR("mysql query failed msg: " << ::mysql_error(&connection_));

          return -1;
        }
      }

      do {
        MYSQL_RES *query_result = ::mysql_store_result(&connection_);
        if (NULL == query_result) {
          LIB_DB_LOG_WARN("mysql query failed msg: " << ::mysql_error(&connection_));

          return -1;
        }

        INT32 rd_size = ::mysql_num_rows(query_result);
        if (0 >= rd_size) {
          return 0;
        }

        INT32 num_fields = ::mysql_num_fields(query_result);
        MYSQL_FIELD *fields = ::mysql_fetch_fields(query_result);
        MYSQL_ROW row;
        while ((row = ::mysql_fetch_row(query_result))) {
        /*
          RECORD record;

          for (INT32 i = 0; i < num_fields; ++i) {
            if (NULL != row[i]) {
              record.insert(make_pair(fields[i].name, row[i]));
            } else {
              record.insert(make_pair(fields[i].name, ""));
            }
          }

          records.push_back(record);
          */
        }

        ::mysql_free_result(query_result);
      } while (0 == ::mysql_next_result(&connection_));

      return 0;
    }

    INT32 CMySQL::SignOut() {
      ::mysql_close(&connection_);
      ::mysql_library_end();

      return 0;
    }

    const string CMySQL::GetErrMsg() {
      return ::mysql_error(&connection_);
    }
  }  // namespace db
}  // namespace lib
