// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include "commlib/magic/inc/str.h"
#include "commlib/magic/inc/url_escape.h"
#include "commlib/db/inc/csql.h"
#include "commlib/db/inc/log.h"
#include "commlib/db/inc/err.h"

namespace lib {
  namespace db {
    using lib::magic::StringSplit;
    using lib::magic::String;
    using lib::magic::URL_ESCAPE;
    using std::size_t;

    INT32 CSQL::ParseSQL() {
      op_ = OTHERS;

      sql_ = String::Trim_Left(String::Trim_Right(sql_));
      if (0 == ::strncasecmp(sql_.c_str(), "insert ", 7)) {
        return ParseInsert(sql_);
      } else if (0 == ::strncasecmp(sql_.c_str(), "update ", 7)) {
        return ParseUpdate(sql_);
      } else if (0 == ::strncasecmp(sql_.c_str(), "delete ", 7)) {
        return ParseDelete(sql_);
      } else if (0 == ::strncasecmp(sql_.c_str(), "select ", 7)) {
        return ParseSelect(sql_);
      }

      return 0;
    }

    INT32 CSQL::ParseSQL(const string & sql) {
      sql_ = sql;
      return ParseSQL();
    }

    INT32 CSQL::ParseInsert(const string & sql) {
      op_ = INSERT;

      string rest_sql = FindTableName(sql, " into ", "value");
      if (0 >= rest_sql.length()) {
        return Err::kERR_DB_SQL_INSERT_HAS_NO_TABLE_NAME;
      }

      LIB_DB_LOG_DEBUG("CSQL after find table name: " << table_name_ << " sql:" << rest_sql);

      size_t left_bracket = table_name_.find("(");
      if (string::npos != left_bracket) {
        INT32 result = FindInsertColumn(table_name_.substr(left_bracket));
        if (0 != result) {
          return result;
        }

        table_name_ = table_name_.substr(0, left_bracket);
      }

      size_t db_start = table_name_.find(".");
      if (string::npos != db_start) {
        db_name_ = table_name_.substr(0, db_start);
        table_name_ = table_name_.substr(db_start+1);
      }

      INT32 result = FindColumnValues(String::Trim_Left(String::Trim_Right(rest_sql)));
      if (0 != result) {
        return result;
      }

      return 0;
    }

    INT32 CSQL::ParseUpdate(const string & sql) {
      op_ = UPDATE;

      string rest_sql = FindTableName(sql, "update ", " set ");
      if (0 >= rest_sql.length()) {
        return Err::kERR_DB_SQL_INSERT_HAS_NO_TABLE_NAME;
      }

      LIB_DB_LOG_DEBUG("CSQL after find table name: " << table_name_ << " sql:" << rest_sql);

      size_t db_start = table_name_.find(".");
      if (string::npos != db_start) {
        db_name_ = table_name_.substr(0, db_start);
        table_name_ = table_name_.substr(db_start+1);
      }

      INT32 result = FindUpdateColumnAndValue(rest_sql);
      if (0 != result) {
        return result;
      }

      return FindConditions(rest_sql, ";");
    }

    INT32 CSQL::ParseDelete(const string & sql) {
      op_ = DELETE;

      string rest_sql = FindTableName(sql, " from ", " where ");
      if (0 >= rest_sql.length()) {
        rest_sql = FindTableName(sql, " from ", ";");
        if (0 >= rest_sql.length() && 0 >= table_name_.length()) {
          return Err::kERR_DB_SQL_INSERT_HAS_NO_TABLE_NAME;
        }
      }

      LIB_DB_LOG_DEBUG("CSQL after find table name: " << table_name_ << " sql:" << rest_sql);

      size_t db_start = table_name_.find(".");
      if (string::npos != db_start) {
        db_name_ = table_name_.substr(0, db_start);
        table_name_ = table_name_.substr(db_start+1);
      }      

      return FindConditions(rest_sql, ";");
    }

