#include <cassert>
#include <lockfree_hash_table.h>
#include <lockfree_hash_table_string.h>
#include <lockfree_hash_table_templated.h>
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

    //Test 3:
    {
        int mapSize = 32;
        int threadCount = 1;
        int tid = threadCount - 1;

        LockfreeHashTableString ht(2 * mapSize, threadCount);
        for (int i = 0; i < mapSize; i++) {
            ht.insert(std::to_string(i), i+10, tid);
        }

        for (int i = 0; i < mapSize; i++) {
            std::pair<int, bool> value = ht.search(std::to_string(i), tid);
            int expectedValue = i + 10;
            assert(value.first == expectedValue);
            assert(value.second == true);
        }
    }
    
    //Test 4:
    {
        int mapSize = 32;
        int threadCount = 10;

        LockfreeHashTableString ht(2 * mapSize, threadCount);
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
    
    //Test 5:
    {
        int mapSize = 32;
        int threadCount = 10;

        std::cout << "Test 5: <std::string, int>" << std::endl;
        LockfreeHashTableTemplated<std::string, int> ht(2 * mapSize, threadCount);
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

        std::cout << "Test 6: <unsigned int, int>" << std::endl;
        LockfreeHashTableTemplated<unsigned int, int> ht(2 * mapSize, threadCount);
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

        std::cout << "Test 7: <size_t, int>" << std::endl;
        LockfreeHashTableTemplated<size_t, int> ht(2 * mapSize, threadCount);
        for (int tid = 0; tid < threadCount; tid++) {
            for (size_t i = 0; i < mapSize; i++) {
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
            for (size_t i = 0; i < mapSize; i++) {
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

        std::cout << "Test 7: <std::string, std::string>" << std::endl;
        LockfreeHashTableTemplated<std::string, std::string> ht(2 * mapSize, threadCount);
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
    
        std::cout << "Test 8: <std::string, std::vector<int>>" << std::endl;
        LockfreeHashTableTemplated<std::string, std::vector<int>> ht(2 * mapSize, threadCount);
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

        std::cout << "Test 9: <std::string, std::vector<int>>" << std::endl;
        LockfreeHashTableTemplated<std::string, std::vector<int>> ht(2 * mapSize, threadCount);
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


}