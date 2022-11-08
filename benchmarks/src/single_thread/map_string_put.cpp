#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapInsertBenchmark, std::string, int)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (std::string key, int value) { ht.insert(key, value, tid);};
        state.SetIterationTime(measureOperation(insert, valueIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapInsertBracketsBenchmark, std::string, int)(benchmark::State &state)
{
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::map<std::string, int> orderedMap;
        auto insert = [&orderedMap] (std::string key, int value) { orderedMap[key] = value;};
        state.SetIterationTime(measureOperation(insert, valueIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapInsertBenchmark, std::string, int)(benchmark::State &state)
{
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::map<std::string, int> orderedMap;
        auto insert = [&orderedMap] (std::string key, int value) { orderedMap.insert({key, value}); };
        state.SetIterationTime(measureOperation(insert, valueIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapInsertBracketsBenchmark, std::string, int)(benchmark::State &state)
{
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::unordered_map<std::string, int> unorderedMap;
        auto insert = [&unorderedMap] (std::string key, int value) { unorderedMap[key] = value;};
        state.SetIterationTime(measureOperation(insert, valueIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapInsertBenchmark, std::string, int)(benchmark::State &state)
{
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::unordered_map<std::string, int> unorderedMap;
        auto insert = [&unorderedMap] (std::string key, int value) { unorderedMap.insert({key, value}); };
        state.SetIterationTime(measureOperation(insert, valueIntCreator));
    }
}
BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapInsertBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapInsertBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapInsertBenchmark)->Apply(MapParameters)->UseRealTime();