    INT32 CSQL::ParseSelect(const string & sql) {
      op_ = SELECT;

      string rest_sql = FindSelectColumns(sql);
      if (0 >= rest_sql.length()) {
        LIB_DB_LOG_ERROR("CSQL find select column failed sql:" << sql << " rest sql is empty");
        return Err::kERR_DB_SQL_SELECT_HAS_NO_REQUIRED_KEY_WORDS;
      }

      LIB_DB_LOG_DEBUG("CSQL after find select column sql:" << rest_sql);

      rest_sql = FindTableName(sql, " from ", " where ");
      if (0 >= rest_sql.length()) {
        rest_sql = FindTableName(sql, " from ", ";");
        if (0 >= rest_sql.length() && 0 >= table_name_.length()) {
          return Err::kERR_DB_SQL_INSERT_HAS_NO_TABLE_NAME;
        }
      }

      LIB_DB_LOG_DEBUG("CSQL after find table name: " << table_name_ << " sql:" << rest_sql);

      if (string::npos != table_name_.find(",")) {
        LIB_DB_LOG_DEBUG("CSQL parser select multiple tables:" << table_name_);
        is_access_cache_ = FALSE;
      }

      size_t db_start = table_name_.find(".");
      if (string::npos != db_start) {
        db_name_ = table_name_.substr(0, db_start);
        table_name_ = table_name_.substr(db_start+1);
      }

      if ((string::npos == rest_sql.find(" group ")) && (string::npos == rest_sql.find(" GROUP "))
        && (string::npos == rest_sql.find(" order ")) && (string::npos == rest_sql.find(" ORDER ")) 
        && (string::npos == rest_sql.find(" limit ")) && (string::npos == rest_sql.find(" LIMIT ")) 
        && (string::npos == rest_sql.find(" having ")) && (string::npos == rest_sql.find(" HAVING "))) {
        INT32 result = FindConditions(rest_sql, ";", TRUE);
        if (0 != result) {
          return result;
        }

        if (0 >= conditions_columns_.size()) {
          is_access_cache_ = FALSE;
        }
      }

      return 0;
    }

    BOOL CSQL::IsConditionHasColumns(const string & column) const {
      if (conditions_columns_.find(column) != conditions_columns_.end()) {
        return TRUE;
      }

      return FALSE;
    }

    BOOL CSQL::IsUpdateHasColumns(const string & column) const { 
      if (find(update_columns_.begin(), update_columns_.end(), column) != update_columns_.end()) {
        return TRUE;
      }

      return FALSE; 
    }

    BOOL CSQL::IsConditionsOPEQ(const string & column) const {
      if (0 >= conditions_columns_.size()) {
        return FALSE;
      }

      CONDITIONS_LIST::const_iterator c_it = conditions_columns_.find(column);
      if (c_it == conditions_columns_.cend()) {
        return FALSE;
      }

      for (UINT32 i = 0; i < c_it->second.size(); ++i) {
        const CONDITION_ITEM & item = c_it->second[i];
        if (EQ != item.condition_op_) {
          return FALSE;
        }
      }

      return TRUE;
    }

    const CSQL::COLUMN_LIST CSQL::GetConditionsColumns() const {
      COLUMN_LIST column_list;

      if (0 >= conditions_columns_.size()) {
        return column_list;
      }

      for (CONDITIONS_LIST::const_iterator c_it = conditions_columns_.cbegin(); c_it != conditions_columns_.cend(); ++c_it) {
        column_list.push_back(c_it->first);
      }

      return column_list;
    }

    BOOL CSQL::GetConditionsItems(const string & column, CONDITION_ITEM_LIST & item_list) const {
      CONDITIONS_LIST::const_iterator c_it = conditions_columns_.find(column);
      if (c_it == conditions_columns_.cend()) {
        return FALSE;
      }

      for (UINT32 i = 0; i < c_it->second.size(); ++i) {
        item_list.push_back(c_it->second[i]);
      }

      return TRUE;
    }

