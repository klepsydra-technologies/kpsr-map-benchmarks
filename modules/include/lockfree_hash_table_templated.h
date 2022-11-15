#ifndef LOCKFREE_HASH_TABLE_TEMPLATED
#define LOCKFREE_HASH_TABLE_TEMPLATED

#include <cstdint>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <functional>

#define MAX_BUF 256
#define THRESHOLD   50
#define R           25
#define MAX_BUFSIZE 128
#define HP_COUNT    2

template <class K, class V>
struct Hash_entry_templated {
  K key;
  V val;
};

// Alternate count_ptr definition using unused bits
//typedef Hash_entry_templated* Count_ptr_templated;
template <class K, class V>
using Count_ptr_templated = Hash_entry_templated<K,V>*;

// Inline bit twiddling functions
template <class K, class V>
inline Count_ptr_templated<K,V> make_pointer(Hash_entry_templated<K,V>* e, uint16_t count) {
  return (Count_ptr_templated<K,V>)((((uint64_t)count) << 48) | ((uint64_t)e & 0xFFFFFFFFFFFF));
}

template <class K, class V>
inline Hash_entry_templated<K,V>* get_pointer(Count_ptr_templated<K,V> ptr) {
  return (Hash_entry_templated<K,V>*)((uint64_t)ptr & 0xFFFFFFFFFFFE);
}

template <class K, class V>
inline uint16_t get_counter(Count_ptr_templated<K,V> ptr) { 
  return (uint16_t)(((uint64_t)ptr >> 48) & 0xFFFF);
}

template <class K, class V>
inline bool get_marked(Hash_entry_templated<K,V> *ent) {
  return ((uint64_t)ent & 1) == 1;
}

template <class K, class V>
inline Hash_entry_templated<K,V> *set_marked(Hash_entry_templated<K,V> *ent, bool marked) {
  return marked ? (Hash_entry_templated<K,V>*)((uint64_t)ent | 1) 
                : (Hash_entry_templated<K,V>*)((uint64_t)ent & (~1));
}
//

enum Find_result_templated { FIRST_templated, SECOND_templated, NIL_templated };

template <class K, class V>
class LockfreeHashTableTemplated {
public:
  LockfreeHashTableTemplated(int capacity, int thread_count)
  {
    size1 = capacity / 2;
    size2 = capacity - size1;
    
    vectorOfKeys = std::vector<std::vector<K>>(thread_count);
  
    table[0] = new Count_ptr_templated<K,V>[size1]();
    table[1] = new Count_ptr_templated<K,V>[size2]();

    hp_rec.reserve(thread_count);
    rlist.reserve(thread_count);
    rcount.reserve(thread_count);

    for (int i = 0; i < thread_count; i++)
    {
      hp_rec[i][0] = NULL;
      hp_rec[i][1] = NULL;
      rcount[i]    = 0;
    }
  }

  virtual ~LockfreeHashTableTemplated()
  {
    for (int i = 0; i < size1; i++)
    {
      Hash_entry_templated<K,V>* node = get_pointer<K,V>(table[0][i]);
      if (node != NULL)
        delete node;
    }
    
    for (int i = 0; i < size2; i++)
    {
      Hash_entry_templated<K,V>* node = get_pointer<K,V>(table[1][i]);
      if (node != NULL)
        delete node;
    }
    
    delete table[0];
    delete table[1];
  }
  
  void rehash_templated() {
    return;
  }

  V& search(K& key, int tid, bool& found)
  {
    int hashKey = createHash(key);
    int h1 = hash1(hashKey);
    int h2 = hash2(hashKey);

    while (true) {
      //std::cout << "search inf loop " << key << std::endl;
      Count_ptr_templated<K,V> ptr1 = table[0][h1]; 
      Hash_entry_templated<K,V> *e1 = get_pointer<K,V>(ptr1);
      
      hp_rec[tid][0] = e1;
      if (ptr1 != table[0][h1])
        continue;

      int ts1 = get_counter<K,V>(ptr1);

      if (e1 && e1->key == key) {
        found = true;
        return e1->val;
      }

      Count_ptr_templated<K,V> ptr2 = table[1][h2];
      Hash_entry_templated<K,V> *e2 = get_pointer<K,V>(ptr2);

      hp_rec[tid][0] = e2;
      if (ptr2 != table[1][h2])
        continue;

      int ts2 = get_counter<K,V>(ptr2);

      if (e2 && e2->key == key) {
        found = true;
        return e2->val;
      }
      int ts1x = get_counter<K,V>(table[0][h1]);
      int ts2x = get_counter<K,V>(table[1][h2]);

      if (check_counter(ts1, ts2, ts1x, ts2x)) {
        continue;
      } else {
        found = false;
        return _defaultValue;
      }
    }

    found = false;
    return _defaultValue;
  }

