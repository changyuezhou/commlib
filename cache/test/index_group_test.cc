// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include <string>
#include <vector>
#include "commlib/public/inc/type.h"
#include "commlib/magic/inc/timeFormat.h"
#include "commlib/magic/inc/str.h"
#include "commlib/cache/inc/cache.h"
#include "commlib/cache/inc/index_group.h"
#include "commlib/log/inc/handleManager.h"

using lib::cache::ChunkInfo;
using lib::cache::IndexMemInfo;
using lib::cache::IndexGroup;
using lib::magic::TimeFormat;
using lib::magic::String;

using lib::log::HandleManager;

using std::string;
using std::vector;

typedef vector<string> KEYS;

INT32 main(INT32 argc, CHAR ** argv) {
  INT32 result = HandleManager::GetInstance()->Initial("index_group.conf");
  if (0 != result) {
    printf("initial log file failed");
    return -1;
  }

  INT32 size = 1024*1024*20;
  CHAR * mem = reinterpret_cast<CHAR *>(::malloc(size));
  if (NULL == mem) {
    printf("memory malloc failed size: %d\n", size);

    return 0;
  }

  memset(mem, 0x00, size);
  IndexMemInfo * index_group = reinterpret_cast<IndexMemInfo *>(mem);
  ChunkInfo * chunk_info = reinterpret_cast<ChunkInfo *>(reinterpret_cast<CHAR *>(mem) + sizeof(IndexMemInfo));
  pthread_mutexattr_t attr;
  ::pthread_mutexattr_init(&attr);
  if (0 != ::pthread_mutex_init(&chunk_info->mutex_, &attr)) {
    return -1;
  }
  ::pthread_mutexattr_destroy(&attr);
  chunk_info->bottom_ = sizeof(IndexMemInfo) + sizeof(ChunkInfo);
  chunk_info->chunk_start_ = chunk_info->bottom_;
  chunk_info->top_ = size;
  chunk_info->size_ = chunk_info->top_ - chunk_info->bottom_;

  IndexGroup index_group_obj;
  if (0 != index_group_obj.Initial(index_group, chunk_info)) {
    printf("initial index group failed\n");

    return -1;
  }

  if (0 != index_group_obj.Insert("test", "111")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  if (0 != index_group_obj.Insert("test", "222")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  if (0 != index_group_obj.Insert("test1", "333")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  if (0 != index_group_obj.Insert("test2", "444")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  if (0 != index_group_obj.Insert("aaa", "000")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  if (0 != index_group_obj.Insert("zzz", "555")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  index_group_obj.Dump();
/*
  if (0 != index_group_obj.Delete("test", "222")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  if (0 != index_group_obj.Delete("aaa", "000")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  if (0 != index_group_obj.Delete("zzz", "555")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  if (0 != index_group_obj.Delete("test1", "333")) {
    printf("index group obj set test failed\n");
    return -1;
  }

  index_group_obj.Dump();
*/

  KEYS keys;
  index_group_obj.GetKeysEQ("test", keys);
  INT32 keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("eq test key: %s\n", keys[i].c_str());
  }

  keys.clear();
  index_group_obj.GetKeysNE("test", keys);
  keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("ne test key: %s\n", keys[i].c_str());
  }

  keys.clear();
  index_group_obj.GetKeysGT("test", keys);
  keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("gt test key: %s\n", keys[i].c_str());
  }

  keys.clear();
  index_group_obj.GetKeysEGT("test", keys);
  keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("gte test key: %s\n", keys[i].c_str());
  }

  keys.clear();
  index_group_obj.GetKeysLT("test", keys);
  keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("lt test key: %s\n", keys[i].c_str());
  }

  keys.clear();
  index_group_obj.GetKeysELT("test", keys);
  keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("lte test key: %s\n", keys[i].c_str());
  }

  keys.clear();
  index_group_obj.GetKeysBE("test", "test2", keys);
  keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("be test and test2 key: %s\n", keys[i].c_str());
  }

  keys.clear();
  KEYS index_keys;
  index_keys.push_back("test");
  index_keys.push_back("aaa");
  index_group_obj.GetKeysIN(index_keys, keys);
  keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("in test and aaa key: %s\n", keys[i].c_str());
  }

  keys.clear();
  index_group_obj.GetKeysNotIN(index_keys, keys);
  keys_size = keys.size();
  for (INT32 i = 0; i < keys_size; ++i) {
    printf("not in test and aaa key: %s\n", keys[i].c_str());
  }

  delete HandleManager::GetInstance();
  ::free(mem);

  return 0;
}

