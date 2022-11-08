#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintVectorRemoveBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    for (auto _ : state) {
        LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (unsigned int key, std::vector<int> value) { ht.insert(key, value, tid);};
        initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
        auto remove = [&ht, tid] (unsigned int key) {ht.remove(key, tid);};
        state.SetIterationTime(measureOperation(remove));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorEraseBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;
        auto insert = [&orderedMap] (unsigned int key, std::vector<int> value) { orderedMap[key] = value;};
        initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
        auto remove = [&orderedMap] (unsigned int key) { orderedMap.erase(key);};
        state.SetIterationTime(measureOperation(remove));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorEraseBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    for (auto _ : state) {
        std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap] (unsigned int key, std::vector<int> value) { unorderedMap[key] = value;};
        initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
        auto remove = [&unorderedMap] (unsigned int key) { unorderedMap.erase(key);};
        state.SetIterationTime(measureOperation(remove));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintVectorRemoveBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorEraseBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorEraseBenchmark)->Apply(MapParameters)->UseRealTime();