  void insert(K& key, V val, int tid)
  {
    Count_ptr_templated<K,V> ptr1, ptr2;

    Hash_entry_templated<K,V> *new_node = new Hash_entry_templated<K,V>();
    new_node->key = key;
    new_node->val = val;
    
    int hashKey = createHash(key);
    int h1 = hash1(hashKey);
    int h2 = hash2(hashKey);


    while (true) {
      //std::cout << "Inserting " << key << std::endl;
      Find_result_templated result = find(key, ptr1, ptr2, tid);

      if (result == FIRST_templated) {
        get_pointer<K,V>(ptr1)->val = val; 
        vectorOfKeys[tid].push_back(key);
        return;
      }

      if (result == SECOND_templated) {
        get_pointer<K,V>(ptr2)->val = val;
        vectorOfKeys[tid].push_back(key);
        return;
      }

      if (!get_pointer<K,V>(ptr1)) { 
        if (!__sync_bool_compare_and_swap(
              &table[0][h1], ptr1, make_pointer<K,V>(new_node, get_counter<K,V>(ptr1)))) {
          continue; 
        }
        vectorOfKeys[tid].push_back(key);
        return;
      }

      if (!get_pointer<K,V>(ptr2)) { 
        if (!__sync_bool_compare_and_swap(
              &table[1][h2], ptr2, make_pointer<K,V>(new_node, get_counter<K,V>(ptr2)))) {
          continue; 
        }
        vectorOfKeys[tid].push_back(key);
        return;
      }

      if (relocate(0, h1, tid)) {
        continue;
      } else {
        rehash_templated();
        vectorOfKeys[0].push_back(key);
        return;
      }
    }
  }

  void remove(K& key, int tid)
  {
    int hashKey = createHash(key);
    int h1 = hash1(hashKey);
    int h2 = hash2(hashKey);

    Count_ptr_templated<K,V> e1;
    Count_ptr_templated<K,V> e2;

    while (true) {
      //std::cout << "remove inf loop" << std::endl;
      Find_result_templated ret = find(key, e1, e2, tid);

      if (ret == NIL_templated) return;

      if (ret == FIRST_templated) {
        if (__sync_bool_compare_and_swap(
              &table[0][h1], e1, make_pointer<K,V>(nullptr, get_counter<K,V>(e1)))) {
          retire_node(get_pointer<K,V>(e1), tid);
          removeKeyFromVector(key, tid);
          return;
        }
      } else if (ret == SECOND_templated) {
        if (table[0][h1] != e1) 
          continue;
        if (__sync_bool_compare_and_swap(
              &table[1][h2], e2, make_pointer<K,V>(nullptr, get_counter<K,V>(e2)))) {
          retire_node(get_pointer<K,V>(e2), tid);
          removeKeyFromVector(key, tid);
          return;
        }
      }
    } 
  }

void removeIf(int tid, std::function<bool(const V &value)> callback) {
  std::vector<K> keys = vectorOfKeys[tid];
  bool found = false;
  for (auto& k : keys) {
    V& val = search(k, tid, found);
    if (found) {
      if (callback(val)) {
        remove(k, tid);
      }
    }
  }
}

private:
  Count_ptr_templated<K,V> *table[2];
  int size1;
  int size2;
  
  std::vector<std::vector<K>> vectorOfKeys;

  std::vector<std::array<Hash_entry_templated<K,V>*, MAX_BUF>>   rlist;
  std::vector<int>                                       rcount;
  std::vector<std::array<Hash_entry_templated<K,V>*, 2>>         hp_rec;

  V _defaultValue;
  
  void removeKeyFromVector(K &key, int tid) {
    auto elementToBeRemoved = std::find_if(
      vectorOfKeys[tid].begin(),
      vectorOfKeys[tid].end(),
      [&key](K currentKey) {
        return currentKey == key;
      });
    if (elementToBeRemoved != vectorOfKeys[tid].end()) {
      vectorOfKeys[tid].erase(elementToBeRemoved);
    }
  }

  virtual int createHash(K &key)
  {
    std::cout << "general specialization" << std::endl;
    return std::hash<K>{}(key);
  }

  int hash1(int hashKey)
  {
    int c2=0x27d4eb2d; // a prime or an odd constant
    hashKey = (hashKey ^ 61) ^ (hashKey >> 16);
    hashKey = hashKey + (hashKey << 3);
    hashKey = hashKey ^ (hashKey >> 4);
    hashKey = hashKey * c2;
    hashKey = hashKey ^ (hashKey >> 15);
    int retVal = hashKey % size1;
    return retVal; //TODO
  }

