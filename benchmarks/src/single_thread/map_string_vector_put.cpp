#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapStringInsertBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (std::string key, std::vector<int> value) { ht.insert(key, value, tid);};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringInsertBracketsBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::map<std::string, std::vector<int>> orderedMap;
        auto insert = [&orderedMap] (std::string key, std::vector<int> value) { orderedMap[key] = value;};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringInsertBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::map<std::string, std::vector<int>> orderedMap;
        auto insert = [&orderedMap] (std::string key, std::vector<int> value) { orderedMap.insert({key, value});};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringInsertBracketsBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::unordered_map<std::string, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap] (std::string key, std::vector<int> value) { unorderedMap[key] = value;};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringInsertBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::unordered_map<std::string, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap] (std::string key, std::vector<int> value) { unorderedMap.insert({key, value});};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapStringInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringInsertBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringInsertBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringInsertBenchmark)->Apply(MapParameters)->UseRealTime();