    const string CSQL::FindTableName(const string & sql, const string & begin_key, const string & end_key) {
      // process first key words like: into, from, update
      string upper_begin_key = begin_key;
      transform(upper_begin_key.begin(), upper_begin_key.end(), upper_begin_key.begin(), toupper);
      size_t begin_key_pos = sql.find(begin_key);
      size_t upper_begin_key_pos = sql.find(upper_begin_key);
      begin_key_pos = (string::npos == begin_key_pos)? upper_begin_key_pos:begin_key_pos;
      if (string::npos == begin_key_pos) {
        LIB_DB_LOG_ERROR("CSQL find table name the begin key" << begin_key << " is not found sql:" << sql);
        return "";
      }
      const string rest_sql = String::Trim_Left(sql.substr(begin_key_pos + begin_key.length()));

      LIB_DB_LOG_DEBUG("CSQL find table name after searching begin key sql:" << rest_sql << " begin key:" << begin_key << " end key:" << end_key);

      // process second key words like value,values,where
      string upper_end_key = end_key;
      transform(upper_end_key.begin(), upper_end_key.end(), upper_end_key.begin(), toupper);

      size_t end = rest_sql.find(end_key);
      if (string::npos == end) {
        end = rest_sql.find(upper_end_key);
      }

      if (string::npos == end) {
        return "";
      }

      string table_name = rest_sql.substr(0, end);

      LIB_DB_LOG_DEBUG("CSQL find table name after searching end key table name:" << table_name_);

      // process insert into table_name( value ,others) value ();
      if (string::npos != table_name.find("(") && string::npos == table_name.find(")")) {
        LIB_DB_LOG_DEBUG("CSQL find table name end key is in bracket ................");
        size_t right_bracket = rest_sql.find(")");
        if (string::npos == right_bracket) {
          LIB_DB_LOG_ERROR("CSQL find table name column has no right bracket sql:" << sql);
          return "";
        }
        LIB_DB_LOG_DEBUG("CSQL find table name right_bracket:" << right_bracket << " ...... ");

        table_name = rest_sql.substr(0, right_bracket + 1);
      }

      table_name_ = String::Trim_Left(String::Trim_Right(table_name));   

      return String::Trim_Left(rest_sql.substr(table_name.length() + 1));
    }

    INT32 CSQL::FindInsertColumn(const string & sql) {
      size_t right_bracket = sql.find(")");
      if (string::npos == right_bracket) {
        LIB_DB_LOG_ERROR("CSQL find insert column right bracket is not found sql:" << sql);
        return Err::kERR_DB_SQL_INSERT_HAS_NO_RIGHT_BRACKET;
      }

      const string columns = sql.substr(1, right_bracket-1);
      StringSplit split(columns, ",");
      INT32 size = split.size();
      for (INT32 i = 0; i < size; ++i) {
        insert_columns_.push_back(split[i]);
      }

      return 0;
    }

    INT32 CSQL::FindColumnValues(const string & sql) {
      LIB_DB_LOG_DEBUG("CSQL find column values sql:" << sql);
      size_t start = string::npos;
      if (0 == ::strncasecmp(sql.c_str(), "values", 6)) {
        start += 6;
      } else if (0 == ::strncasecmp(sql.c_str(), "value", 5)) {
        start += 5;
      }

      if (string::npos == start) {
        return Err::kERR_DB_SQL_INSERT_HAS_NO_VALUE;
      }

      string rest_sql = String::Trim_Left(sql.substr(start));
      while (0 < rest_sql.length() && rest_sql.at(0) != ';') {
        size_t left_bracket = rest_sql.find("(");
        size_t right_bracket = rest_sql.find(")");

        if (string::npos == left_bracket || string::npos == right_bracket) {
          return Err::kERR_DB_SQL_INSERT_VALUES_BRACKET_INVALID;
        }

        COLUMN_VALUE value;
        StringSplit split(rest_sql.substr(left_bracket+1, right_bracket-1), ",");
        INT32 size = split.size();
        for (INT32 i = 0; i < size; ++i) {
          string column = String::Trim_Left(String::Trim_Right(split[i]));
          if (column.at(0) == '\'' || column.at(0) == '"') {
            column.at(0) = ' ';
            column.at(column.length()-1) = ' ';
          }

          value.push_back(String::Trim_Left(String::Trim_Right(column)));
        }

        insert_values_.push_back(value);

        rest_sql = String::Trim_Left(rest_sql.substr(right_bracket + 1));
      }

      return 0;                
    }