  int hash2(int hashKey)
  {
    hashKey = ((hashKey >> 16) ^ hashKey) * 0x45d9f3b;
    hashKey = ((hashKey >> 16) ^ hashKey) * 0x45d9f3b;
    hashKey = (hashKey >> 16) ^ hashKey;
    int retVal = hashKey % size2;
    return retVal;
  }

  bool check_counter(int ts1, int ts2, int ts1x, int ts2x)
  {
    return (ts1x >= ts1 + 2) && (ts2x >= ts2 + 2) && (ts2x >= ts1 + 3);
  }

  Find_result_templated find(K &key, Count_ptr_templated<K,V> &ptr1, Count_ptr_templated<K,V> &ptr2, int tid)
  {
    int hashKey = createHash(key);
    int h1 = hash1(hashKey);
    int h2 = hash2(hashKey);
    Find_result_templated result = (Find_result_templated)-1;

    while (true) {
      //std::cout << "Find inf loop" << std::endl;
      ptr1 = table[0][h1];

      int ts1 = get_counter<K,V>(ptr1);

      hp_rec[tid][0] = get_pointer<K,V>(ptr1);
      if (get_pointer<K,V>(ptr1) != get_pointer<K,V>(table[0][h1]))
        continue;

      if (get_pointer<K,V>(ptr1)) {
        if (get_marked<K,V>(ptr1)) {
          help_relocate(0, h1, false, tid);
          continue; 
        }

        if (get_pointer<K,V>(ptr1)->key == key) 
          result = FIRST_templated; 
      }

      ptr2 = table[1][h2];
      int ts2 = get_counter<K,V>(ptr2);

      hp_rec[tid][1] = get_pointer<K,V>(ptr2);
      if (get_pointer<K,V>(ptr2) != get_pointer<K,V>(table[1][h2]))
        continue;

      if (get_pointer<K,V>(ptr2)) {
        if (get_marked<K,V>(ptr2)) {
          help_relocate(1, h2, false, tid);
          continue; 
        }

        if (get_pointer<K,V>(ptr2)->key == key) {
          if (result == FIRST_templated) {
            del_dup(h1, ptr1, h2, ptr2, tid);
          } else {
            result = SECOND_templated;
          }
        }
      }

      if (result == FIRST_templated || result == SECOND_templated) {
        return result;
      }

      ptr1 = table[0][h1];
      ptr2 = table[1][h2];

      if (check_counter(ts1, ts2, get_counter<K,V>(ptr1), get_counter<K,V>(ptr2))) {
        continue;
      } else {
        return NIL_templated;
      }
    }
  }

