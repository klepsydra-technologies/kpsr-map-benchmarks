#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintInsertBenchmark, unsigned int, int)(benchmark::State &state)
{
    int tid = threadCount - 1;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        LockfreeHashTableUint<int> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (unsigned int key, int value) { ht.insert(key, value, tid);};
        bool found;
        auto search = [&ht, tid, &found] (unsigned int key) {ht.search(key, tid, found);};
        state.SetIterationTime(measureOperationMultithreaded(insert, search, valueIntCreator));        
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintInsertBracketsBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;
        auto insert = [&orderedMap, &mutex] (unsigned int key, int value) {
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap[key] = value;
        };
        auto find = [&orderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = orderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueIntCreator));   
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintInsertBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;
        auto insert = [&orderedMap, &mutex] (unsigned int key, int value) { 
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap.insert({key, value});
        };
        auto find = [&orderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = orderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueIntCreator));   
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintInsertBracketsBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::unordered_map<unsigned int, int> unorderedMap;
        auto insert = [&unorderedMap, &mutex] (unsigned int key, int value) { 
            std::lock_guard<std::mutex> guard(mutex);
            unorderedMap[key] = value;
        };
        auto find = [&unorderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = unorderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueIntCreator));   
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintInsertBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::unordered_map<unsigned int, int> unorderedMap;
        auto insert = [&unorderedMap, &mutex] (unsigned int key, int value) { 
            std::lock_guard<std::mutex> guard(mutex);
            unorderedMap.insert({key, value});
        };
        auto find = [&unorderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = unorderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueIntCreator));   
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintInsertBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintInsertBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
