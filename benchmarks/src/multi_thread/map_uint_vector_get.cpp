#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>
#include <mutex>
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintVectorSearchBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    int tid = threadCount - 1;
    LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
    auto insert = [&ht, tid] (unsigned int key, std::vector<int> value) { ht.insert(key, value, tid);};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    bool found = false;
    auto search = [&ht, tid, &found] (unsigned int key) {ht.search(key, tid, found);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(search));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorBracketsBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    std::map<unsigned int, std::vector<int>> orderedMap;
    auto insert = [&orderedMap] (unsigned int key, std::vector<int> value) { orderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto bracketAccess = [&orderedMap, &mutex] (unsigned int key) {
        std::lock_guard<std::mutex> guard(mutex);
        auto value = orderedMap[key];
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(bracketAccess));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorAtBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    std::map<unsigned int, std::vector<int>> orderedMap;
    auto insert = [&orderedMap] (unsigned int key, std::vector<int> value) { orderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto atAccess = [&orderedMap, &mutex] (unsigned int key) {
        std::lock_guard<std::mutex> guard(mutex);
        auto value = orderedMap.at(key);
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(atAccess));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorBracketsBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
    auto insert = [&unorderedMap] (unsigned int key, std::vector<int> value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto bracketAccess = [&unorderedMap, &mutex] (unsigned int key) {
        std::lock_guard<std::mutex> guard(mutex);
        auto value = unorderedMap[key];
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(bracketAccess));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorAtBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
    auto insert = [&unorderedMap] (unsigned int key, std::vector<int> value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto atAccess = [&unorderedMap, &mutex] (unsigned int key) {
        std::lock_guard<std::mutex> guard(mutex);
        auto value = unorderedMap.at(key);
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(atAccess));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintVectorSearchBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorAtBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorAtBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
