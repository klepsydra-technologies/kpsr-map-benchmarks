#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintVectorInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (unsigned int key, std::vector<int> value) { ht.insert(key, value, tid);};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorBracketsInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;
        auto insert = [&orderedMap] (unsigned int key, std::vector<int> value) { orderedMap[key] = value;};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;
        auto insert = [&orderedMap] (unsigned int key, std::vector<int> value) { orderedMap.insert({key, value});};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorBracketsInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap] (unsigned int key, std::vector<int> value) { unorderedMap[key] = value;};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap] (unsigned int key, std::vector<int> value) { unorderedMap.insert({key, value});};
        state.SetIterationTime(measureOperation(insert, valueVectorIntCreator));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintVectorInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorBracketsInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorBracketsInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorInsertBenchmark)->Apply(MapParameters)->UseRealTime();