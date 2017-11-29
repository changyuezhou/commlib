// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include "commlib/db/inc/dba.h"
#include "commlib/db/inc/log.h"

namespace lib {
  namespace db {
    INT32 DBA::SignIn(const string & host, INT32 port, const string & db, const string & user, const string & pwd) {
      host_ = host;
      port_ = port;
      db_ = db;
      user_ = user;
      pwd_ = pwd;

      return 0;
    }
  }  // namespace db
}  // namespace lib