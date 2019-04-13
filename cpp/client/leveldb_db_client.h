#ifndef CERT_TRANS_LOG_LEVELDB_DB_H_
#define CERT_TRANS_LOG_LEVELDB_DB_H_

#include <leveldb/db.h>
#ifdef HAVE_LEVELDB_FILTER_POLICY_H
#include <leveldb/filter_policy.h>
#endif
#include <stdint.h>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>
#include <vector>

#include "log/database.h"
#include "proto/ct.pb.h"

namespace cert_trans {


//class LevelDB_client : public Database {
class LevelDB_client {
 public:
  static const size_t kTimestampBytesIndexed;

  explicit LevelDB_client(const std::string& dbfile);
  ~LevelDB_client();
  LevelDB_client(const LevelDB_client&) = delete;
  LevelDB_client& operator=(const LevelDB_client&) = delete;

  // Implement abstract functions, see database.h for comments.
  int InsertValue(std::string key, std::string value);
  int FetchValue(std::string key, std::string value);
/*
  Database::LookupResult LookupByHash(const std::string& hash,
                                      LoggedEntry* result) const override;

  Database::LookupResult LookupByIndex(int64_t sequence_number,
                                       LoggedEntry* result) const override;

  std::unique_ptr<Database::Iterator> ScanEntries(
      int64_t start_index) const override;

  Database::WriteResult WriteTreeHead_(const ct::SignedTreeHead& sth) override;

  Database::LookupResult LatestTreeHead(
      ct::SignedTreeHead* result) const override;

  int64_t TreeSize() const override;

  void AddNotifySTHCallback(
      const Database::NotifySTHCallback* callback) override;

  void RemoveNotifySTHCallback(
      const Database::NotifySTHCallback* callback) override;

  void InitializeNode(const std::string& node_id) override;

  Database::LookupResult NodeId(std::string* node_id) override;
*/

 private:
  std::unique_ptr<leveldb::DB> db_;
/*
  class Iterator;

  void BuildIndex();
  Database::LookupResult LatestTreeHeadNoLock(
      ct::SignedTreeHead* result) const;
  void InsertEntryMapping(int64_t sequence_number, const std::string& hash);

  mutable std::mutex lock_;
#ifdef HAVE_LEVELDB_FILTER_POLICY_H
  // filter_policy_ must be valid for at least as long as db_ is, so
  // keep this order.
  const std::unique_ptr<const leveldb::FilterPolicy> filter_policy_;
#endif

  int64_t contiguous_size_;
  std::unordered_map<std::string, int64_t> id_by_hash_;

  // This is a mapping of the non-contiguous entries of the log (which
  // can happen while it is being fetched). When entries here become
  // contiguous with the beginning of the tree, they are removed.
  std::set<int64_t> sparse_entries_;

  uint64_t latest_tree_timestamp_;
  std::string latest_timestamp_key_;
  cert_trans::DatabaseNotifierHelper callbacks_;
  */
};


}  // namespace cert_trans

#endif  // CERT_TRANS_LOG_LEVELDB_DB_H_
