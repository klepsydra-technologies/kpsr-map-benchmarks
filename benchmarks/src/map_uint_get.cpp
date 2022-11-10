#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>

void MapParameters(benchmark::internal::Benchmark* benchmark) {
    benchmark->ArgNames({"Size"});

    for (int e = 2; e <= 64; e*=2) {
        benchmark->Args({e});
    }
}

static void LockFreeMapUintSearchBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    LockfreeHashTableUint<int> ht(2 * mapSize, threadCount);
    for (unsigned int i = 0; i < mapSize; i++) {
        ht.insert(i, i, tid);
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            std::pair<int, bool> value = ht.search(i, tid);
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
    std::map<unsigned int, int> orderedMap;
    for (int i = 0; i < mapSize; i++) {
        orderedMap[i] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            int value = orderedMap[i]; 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapAtBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    std::map<unsigned int, int> orderedMap;
    for (unsigned int i = 0; i < mapSize; i++) {
        orderedMap[i] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            int value = orderedMap.at(i);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapBracketsBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    std::unordered_map<unsigned int, int> unorderedMap;
    for (unsigned int i = 0; i < mapSize; i++) {
        unorderedMap[i] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            int value = unorderedMap[i]; 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapAtBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    std::unordered_map<unsigned int, int> unorderedMap;
    for (unsigned int i = 0; i < mapSize; i++) {
        unorderedMap[i] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            int value = unorderedMap.at(i); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapUintSearchBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