    INT32 CSQL::FindUpdateColumnAndValue(const string & sql) {
      size_t start = string::npos;
      if (0 == ::strncasecmp(sql.c_str(), "set ", 4)) {
        start += 4;
      }

      if (string::npos == start) {
        return Err::kERR_DB_SQL_UPDATE_HAS_NO_REQUIRED_KEY_WORDS;
      }

      string rest_sql = String::Trim_Left(sql.substr(start));

      LIB_DB_LOG_DEBUG("CSQL find update columns and values sql:" << rest_sql);

      size_t columns_end = rest_sql.find(" where ");
      if (string::npos == columns_end) {
        columns_end = rest_sql.find(" WHERE ");
      }

      if (string::npos == columns_end) {
        columns_end = rest_sql.find(";");
      }

      const string columns_values_str = String::Trim_Left(String::Trim_Right(rest_sql.substr(0, columns_end)));
      StringSplit split(columns_values_str, ",");
      INT32 columns_size = split.size();
      for (INT32 i = 0; i < columns_size; ++i) {
        const string column_value = String::Trim_Left(String::Trim_Right(split[i]));
        StringSplit split_column_value(column_value, "=");
        if (2 != split_column_value.size()) {
          return Err::kERR_DB_SQL_UPDATE_COLUMN_VALUE_INVALID;
        }
        update_columns_.push_back(String::Trim_Left(String::Trim_Right(split_column_value[0])));
        string value = String::Trim_Left(String::Trim_Right(split_column_value[1]));
        if (value.at(0) == '\'' || value.at(0) == '"') {
          value.at(0) = ' ';
          value.at(value.length()-1) = ' ';
        }
        update_values_.push_back(String::Trim_Left(String::Trim_Right(value)));
      }

      return 0;
    }

    const string CSQL::FindSelectColumns(const string & sql) {
      string rest_sql = String::Trim_Left(sql.substr(7));
      
      LIB_DB_LOG_DEBUG("CSQL find select columns sql:" << rest_sql);

      size_t end = rest_sql.find(" from ");
      if (string::npos == end) {
        end = rest_sql.find(" FROM ");
        if (string::npos == end) {
          return "";
        }
      }

      StringSplit split(rest_sql.substr(0, end), ",");
      INT32 column_size = split.size();
      for (INT32 i = 0; i < column_size; ++i) {
        const string column = String::Trim_Right(String::Trim_Left(split[i]));
        if (column.at(0) != '*') {
          select_columns_.push_back(column);
        }
      }

      return String::Trim_Left(rest_sql.substr(end));      
    }

    INT32 CSQL::FetchConditionColumnValue(string & item, string & value) {
      item = String::Trim_Left(String::Trim_Right(item));
      size_t value_end = 0;
      size_t value_start = 0;
      if ('\'' == item.at(0)) {
        value_start = 1;
        value_end = item.find("\'", 1);
        if (string::npos == value_end) {
          LIB_DB_LOG_ERROR("CSQL FetchConditionColumnValue sql item:" << item << " is invalid");
          return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;
        }
      } else if ('(' == item.at(0)) {
        value_start = 1;
        value_end = item.find(")", 1);
        if (string::npos == value_end) {
          LIB_DB_LOG_ERROR("CSQL FetchConditionColumnValue sql item:" << item << " is invalid");
          return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;
        }
      } else {
        value_end = item.find(" ");
      }

      value = item.substr(value_start, value_end);

      if (string::npos == value_end) {
        item = "";
      } else {
        item = String::Trim_Left(item.substr(value_end + 1));
      }

      return 0;
    }

