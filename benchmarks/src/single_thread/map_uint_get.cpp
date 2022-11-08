#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintSearchBenchmark, unsigned int, int)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    LockfreeHashTableUint<int> ht(2 * mapSize, threadCount);
    auto insert = [&ht, tid] (unsigned int key, int value) { ht.insert(key, value, tid);};
    initializeCollection(insert, keyUintCreator);
    bool found = false;
    auto search = [&ht, tid, &found] (unsigned int key) {ht.search(key, tid, found);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintBracketsBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::map<unsigned int, int> orderedMap;
    auto insert = [&orderedMap] (unsigned int key, int value) { orderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator);
    auto search = [&orderedMap] (unsigned int key) {int value = orderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintAtBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::map<unsigned int, int> orderedMap;
    auto insert = [&orderedMap] (unsigned int key, int value) { orderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator);
    auto search = [&orderedMap] (unsigned int key) {int value = orderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintBracketsBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::unordered_map<unsigned int, int> unorderedMap;
    auto insert = [&unorderedMap] (unsigned int key, int value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator);
    auto search = [&unorderedMap] (unsigned int key) {int value = unorderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintAtBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::unordered_map<unsigned int, int> unorderedMap;
    auto insert = [&unorderedMap] (unsigned int key, int value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator);
    auto search = [&unorderedMap] (unsigned int key) {int value = unorderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintSearchBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintAtBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintAtBenchmark)->Apply(MapParameters)->UseRealTime();
