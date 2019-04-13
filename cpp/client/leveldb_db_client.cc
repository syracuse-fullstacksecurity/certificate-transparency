#include "client/leveldb_db_client.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <stdint.h>
#include <map>
#include <string>

#include "monitoring/latency.h"
#include "monitoring/monitoring.h"
#include "proto/ct.pb.h"
#include "proto/serializer.h"
#include "util/util.h"

using cert_trans::serialization::DeserializeResult;
using std::chrono::milliseconds;
using std::lock_guard;
using std::make_pair;
using std::min;
using std::mutex;
using std::string;
using std::unique_lock;
using std::unique_ptr;

DEFINE_int32(leveldb_max_open_files, 0,
             "number of open files that can be used by leveldb");
DEFINE_int32(leveldb_bloom_filter_bits_per_key, 0,
             "number of open files that can be used by leveldb");

namespace cert_trans {


LevelDB_client::LevelDB_client(const string& dbfile){
  leveldb::Options options;
  options.create_if_missing = true;
  if (FLAGS_leveldb_max_open_files > 0) {
    options.max_open_files = FLAGS_leveldb_max_open_files;
  }
#ifdef HAVE_LEVELDB_FILTER_POLICY_H
  options.filter_policy = filter_policy_.get();
#else
  CHECK_EQ(FLAGS_leveldb_bloom_filter_bits_per_key, 0)
      << "this version of leveldb does not have bloom filter support";
#endif
  leveldb::DB* db;
  leveldb::Status status(leveldb::DB::Open(options, dbfile, &db));
  CHECK(status.ok()) << status.ToString();
  db_.reset(db);

  //BuildIndex();
}

int LevelDB_client::InsertValue(std::string key, std::string value){
    leveldb::Status s = db_->Put(leveldb::WriteOptions(), key, value);
    if(s.ok()){
        return 1;
    }else{
        return 0;
    }
}

int LevelDB_client::FetchValue(std::string key, std::string value){
    leveldb::Status s = db_->Get(leveldb::ReadOptions(), key, &value);
    printf("  value=%s\n\n", value.c_str());
    if(s.ok()){
        return 1;
    }else{
        return 0;
    }
}

LevelDB_client::~LevelDB_client(){
  ;
}






}