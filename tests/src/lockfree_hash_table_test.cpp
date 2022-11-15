#include <cassert>
#include <lockfree_hash_table.h>
#include <lockfree_hash_table_string.h>
#include <lockfree_hash_table_templated.h>
#include <lockfree_hash_table_uint.h>
#include <iostream>

int main() {

    std::cout << "Tests Init" << std::endl;

    {
        int mapSize = 32;
        int threadCount = 1;
        int tid = threadCount - 1;

        Lockfree_hash_table ht(2 * mapSize, threadCount);
        for (int i = 0; i < mapSize; i++) {
            ht.insert(i, i+10, tid);
        }

        for (int i = 0; i < mapSize; i++) {
            std::pair<int, bool> value = ht.search(i, tid);
            assert(value.first == i+10);
            assert(value.second == true);
        }
    }

    {
        int mapSize = 32;
        int threadCount = 10;

        Lockfree_hash_table ht(2 * mapSize, threadCount);
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                int valueToStore = i + 10 + mapSize * tid;
                ht.insert(i, valueToStore, tid);
                
                std::pair<int, bool> value = ht.search(i, tid);
                int expectedValue = i + 10 + mapSize * tid;
                assert(value.first == expectedValue);
                assert(value.second == true);
            }
        }

        int lastTid = threadCount - 1;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                std::pair<int, bool> value = ht.search(i, tid);
                
                int expectedValue = i + 10 + mapSize * lastTid;
                assert(value.first == expectedValue);
                assert(value.second == true);
            }
        }
    }

    {
        int mapSize = 32;
        int threadCount = 10;

        LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
        std::vector<std::string> keys(mapSize);
        bool found = false;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                int valueToStore = i + 10 + mapSize * tid;
                keys[i] = std::to_string(i);
                ht.insert(keys[i], valueToStore, tid);
                
                found = false;
                int value = ht.search(keys[i], tid, found);
                int expectedValue = i + 10 + mapSize * tid;
                assert(value == expectedValue);
                assert(found == true);
            }
        }

        int lastTid = threadCount - 1;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                found = false;
                int value = ht.search(keys[i], tid, found);
                
                int expectedValue = i + 10 + mapSize * lastTid;
                assert(value == expectedValue);
                assert(found == true);
            }
        }
    }

    {
        int mapSize = 32;
        int threadCount = 10;

        LockfreeHashTableUint<int> ht(2 * mapSize, threadCount);
        std::vector<unsigned int> keys(mapSize);
        bool found = false;
        for (int tid = 0; tid < threadCount; tid++) {
            for (unsigned int i = 0; i < mapSize; i++) {
                int valueToStore = i + 10 + mapSize * tid;
                keys[i] = i;
                ht.insert(keys[i], valueToStore, tid);
                
                found = false;
                int value = ht.search(i, tid, found);
                int expectedValue = i + 10 + mapSize * tid;
                assert(value == expectedValue);
                assert(found == true);
            }
        }

        int lastTid = threadCount - 1;
        for (int tid = 0; tid < threadCount; tid++) {
            for (unsigned int i = 0; i < mapSize; i++) {
                found = false;
                int value = ht.search(keys[i], tid, found);
                
                int expectedValue = i + 10 + mapSize * lastTid;
                assert(value == expectedValue);
                assert(found == true);
            }
        }
    }

    {
        int mapSize = 32;
        int threadCount = 10;

        LockfreeHashTableString<std::string> ht(2 * mapSize, threadCount);
        std::vector<std::string> keys(mapSize);
        bool found = false;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                std::string valueToStore = std::to_string(i + 10 + mapSize * tid);
                keys[i] = std::to_string(i);
                ht.insert(keys[i], valueToStore, tid);
                
                found = false;
                std::string value = ht.search(keys[i], tid, found);
                std::string expectedValue = std::to_string(i + 10 + mapSize * tid);
                assert(expectedValue == value);
                assert(expectedValue == value);
                assert(found == true);
            }
        }

        int lastTid = threadCount - 1;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                found = false;
                std::string value = ht.search(keys[i], tid, found);
                
                std::string expectedValue = std::to_string(i + 10 + mapSize * lastTid);
                assert(value == expectedValue);
                assert(found == true);
            }
        }
    }
    
    {
        int mapSize = 32;
        int vectorSize = 16;
        int threadCount = 1;
        int tid = threadCount - 1;
    
        LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
        std::vector<std::string> keys(mapSize);
        bool found = false;
        for (int i = 0; i < mapSize; i++) {
            std::vector<int> vectorToStore(vectorSize, i + 10);
            keys[i] = std::to_string(i);
            ht.insert(keys[i], vectorToStore, tid);
        }

        for (int i = 0; i < mapSize; i++) {
            found = false;
            std::vector<int> value = ht.search(keys[i], tid, found);

            std::vector<int> expectedVector(vectorSize, i+10);
            assert(value.size() == vectorSize);
            assert(found == true);
            for (size_t j = 0; j < vectorSize; j++) {
                assert(expectedVector[j] == value[j]);
            }
        }
    }

    {
        int mapSize = 32;
        int vectorSize = 16;
        int threadCount = 10;

        LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
        std::vector<std::string> keys(mapSize);
        bool found = false;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                std::vector<int> vectorToStore(vectorSize, i + 10 + mapSize * tid);
                keys[i] = std::to_string(i);
                ht.insert(keys[i], vectorToStore, tid);
                
                found = false;
                std::vector<int> value = ht.search(keys[i], tid, found);
                assert(value.size() == vectorSize);
                assert(found == true);
                for (size_t j = 0; j < vectorSize; j++) {
                    assert(vectorToStore[j] == value[j]);
                }
            }
        }

        int lastTid = threadCount - 1;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                found = false;
                std::vector<int> value = ht.search(keys[i], tid, found);
                std::vector<int> expectedVector(vectorSize, i + 10 + mapSize * lastTid);
                assert(value.size() == vectorSize);
                assert(found == true);
                for (size_t j = 0; j < vectorSize; j++) {
                    assert(expectedVector[j] == value[j]);
                }
            }
        }
    }

    {
        int mapSize = 32;
        int vectorSize = 16;
        int threadCount = 1;
        int tid = threadCount - 1;
    
        LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
        std::vector<unsigned int> keys(mapSize);
        bool found = false;
        for (unsigned int i = 0; i < mapSize; i++) {
            std::vector<int> vectorToStore(vectorSize, i + 10);
            keys[i] = i;
            ht.insert(keys[i], vectorToStore, tid);
        }

        for (int i = 0; i < mapSize; i++) {
            found = false;
            std::vector<int> value = ht.search(keys[i], tid, found);
            std::vector<int> expectedVector(vectorSize, i+10);
            assert(value.size() == vectorSize);
            assert(found == true);
            for (size_t j = 0; j < vectorSize; j++) {
                assert(expectedVector[j] == value[j]);
            }
        }
    }
    
    {
        int mapSize = 32;
        int vectorSize = 16;
        int threadCount = 1;
        int tid = threadCount - 1;
    
        LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
        std::vector<std::string> keys(mapSize);
        bool found = false;
        for (int i = 0; i < mapSize; i++) {
            int valueToStore = i + 10;
            keys[i] = std::to_string(i);
            ht.insert(keys[i], valueToStore, tid);
        }

        for (int i = 0; i < mapSize; i++) {
            found = false;
            int value = ht.search(keys[i], tid, found);

            int expectedValue = i+10;
            assert(found == true);
            assert(expectedValue == value);
        }

        int listenerId = 10;
        std::function<bool(const int)> callback = [listenerId](const int &value) {
            if (listenerId == value) {
                return true;
            } else {
                return false;
            }
        };
        for (int i = 0; i < mapSize; i++) {
            ht.removeIf(tid, callback);
        }
    }

    std::cout << "Tests Ok" << std::endl;
}