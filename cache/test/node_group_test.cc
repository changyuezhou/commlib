// Copyright (c) 2013 zhou chang yue. All rights reserved.

#include <string>
#include <vector>
#include "commlib/public/inc/type.h"
#include "commlib/magic/inc/timeFormat.h"
#include "commlib/magic/inc/str.h"
#include "commlib/cache/inc/cache.h"
#include "commlib/cache/inc/node_group.h"
#include "commlib/log/inc/handleManager.h"

using lib::cache::ChunkInfo;
using lib::cache::NodeMemInfo;
using lib::cache::Cache;
using lib::magic::TimeFormat;
using lib::magic::String;

using lib::log::HandleManager;

using std::string;
using std::vector;

INT32 main(INT32 argc, CHAR ** argv) {
  INT32 result = HandleManager::GetInstance()->Initial("node_group.conf");
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
  NodeMemInfo * node_group = reinterpret_cast<NodeMemInfo *>(mem);
  ChunkInfo * chunk_info = reinterpret_cast<ChunkInfo *>(reinterpret_cast<CHAR *>(mem) + sizeof(NodeMemInfo));
  pthread_mutexattr_t attr;
  ::pthread_mutexattr_init(&attr);
  if (0 != ::pthread_mutex_init(&chunk_info->mutex_, &attr)) {
    return -1;
  }
  ::pthread_mutexattr_destroy(&attr);
  chunk_info->bottom_ = sizeof(NodeMemInfo) + sizeof(ChunkInfo);
  chunk_info->chunk_start_ = chunk_info->bottom_;
  chunk_info->top_ = size;
  chunk_info->size_ = chunk_info->top_ - chunk_info->bottom_;

  Cache cache;
  if (0 != cache.Initial(node_group, chunk_info)) {
    printf("initial cache failed\n");

    return -1;
  }

  if (0 != cache.Set("test", "111", 3)) {
    printf("cache set test failed\n");
    return -1;
  }
  cache.Dump();
/*
  CHAR buf[1024] = {0};
  INT32 max_size = sizeof(buf);
  if (0 != cache.Get("test", buf, &max_size)) {
    printf("cache get test failed\n");
  }
*/
  printf("delete key:test success\n");

  delete HandleManager::GetInstance();
  ::free(mem);

  return 0;
}

