#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintRemoveBenchmark, unsigned int, int)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    for (auto _ : state) {
        LockfreeHashTableUint<int> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (unsigned int key, int value) { ht.insert(key, value, tid);};
        initializeCollection(insert, keyUintCreator);
        auto remove = [&ht, tid] (unsigned int key) {ht.remove(key, tid);};
        state.SetIterationTime(measureOperation(remove));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintEraseBenchmark, unsigned int, int)(benchmark::State &state)
{
    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;
        auto insert = [&orderedMap] (unsigned int key, int value) { orderedMap[key] = value;};
        initializeCollection(insert, keyUintCreator);
        auto remove = [&orderedMap] (unsigned int key) {orderedMap.erase(key);};
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintEraseBenchmark, unsigned int, int)(benchmark::State &state)
{
    for (auto _ : state) {
        std::map<unsigned int, int> unorderedMap;
        auto insert = [&unorderedMap] (unsigned int key, int value) { unorderedMap[key] = value;};
        initializeCollection(insert, keyUintCreator);
        auto remove = [&unorderedMap] (unsigned int key) {unorderedMap.erase(key);};
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintRemoveBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintEraseBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintEraseBenchmark)->Apply(MapParameters)->UseRealTime();