#include <cassert>
#include <lockfree_hash_table.h>
#include <lockfree_hash_table_string.h>
#include <lockfree_hash_table_templated.h>
#include <lockfree_hash_table_uint.h>
#include <iostream>

int main() {

    //Test 1:
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

    //Test 2:
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
    
    //Test 5:
    {
        int mapSize = 32;
        int threadCount = 10;

        LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                int valueToStore = i + 10 + mapSize * tid;
                ht.insert(std::to_string(i), valueToStore, tid);
                
                std::pair<int, bool> value = ht.search(std::to_string(i), tid);
                int expectedValue = i + 10 + mapSize * tid;
                assert(value.first == expectedValue);
                assert(value.second == true);
            }
        }

        int lastTid = threadCount - 1;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                std::pair<int, bool> value = ht.search(std::to_string(i), tid);
                
                int expectedValue = i + 10 + mapSize * lastTid;
                assert(value.first == expectedValue);
                assert(value.second == true);
            }
        }
    }

    //Test 6:
    {
        int mapSize = 32;
        int threadCount = 10;

        LockfreeHashTableUint<int> ht(2 * mapSize, threadCount);
        for (int tid = 0; tid < threadCount; tid++) {
            for (unsigned int i = 0; i < mapSize; i++) {
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
            for (unsigned int i = 0; i < mapSize; i++) {
                std::pair<int, bool> value = ht.search(i, tid);
                
                int expectedValue = i + 10 + mapSize * lastTid;
                //std::cout << "value.first " << value.first << std::endl;
                assert(value.first == expectedValue);
                assert(value.second == true);
            }
        }
    }

    //Test 7:
    {
        int mapSize = 32;
        int threadCount = 10;

        LockfreeHashTableString<std::string> ht(2 * mapSize, threadCount);
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                std::string valueToStore = std::to_string(i + 10 + mapSize * tid);
                ht.insert(std::to_string(i), valueToStore, tid);
                
                std::pair<std::string, bool> value = ht.search(std::to_string(i), tid);
                std::string expectedValue = std::to_string(i + 10 + mapSize * tid);
                assert(expectedValue == value.first);
                assert(expectedValue == value.first);
                assert(value.second == true);
            }
        }

        int lastTid = threadCount - 1;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                std::pair<std::string, bool> value = ht.search(std::to_string(i), tid);
                
                std::string expectedValue = std::to_string(i + 10 + mapSize * lastTid);
                assert(value.first == expectedValue);
                assert(value.second == true);
            }
        }
    }
    
    //Test 8:
    {
        int mapSize = 32;
        int vectorSize = 16;
        int threadCount = 1;
        int tid = threadCount - 1;
    
        LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
        for (int i = 0; i < mapSize; i++) {
            std::vector<int> vectorToStore(vectorSize, i + 10);
            ht.insert(std::to_string(i), vectorToStore, tid);
        }

        for (int i = 0; i < mapSize; i++) {
            std::pair<std::vector<int>, bool> value = ht.search(std::to_string(i), tid);

            std::vector<int> expectedVector(vectorSize, i+10);
            assert((value.first).size() == vectorSize);
            assert(value.second == true);
            for (size_t j = 0; j < vectorSize; j++) {
                assert(expectedVector[j] == value.first[j]);
            }
        }
    }

    //Test 9:
    {
        int mapSize = 32;
        int vectorSize = 16;
        int threadCount = 10;

        LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                std::vector<int> vectorToStore(vectorSize, i + 10 + mapSize * tid);
                ht.insert(std::to_string(i), vectorToStore, tid);
                
                std::pair<std::vector<int>, bool> value = ht.search(std::to_string(i), tid);
                assert((value.first).size() == vectorSize);
                assert(value.second == true);
                for (size_t j = 0; j < vectorSize; j++) {
                    assert(vectorToStore[j] == value.first[j]);
                }
            }
        }

        int lastTid = threadCount - 1;
        for (int tid = 0; tid < threadCount; tid++) {
            for (int i = 0; i < mapSize; i++) {
                std::pair<std::vector<int>, bool> value = ht.search(std::to_string(i), tid);
                
                std::vector<int> expectedVector(vectorSize, i + 10 + mapSize * lastTid);
                assert((value.first).size() == vectorSize);
                assert(value.second == true);
                for (size_t j = 0; j < vectorSize; j++) {
                    assert(expectedVector[j] == value.first[j]);
                }
            }
        }
    }

    //Test 10:
    {
        int mapSize = 32;
        int vectorSize = 16;
        int threadCount = 1;
        int tid = threadCount - 1;
    
        LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
        for (unsigned int i = 0; i < mapSize; i++) {
            std::vector<int> vectorToStore(vectorSize, i + 10);
            ht.insert(i, vectorToStore, tid);
        }

        for (int i = 0; i < mapSize; i++) {
            std::pair<std::vector<int>, bool> value = ht.search(i, tid);

            std::vector<int> expectedVector(vectorSize, i+10);
            assert((value.first).size() == vectorSize);
            assert(value.second == true);
            for (size_t j = 0; j < vectorSize; j++) {
                assert(expectedVector[j] == value.first[j]);
            }
        }
    }


}