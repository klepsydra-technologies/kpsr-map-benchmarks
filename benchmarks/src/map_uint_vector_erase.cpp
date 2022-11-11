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

static void LockFreeMapUintRemoveBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    for (auto _ : state) {
        LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
        for (unsigned int i = 0; i < mapSize; i++) {
            ht.insert(i, std::vector<int>(mapSize, i), tid);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            ht.remove(i, tid);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapEraseBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);

    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;
        for (unsigned int i = 0; i < mapSize; i++) {
            orderedMap[i] = std::vector<int>(mapSize, i); 
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            orderedMap.erase(i);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapEraseBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);

    for (auto _ : state) {
        std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
        for (unsigned int i = 0; i < mapSize; i++) {
            unorderedMap[i] = std::vector<int>(mapSize, i); 
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            unorderedMap.erase(i);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapUintRemoveBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapEraseBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapEraseBenchmark)->Apply(MapParameters)->UseRealTime();