    INT32 CSQL::DoConditionItems(const string & item, const string & key) {
      StringSplit condition_item_array(String::Trim_Left(String::Trim_Right(item)), key);
      LIB_DB_LOG_DEBUG("CSQL DoConditionItems sql:" << item << " key:" << key);
      for (INT32 i = 0; i < condition_item_array.size(); ++i) {
        string item_str = String::Trim_Left(String::Trim_Right(condition_item_array[i]));
        LIB_DB_LOG_DEBUG("CSQL DoConditionItems item string:" << item_str);        
        size_t column_end = item_str.find_first_of("><= ");
        if (string::npos == column_end) {
          LIB_DB_LOG_ERROR("CSQL DoConditionItems sql condition:" << item_str << " is invalid");
          return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;          
        }

        const string column = item_str.substr(0, column_end);
        if (' ' == item_str.at(column_end)) {
          item_str = String::Trim_Left(String::Trim_Right((item_str.substr(column_end+1))));
        } else {
          item_str = String::Trim_Left(String::Trim_Right((item_str.substr(column_end))));
        }

        LIB_DB_LOG_DEBUG("CSQL DoConditionItems fetch column:" << column << " item_str:" << item_str << " .......");

        size_t op_end = item_str.find_first_not_of("><= ");
        if (string::npos == op_end) {
          LIB_DB_LOG_ERROR("CSQL DoConditionItems sql condition:" << item_str << " is invalid");
          return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;          
        }

        if (0 == op_end) {
          op_end = item_str.find_first_of(" ");
          if (string::npos == op_end) {
            LIB_DB_LOG_ERROR("CSQL DoConditionItems sql condition:" << item_str << " is invalid");
            return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;          
          }          
        }

        const string op = String::Trim_Left(String::Trim_Right(item_str.substr(0, op_end)));
        item_str = String::Trim_Left(String::Trim_Right((item_str.substr(op_end))));

        LIB_DB_LOG_DEBUG("CSQL DoConditionItems column:" << column << " op:" << op << " ..................");

        CONDITION_ITEM c_item;

        // do between begin
        if ((7 == op.length()) && (0 == ::strncasecmp(op.c_str(), "between", op.length()))) {
          c_item.condition_op_ = RANGE;
          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          CHAR buff[1024*4] = {0};
          URL_ESCAPE::url_encode(String::Trim_Left(String::Trim_Right(value)).c_str(), buff, sizeof(buff));
          ::strcpy(buff + ::strlen(buff), "&");
          c_item.value_ = buff;

          if (0 >= item_str.length()) {
            LIB_DB_LOG_ERROR("CSQL DoConditionItems sql condition:" << item_str << " is invalid");            
            return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;
          }

          if (0 != ::strncasecmp(item_str.c_str(), "OR ", 3)) {
            LIB_DB_LOG_ERROR("CSQL DoConditionItems sql condition:" << item_str << " is invalid");            
            return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;
          }
          item_str = String::Trim_Left(item_str.substr(3));

          value = "";
          result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          ::memset(buff, 0x00, sizeof(buff));
          URL_ESCAPE::url_encode(String::Trim_Left(String::Trim_Right(value)).c_str(), buff, sizeof(buff));
          c_item.value_ += buff;          
        }  // do between end

        if ((3 == op.length()) && (0 == ::strncasecmp(op.c_str(), "not", op.length()))) {
          if (0 != ::strncasecmp(item_str.c_str(), "in ", 3)) {
            LIB_DB_LOG_ERROR("CSQL DoConditionItems sql condition:" << item_str << " is invalid");            
            return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;            
          }

          item_str = String::Trim_Left(item_str.substr(3));

          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          StringSplit split(value, ",");
          value = "";
          for (INT32 j = 0; j < split.size(); ++j) {
            string split_item = String::Trim_Left(String::Trim_Right(split[j]));
            if ('\'' == split_item.at(0)) {
              split_item.at(0) = ' ';
              split_item.at(split_item.length() - 1) = ' ';
            }
            split_item = String::Trim_Left(String::Trim_Right(split_item));

            value += split_item;
            if (j < (split.size() - 1)) {
              value += ",";
            }
          }

          c_item.condition_op_ = NE;
          c_item.value_ = value;
        }

        if ((2 == op.length()) && (0 == ::strncasecmp(op.c_str(), "in", op.length()))) {
          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          StringSplit split(value, ",");
          value = "";
          for (INT32 j = 0; j < split.size(); ++j) {
            string split_item = String::Trim_Left(String::Trim_Right(split[j]));
            if ('\'' == split_item.at(0)) {
              split_item.at(0) = ' ';
              split_item.at(split_item.length() - 1) = ' ';
            }
            split_item = String::Trim_Left(String::Trim_Right(split_item));

            value += split_item;
            if (j < (split.size() - 1)) {
              value += ",";
            }
          }

          c_item.condition_op_ = EQ;
          c_item.value_ = value;
        }        

        if ((2 == op.length()) && (0 == ::strncasecmp(op.c_str(), "<=", op.length()))) {
          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          c_item.condition_op_ = ELT;
          c_item.value_ = value;
        }

        if ((2 == op.length()) && (0 == ::strncasecmp(op.c_str(), ">=", op.length()))) {
          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          c_item.condition_op_ = EGT;
          c_item.value_ = value;
        }

        if ((2 == op.length()) && (0 == ::strncasecmp(op.c_str(), "<>", op.length()))) {
          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          c_item.condition_op_ = NE;
          c_item.value_ = value;
        }        

        if ((1 == op.length()) && (0 == ::strncasecmp(op.c_str(), ">", op.length()))) {
          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          c_item.condition_op_ = GT;
          c_item.value_ = value;
        }

        if ((1 == op.length()) && (0 == ::strncasecmp(op.c_str(), "<", op.length()))) {
          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          c_item.condition_op_ = LT;
          c_item.value_ = value;
        }

        if ((1 == op.length()) && (0 == ::strncasecmp(op.c_str(), "=", op.length()))) {
          string value = "";
          INT32 result = FetchConditionColumnValue(item_str, value);
          if (0 != result) {
            return result;              
          }

          c_item.condition_op_ = EQ;
          c_item.value_ = value;
        }                

        CONDITIONS_LIST::iterator it = conditions_columns_.find(column);
        if (it != conditions_columns_.end()) {
          it->second.push_back(c_item);
        } else {
          CONDITION_ITEM_LIST list;
          list.push_back(c_item);
          if (!conditions_columns_.insert(make_pair(column, list)).second) {
            LIB_DB_LOG_ERROR("CSQL DoConditionItem insert condition:" << column << " failed");
            return Err::kERR_DB_SQL_CONDITION_DO_ITEM_INVALID;
          } 
        }    
      }
      return 0;
    }

