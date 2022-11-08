#ifndef LOCKFREE_HASH_TABLE_UINT
#define LOCKFREE_HASH_TABLE_UINT

#include "lockfree_hash_table_templated.h"

template<class V>
class LockfreeHashTableUint : public LockfreeHashTableTemplated<unsigned int, V> {
public:
  LockfreeHashTableUint(int capacity, int thread_count)
  : LockfreeHashTableTemplated<unsigned int, V>(capacity, thread_count)
  {}

  virtual ~LockfreeHashTableUint()
  {}
  
private:

  virtual int createHash(unsigned int &key)
  {
    return key;
  }
};
#endif