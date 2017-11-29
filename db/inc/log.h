// Copyright (c) 2013 zhou chang yue. All rights reserved.

#ifndef COMMLIB_DB_INC_LOG_H_
#define COMMLIB_DB_INC_LOG_H_

#include <string>
#include "commlib/public/inc/type.h"
#include "commlib/log/inc/log.h"

namespace lib {
  namespace db {
    #define LIB_DB_LOG_DEBUG(expression) LOG_DEBUG(0xFFFFFFFFFFFFFFFF, expression)
    #define LIB_DB_LOG_INFO(expression) LOG_INFO(0xFFFFFFFFFFFFFFFF, expression)
    #define LIB_DB_LOG_WARN(expression) LOG_WARN(0xFFFFFFFFFFFFFFFF, expression)
    #define LIB_DB_LOG_ERROR(expression) LOG_ERROR(0xFFFFFFFFFFFFFFFF, expression)
    #define LIB_DB_LOG_FATAL(expression) LOG_FATAL(0xFFFFFFFFFFFFFFFF, expression)
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_LOG_H_
