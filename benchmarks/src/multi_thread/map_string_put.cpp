#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapInsertBenchmark, std::string, int)(benchmark::State &state)
{
    int tid = threadCount - 1;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
        bool found = false;
        auto insert = [&ht, tid] (std::string key, int value) { ht.insert(key, value, tid);};
        auto search = [&ht, tid, &found] (std::string key) {ht.search(key, tid, found);};
        state.SetIterationTime(measureOperationMultithreaded(insert, search, valueIntCreator));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapInsertBracketsBenchmark, std::string, int)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::map<std::string, int> orderedMap;
        auto insert = [&orderedMap, &mutex] (std::string key, int value) {
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap[key] = value;
        };
        auto find = [&orderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = orderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueIntCreator));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapInsertBenchmark, std::string, int)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::map<std::string, int> orderedMap;
        auto insert = [&orderedMap, &mutex] (std::string key, int value) { 
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap.insert({key, value}); 
        };
        auto find = [&orderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = orderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapInsertBracketsBenchmark, std::string, int)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::unordered_map<std::string, int> unorderedMap;
        auto insert = [&unorderedMap, &mutex] (std::string key, int value) { 
            std::lock_guard<std::mutex> guard(mutex);
            unorderedMap[key] = value;
        };
        auto find = [&unorderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = unorderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapInsertBenchmark, std::string, int)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::unordered_map<std::string, int> unorderedMap;
        auto insert = [&unorderedMap, &mutex] (std::string key, int value) { 
            std::lock_guard<std::mutex> guard(mutex);
            unorderedMap.insert({key, value}); 
        };
        auto find = [&unorderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = unorderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueIntCreator));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapInsertBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapInsertBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
