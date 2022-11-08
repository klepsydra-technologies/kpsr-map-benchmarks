#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintInsertBenchmark, unsigned int, int)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        LockfreeHashTableUint<int> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (unsigned int key, int value) { ht.insert(key, value, tid);};
        state.SetIterationTime(measureOperation(insert, valueIntCreator));        
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintInsertBracketsBenchmark, unsigned int, int)(benchmark::State &state)
{
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;
        auto insert = [&orderedMap] (unsigned int key, int value) { orderedMap[key] = value;};
        state.SetIterationTime(measureOperation(insert, valueIntCreator));   
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintInsertBenchmark, unsigned int, int)(benchmark::State &state)
{
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;
        auto insert = [&orderedMap] (unsigned int key, int value) { orderedMap.insert({key, value});};
        state.SetIterationTime(measureOperation(insert, valueIntCreator));   
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintInsertBracketsBenchmark, unsigned int, int)(benchmark::State &state)
{
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::unordered_map<unsigned int, int> unorderedMap;
        auto insert = [&unorderedMap] (unsigned int key, int value) { unorderedMap[key] = value;};
        state.SetIterationTime(measureOperation(insert, valueIntCreator));   
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintInsertBenchmark, unsigned int, int)(benchmark::State &state)
{
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::unordered_map<unsigned int, int> unorderedMap;
        auto insert = [&unorderedMap] (unsigned int key, int value) { unorderedMap.insert({key, value});};
        state.SetIterationTime(measureOperation(insert, valueIntCreator));   
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintInsertBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintInsertBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintInsertBenchmark)->Apply(MapParameters)->UseRealTime();