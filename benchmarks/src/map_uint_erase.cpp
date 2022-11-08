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

static void LockFreeMapTemplatedRemoveBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    for (auto _ : state) {
        LockfreeHashTableTemplated<unsigned int, int> ht(2 * mapSize, threadCount);
        for (unsigned int i = 0; i < mapSize; i++) {
            ht.insert(i, i, tid);
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

static void LockFreeMapRemoveBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    for (auto _ : state) {
        Lockfree_hash_table ht(2 * mapSize, threadCount);
        for (int i = 0; i < mapSize; i++) {
            ht.insert(i, i, tid);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
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
    int mapSize = state.range(0);

    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;
        for (unsigned int i = 0; i < mapSize; i++) {
            orderedMap[i] = i; 
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
    int mapSize = state.range(0);

    for (auto _ : state) {
        std::unordered_map<unsigned int, int> unorderedMap;
        for (unsigned int i = 0; i < mapSize; i++) {
            unorderedMap[i] = i; 
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

BENCHMARK(LockFreeMapTemplatedRemoveBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(LockFreeMapRemoveBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapEraseBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapEraseBenchmark)->Apply(MapParameters)->UseRealTime();