  bool relocate(int which, int index, int tid)
  {
    try_again:
      int  route[THRESHOLD];
      Count_ptr_templated<K,V> pptr   = NULL;
      int  pre_idx     = 0;
      int  start_level = 0;
      int  tbl         = which;
      int  idx         = index;


    path_discovery:
      bool found = false;
      int depth = start_level;
      do
      {
        Count_ptr_templated<K,V> ptr1 = table[tbl][idx];
        
        while (get_marked<K,V>(ptr1))
        {
          help_relocate(tbl, idx, false, tid);
          ptr1 = table[tbl][idx];
        }

        Hash_entry_templated<K,V>* e1 = get_pointer<K,V>(ptr1);
        Hash_entry_templated<K,V>* p1 = get_pointer<K,V>(pptr);
        hp_rec[tid][0] = e1;
        if (e1 != get_pointer<K,V>(table[tbl][idx]))
          goto try_again;
        /*
        if (p1 && e1 && e1->key == p1->key)
        {
          if (tbl == 0)
            del_dup(idx, ptr1, pre_idx, pptr, tid);
          else
            del_dup(pre_idx, pptr, idx, ptr1, tid);
        }
        */
        if (e1 != nullptr)
        {
          route[depth] = idx;
          K key = e1->key; 
          pptr    = ptr1;
          pre_idx = idx;
          tbl     = 1 - tbl;
          int hashKey = createHash(key);
          idx     = (tbl == 0) ? hash1(hashKey) : hash2(hashKey); 
        }
        else
        {
          found = true;
        }
      } while (!found && ++depth < THRESHOLD);

      if (found)
      {
        tbl = 1 - tbl;
        for (int i = depth-1; i >= 0; i--, tbl = 1 - tbl)
        {
          idx = route[i];
          Count_ptr_templated<K,V> ptr1 = table[tbl][idx];
          /*
          hp_rec[tid][0] = get_pointer<K,V>(ptr1);
          if (get_pointer<K,V>(ptr1) != get_pointer<K,V>(table[tbl][idx]))
            goto try_again;
          */
          if (get_marked<K,V>(ptr1))
          {
            help_relocate(tbl, idx, false, tid);
            hp_rec[tid][0] = table[tbl][idx];
            ptr1 = hp_rec[tid][0];
            /*
            if (get_pointer<K,V>(ptr1) != get_pointer<K,V>(table[tbl][idx]))
              goto try_again;
            */
          }

          Hash_entry_templated<K,V>* e1 = get_pointer<K,V>(ptr1);
          if (e1 == nullptr)
            continue;
          
          int hashKey = createHash(e1->key);
          int dest_idx = (tbl == 0) ? hash2(hashKey) : hash1(hashKey);
          Count_ptr_templated<K,V> ptr2 = table[1-tbl][dest_idx];
          Hash_entry_templated<K,V>* e2 = get_pointer<K,V>(ptr2);

          if (e2 != nullptr)
          {
            start_level = i + 1;
            idx = dest_idx;
            tbl = 1 - tbl;
            goto path_discovery;
          }
          help_relocate(tbl, idx, true, tid);
        }
      }

      return found;
  }
  void help_relocate(int which, int index, bool initiator, int tid)
  {
    while (1)
    {
      //std::cout << "help_relocate inf loop" << std::endl;
      Count_ptr_templated<K,V> ptr1 = table[which][index];
      Hash_entry_templated<K,V>* src = get_pointer<K,V>(ptr1);
      hp_rec[tid][0] = src;
      if (ptr1 != table[which][index])
        continue;

      while (initiator && !get_marked<K,V>(ptr1))
      {
        //std::cout << "help_relocate mark inf loop" << std::endl;
        if (src == nullptr)
          return;

        __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                    set_marked<K,V>(ptr1, 1));
        ptr1 = table[which][index];
        hp_rec[tid][0] = ptr1;
        if (ptr1 != table[which][index])
          continue;
        src  = get_pointer<K,V>(ptr1);
      }

      if (!get_marked<K,V>(ptr1))
        return;

      int hashKey = createHash(src->key);
      int hd = ((1 - which) == 0) ? hash1(hashKey) : hash2(hashKey);
      Count_ptr_templated<K,V> ptr2 = table[1-which][hd];
      Hash_entry_templated<K,V>* dst = get_pointer<K,V>(ptr2);
      hp_rec[tid][1] = dst;
      if (ptr2 != table[1-which][hd])
        continue;

      uint16_t ts1 = get_counter<K,V>(ptr1);
      uint16_t ts2 = get_counter<K,V>(ptr2);

      if (dst == nullptr)
      {
        int nCnt = ts1 > ts2 ? ts1 + 1 : ts2 + 1;
        
        if (ptr1 != table[which][index])
          continue;
        
        if (__sync_bool_compare_and_swap(&table[1-which][hd], ptr2, 
                                        make_pointer<K,V>(src, nCnt)))
        {
          __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                      make_pointer<K,V>(nullptr, ts1+1));
          return;
        }
      }

      if (src == dst)
      {
        __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                    make_pointer<K,V>(nullptr, ts1+1));
        return;
      }

      __sync_bool_compare_and_swap(&table[which][index], ptr1, 
                                  make_pointer<K,V>(set_marked<K,V>(src, 0), ts1+1));
      return;
      
    }
  }
  void del_dup(int idx1, Count_ptr_templated<K,V> ptr1, int idx2, Count_ptr_templated<K,V> ptr2, int tid)
  {
    hp_rec[tid][0] = ptr1;
    hp_rec[tid][1] = ptr2;
    if (ptr1 != table[0][idx1] && ptr2 != table[1][idx2])
      return;
    if (get_pointer<K,V>(ptr1)->key != get_pointer<K,V>(ptr2)->key)
      return;

    __sync_bool_compare_and_swap(&table[1][idx2], ptr2, 
                                make_pointer<K,V>(nullptr, get_counter<K,V>(ptr2)));
  }

  void retire_node(Hash_entry_templated<K,V>* node, int tid)
  {
    rlist[tid][rcount[tid]] = node;
    rcount[tid]++;

    if (rcount[tid] > R)
      scan(tid);
  }
  void scan(int tid)
  {
    // Stage 1
    int size = 0;
    std::vector<Hash_entry_templated<K,V>*> plist;
    for (int i = 0; i < hp_rec.size(); i++)
    {
      for (int j = 0; j < hp_rec[i].size(); j++)
      {
        Hash_entry_templated<K,V>* hptr = hp_rec[i][j];
        if (hptr != NULL)
        {
          plist.push_back(hptr);
        }
      }
    }

    // Stage 2
    int n = rcount[tid];
    rcount[tid] = 0;
    for (int i = 0; i < n; i++)
    {
      if (std::find(plist.begin(), plist.end(), rlist[tid][i]) != plist.end())
      {
        rlist[tid][rcount[tid]] = rlist[tid][i];
        rcount[tid]++;
      }
      else
      {
        //printf("freed %p\n", rlist[tid][i]);
        delete rlist[tid][i];
      }
    }
  }

};
#endif