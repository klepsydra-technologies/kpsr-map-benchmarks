#ifndef LOCKFREE_HASH_TABLE_STRING
#define LOCKFREE_HASH_TABLE_STRING

#include "lockfree_hash_table_templated.h"

template<class V>
class LockfreeHashTableString : public LockfreeHashTableTemplated<std::string, V> {
public:
  LockfreeHashTableString(int capacity, int thread_count)
  : LockfreeHashTableTemplated<std::string, V>(capacity, thread_count)
  {}

  virtual ~LockfreeHashTableString()
  {}
  
private:
  virtual int createHash(std::string &key)
  {
    return std::hash<std::string>{}(key);
  }
};
#endif