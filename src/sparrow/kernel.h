#ifndef KERNELREGISTRY_H_
#define KERNELREGISTRY_H_

#include "sparrow/table.h"

#include "sparrow/util/common.h"
#include "sparrow/util/registry.h"
#include "sparrow/sparrow.pb.h"

#include <map>

namespace sparrow {

class TableBase;
class Worker;
class Table;

class Kernel {
public:
  typedef boost::scoped_ptr<Kernel> ScopedPtr;
  typedef std::map<string, string> ArgMap;

  // The table and shard being processed.
  int shard_id() const {
    return shard_;
  }

  int table_id() const {
    return table_id_;
  }

  Shard* current_shard() {
    return get_table(table_id())->shard(shard_id());
  }

  ArgMap& args() {
    return args_;
  }

  Table* get_table(int id);

  void init(Worker *w, int t, int s, const ArgMap& args) {
    w_ = w;
    shard_ = s;
    table_id_ = t;
    args_ = args;
  }

  virtual void run() = 0;

private:
  friend class Worker;
  friend class Master;

  Worker *w_;
  int shard_;
  int table_id_;
  ArgMap args_;
};

#define REGISTER_KERNEL(klass)\
  static TypeRegistry<Kernel>::Helper<klass> k_helper_ ## klass(#klass);


}
#endif /* KERNELREGISTRY_H_ */