    INT32 CSQL::DoBetween(const string & conditions) {
      const INT32 kBETWEEN = 1;
      const INT32 kNON_BETWEEN = 2;

      StringSplit convert_array(conditions, " ");
      string convert2new = "";
      const string key = "OR";
      INT32 flag = kNON_BETWEEN;
      for (INT32 i = 0; i < convert_array.size(); ++i) {
        const string item = String::Trim_Left(String::Trim_Right(convert_array[i]));
        if (7 == item.length() && 0 == ::strncasecmp(item.c_str(), "between", item.length())) {
          flag = kBETWEEN;
        }

        if ((3 == item.length()) && (0 == ::strncasecmp(item.c_str(), "and", item.length()))) {
          if (kBETWEEN == flag) {
            convert2new += " " + key + " ";
            flag = kNON_BETWEEN;
          } else {
            convert2new += " AND ";
          }

          continue;
        }

        convert2new += " " + item;
      }

      return DoConditionItems(convert2new, "AND");
    }

    INT32 CSQL::DoOthers(const string & conditions) {
      string convert2new = conditions;

      size_t pos = string::npos;
      while (string::npos != (pos = convert2new.find(" and "))) {
        convert2new = convert2new.replace(pos, 5, " AND ");
      }
      
      return DoConditionItems(convert2new, "AND");
    }

    BOOL CSQL::CheckBracketIsAfterIN(const string & conditions) {
      size_t pos = string::npos;
      size_t start = 0;
      while (string::npos != (pos = conditions.find("(", start))) {
        const string prev = String::Trim_Right(conditions.substr(0, pos));
        INT32 prev_len = prev.length();
        if (2 >= prev_len) {
          return FALSE;
        }

        if ('n' != prev.at(prev_len-1) && 'N' != prev.at(prev_len-1)) {
          return FALSE;
        }

        if ('i' != prev.at(prev_len-2) && 'I' != prev.at(prev_len-2)) {
          return FALSE;
        }

        size_t end = conditions.find(")", pos);
        if (string::npos == end) {
          return FALSE;
        }
        if (end == (conditions.length() - 1)) {
          break;
        }          

        start = end + 1;
      }

      return TRUE;
    }

    INT32 CSQL::FindConditionsItems(const string & conditions) {
      LIB_DB_LOG_DEBUG("CSQL find conditions:" << conditions);
      if (string::npos != conditions.find("(") && FALSE == CheckBracketIsAfterIN(conditions)) {
        return 0;
      } 

      if (string::npos != conditions.find(" or ") || string::npos != conditions.find(" OR ")) {
        return 0;
      }

      if (string::npos != conditions.find(" like ") || string::npos != conditions.find(" LIKE ")) {
        return 0;
      }

      if (string::npos != conditions.find(" between ") || string::npos != conditions.find(" BEWTEEN ")) {
        return DoBetween(conditions);
      }

      return DoOthers(conditions);
    }

