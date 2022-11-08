#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table.h>
#include <lockfree_hash_table_templated.h>

void MapParameters(benchmark::internal::Benchmark* benchmark) {
    benchmark->ArgNames({"Size"});

    for (int e = 2; e <= 64; e*=2) {
        benchmark->Args({e});
    }
}

static void LockFreeMapTemplatedInsertBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    for (auto _ : state) {
        LockfreeHashTableTemplated<unsigned int, int> ht(2 * mapSize, threadCount);

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            ht.insert(i, i, tid);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void LockFreeMapInsertBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    for (auto _ : state) {
        Lockfree_hash_table ht(2 * mapSize, threadCount);

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            ht.insert(i, i, tid);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapBracketsBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    
    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            orderedMap[i] = i; 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapInsertBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    
    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            orderedMap.insert({i, i}); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapBracketsBenchmark(benchmark::State &state)
{
    size_t mapSize = state.range(0);
    
    for (auto _ : state) {
        std::unordered_map<unsigned int, int> unorderedMap;

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            unorderedMap[i] = i; 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapInsertBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    
    for (auto _ : state) {
        std::unordered_map<unsigned int, int> unorderedMap;

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            unorderedMap.insert({i, i}); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapTemplatedInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(LockFreeMapInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapInsertBenchmark)->Apply(MapParameters)->UseRealTime();