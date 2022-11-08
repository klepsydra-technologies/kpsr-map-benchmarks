#ifndef LOCKFREE_HASH_TABLE_STRING
#define LOCKFREE_HASH_TABLE_STRING

#define MAX_BUF 256

#include <vector>
#include <array>

struct Hash_entry_string {
  std::string key;
  int val;
};

// Alternate count_ptr definition using unused bits
typedef Hash_entry_string* Count_ptr_string;

enum Find_result_string { FIRST_STRING, SECOND_STRING, NIL_STRING };

class LockfreeHashTableString {
public:
  LockfreeHashTableString(int capacity, int thread_count);
  ~LockfreeHashTableString();
  
  std::pair<int, bool> search(std::string key, int tid);
  void                 insert(std::string key, int val, int tid);
  void                 remove(std::string key, int tid);

private:
  Count_ptr_string *table[2];  
  int size1;
  int size2;

  std::vector<std::array<Hash_entry_string*, MAX_BUF>>   rlist;
  std::vector<int>                                       rcount;
  std::vector<std::array<Hash_entry_string*, 2>>         hp_rec;

  int createHash(std::string key);
  int hash1(int hashKey);
  int hash2(int hashKey);
  bool check_counter(int ts1, int ts2, int ts1x, int ts2x);
  Find_result_string find(std::string key, Count_ptr_string &ptr1, Count_ptr_string &ptr2, int tid);
  bool relocate(int which, int index, int tid);
  void help_relocate(int which, int index, bool initiator, int tid);
  void del_dup(int idx1, Count_ptr_string ptr1, int idx2, Count_ptr_string ptr2, int tid);

  void retire_node(Hash_entry_string* node, int tid);
  void scan(int tid);

};
#endif