    INT32 CSQL::FindConditions(const string & sql, const string & end_key, BOOL need_parsing) {
      size_t pos = (string::npos == sql.find("where "))? sql.find("WHERE "):sql.find("where ");
      if (string::npos == pos) {
        LIB_DB_LOG_ERROR("CSQL find conditions sql:" << sql << " is invalid");
        return Err::kERR_DB_SQL_CONDITION_INVALID;
      }
      string rest_sql = String::Trim_Right(String::Trim_Left(sql.substr(pos + 6)));
      
      LIB_DB_LOG_DEBUG("CSQL find conditions sql:" << rest_sql);

      if (end_key.at(0) == ';') {
        if (';' != rest_sql.at(rest_sql.length() - 1)) {
          conditions_str_ = rest_sql.substr(0, rest_sql.length());
        } else {
          conditions_str_ = rest_sql.substr(0, rest_sql.length() - 1);          
        }

        return FindConditionsItems(conditions_str_);
      }

      string upper_end_key = end_key;
      transform(upper_end_key.begin(), upper_end_key.end(), upper_end_key.begin(), toupper);

      size_t end = rest_sql.find(end_key);
      if (string::npos == end) {
        end = rest_sql.find(upper_end_key);
      }

      if (string::npos == end) {
        return -1;
      }

      conditions_str_ = String::Trim_Left(String::Trim_Right(rest_sql.substr(0, end)));

      if (FALSE == need_parsing) {
        return 0;
      }

      return FindConditionsItems(conditions_str_);
    }

    VOID CSQL::Dump() {
      LIB_DB_LOG_DEBUG("SQL BEGIN *************************************************************");
      LIB_DB_LOG_DEBUG("sql:" << sql_);
      LIB_DB_LOG_DEBUG("db_name:" << db_name_);
      LIB_DB_LOG_DEBUG("table_name:" << table_name_); 
      if (INSERT == op_) {
        INT32 columns_size = insert_columns_.size();
        string columns = "";
        for (INT32 i = 0; i < columns_size; ++i) {
          columns = columns + insert_columns_[i];
          if (i < (columns_size - 1)) {
            columns = columns + ",";
          }
        }

        LIB_DB_LOG_DEBUG("INSERT columns:" << columns);

        INT32 records_size = insert_values_.size();
        for (INT32 i = 0; i < records_size; ++i) {
          INT32 value_size = insert_values_[i].size();
          string value = "";
          for (INT32 j = 0; j < value_size; ++j) {
            value = value + insert_values_[i][j];
            if (j < (value_size - 1)) {
              value = value + ",";
            }
          }

          LIB_DB_LOG_DEBUG("INSERT value:" << value);
        }
      }

      if (UPDATE == op_) {
        INT32 columns_size = update_columns_.size();
        string columns = "";
        for (INT32 i = 0; i < columns_size; ++i) {
          columns = columns + update_columns_[i];
          columns += "=";
          columns += update_values_[i];
          if (i < (columns_size - 1)) {
            columns = columns + ",";
          }
        }

        LIB_DB_LOG_DEBUG("UPDATE column values:" << columns);                 
      }

      if (DELETE == op_) {

      }

      if (SELECT == op_) {
        INT32 columns_size = select_columns_.size();
        string columns = "";
        for (INT32 i = 0; i < columns_size; ++i) {
          columns = columns + select_columns_[i];
          if (i < (columns_size - 1)) {
            columns = columns + ",";
          }
        }

        LIB_DB_LOG_DEBUG("SELECT columns:" << columns);
        LIB_DB_LOG_DEBUG("can access cache:" << is_access_cache_);
      }

      LIB_DB_LOG_DEBUG("conditions:" << conditions_str_);
      for (CONDITIONS_LIST::const_iterator c_it = conditions_columns_.cbegin(); c_it != conditions_columns_.cend(); ++c_it) {
        const string column = c_it->first;
        const CONDITION_ITEM_LIST & c_list = c_it->second;
        for (UINT32 i = 0; i < c_list.size(); ++i) {
          const CONDITION_ITEM & c_item = c_list[i];
          string op = "";
          if (EQ == c_item.condition_op_) {
            op = "=";
          }

          if (NE == c_item.condition_op_) {
            op = "<>";
          }

          if (LT == c_item.condition_op_) {
            op = "<";
          }

          if (ELT == c_item.condition_op_) {
            op = "<=";
          }

          if (GT == c_item.condition_op_) {
            op = ">";
          }

          if (EGT == c_item.condition_op_) {
            op = ">=";
          }

          if (RANGE == c_item.condition_op_) {
            op = "RANGE";
          }
          LIB_DB_LOG_DEBUG("condition item:" << column << " " << op << " " << c_item.value_);
        }
     }
      LIB_DB_LOG_DEBUG("SQL END ***************************************************************");
    }
  }  // namespace db
}  // namespace lib