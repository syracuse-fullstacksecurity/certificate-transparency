#ifndef CERT_TRANS_LOG_CONSISTENT_STORE_H_
#define CERT_TRANS_LOG_CONSISTENT_STORE_H_

#include <stdint.h>
#include <mutex>
#include <vector>

#include "base/macros.h"
#include "util/status.h"
#include "util/statusor.h"

namespace ct {

class ClusterNodeState;
class SignedTreeHead;

}  // namespace ct;


namespace cert_trans {


template <class Logged>
class EtcdConsistentStore;


// Wraps an instance of |T| and associates it with a versioning handle
// (required for atomic 'compare-and-update' semantics.)
template <class T>
class EntryHandle {
 public:
  EntryHandle() = default;

  const T& Entry() const {
    return entry_;
  }

  T* MutableEntry() {
    return &entry_;
  }

  bool HasHandle() const {
    return has_handle_;
  }

  int Handle() const {
    return handle_;
  }

 private:
  EntryHandle(const T& entry, int handle)
      : entry_(entry), has_handle_(true), handle_(handle) {
  }

  explicit EntryHandle(const T& entry) : entry_(entry), has_handle_(false) {
  }

  void Set(const T& entry, int handle) {
    entry_ = entry;
    handle_ = handle;
    has_handle_ = true;
  }

  void SetHandle(int new_handle) {
    handle_ = new_handle;
    has_handle_ = true;
  }

  T entry_;
  bool has_handle_;
  int handle_;

  template <class Logged>
  friend class EtcdConsistentStore;
  friend class EtcdConsistentStoreTest;
};


template <class T>
struct Update {
  Update(const EntryHandle<T>& handle, bool exists)
      : handle_(handle), exists_(exists) {
  }

  Update(const Update& other) = default;

  const EntryHandle<T> handle_;
  const bool exists_;
};


template <class Logged>
class ConsistentStore {
 public:
  typedef std::function<void(const Update<ct::SignedTreeHead>& update)>
      ServingSTHCallback;
  typedef std::function<void(const std::vector<Update<ct::ClusterNodeState>>&
                                 updates)> ClusterNodeStateCallback;


  ConsistentStore() = default;

  virtual util::StatusOr<int64_t> NextAvailableSequenceNumber() const = 0;

  virtual util::Status SetServingSTH(const ct::SignedTreeHead& new_sth) = 0;

  virtual util::Status AddPendingEntry(Logged* entry) = 0;

  virtual util::Status GetPendingEntryForHash(
      const std::string& hash, EntryHandle<Logged>* entry) const = 0;

  virtual util::Status GetPendingEntries(
      std::vector<EntryHandle<Logged>>* entries) const = 0;

  virtual util::Status GetSequencedEntries(
      std::vector<EntryHandle<Logged>>* entries) const = 0;

  virtual util::Status GetSequencedEntry(const int64_t sequence_number,
                                         EntryHandle<Logged>* entry) const = 0;

  virtual util::Status AssignSequenceNumber(const int64_t sequence_number,
                                            EntryHandle<Logged>* entry) = 0;

  virtual util::Status SetClusterNodeState(
      const ct::ClusterNodeState& state) = 0;

  void WatchServingSTH(const ServingSTHCallback& cb);

  void WatchClusterNodeStates(const ClusterNodeStateCallback& cb);

 protected:
  void OnServingSTHUpdate(const Update<ct::SignedTreeHead>& update);

  void OnClusterNodeStatesUpdate(
      const std::vector<Update<ct::ClusterNodeState>>& updates);

 private:
  std::mutex watcher_mutex_;
  std::vector<ServingSTHCallback> sth_watchers_;
  std::vector<ClusterNodeStateCallback> cluster_node_watchers_;

  DISALLOW_COPY_AND_ASSIGN(ConsistentStore);
};


}  // namespace

#endif  // CERT_TRANS_LOG_CONSISTENT_STORE_H_
