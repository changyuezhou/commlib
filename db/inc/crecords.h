// Copyright (c) 2013 zhou chang yue. All rights reserved.

#ifndef COMMLIB_DB_INC_CRECORDS_H_
#define COMMLIB_DB_INC_CRECORDS_H_

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
    
    class CRECORDS {
     public:
       static const INT32 kLINES = 1024*10;
       static const INT32 kBUFFER_SIZE = 1024*512;
       static const INT32 kLARGE_BUFFER_SIZE = 1024*1024*8;
       
     public:
       CRECORDS():is_large_(FALSE), large_data_(NULL), total_lines_(0), columns_("") {}
       virtual ~CRECORDS () {}

     public:
       INT32 AddRow(const CHAR * data, INT32 size);
       
     public:  
       CHAR * Data() { return data_; }
       
     private:
       CRECORDS(const CRECORDS &);
       CRECORDS & operator=(const CRECORDS &);
       
     private:
       BOOL is_large_;
       CHAR * large_data_;
       INT32 total_lines_;
       const string columns_;
       CHAR data_[kBUFFER_SIZE];
       INT32 lines_offset_[kLINES];
    };
  }  // namespace db
}  // namespace lib

#endif  // COMMLIB_DB_INC_CRECORDS_H_