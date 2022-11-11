#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>

void MapParameters(benchmark::internal::Benchmark* benchmark) {
    benchmark->ArgNames({"Size"});

    for (int e = 2; e <= 64; e*=2) {
        benchmark->Args({e});
    }
}
static void LockFreeMapSearchBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;
    std::vector<std::string> keys(mapSize);

    LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
    for (int i = 0; i < mapSize; i++) {
        keys[i] = std::to_string(i);
        ht.insert(keys[i], i, tid);
    }
    bool found = false;

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            int value = ht.search(keys[i], tid, found);
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
    std::vector<std::string> keys(mapSize);
    std::map<std::string, int> orderedMap;
    for (int i = 0; i < mapSize; i++) {
        keys[i] = std::to_string(i);
        orderedMap[keys[i]] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            int value = orderedMap[keys[i]]; 
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
    std::vector<std::string> keys(mapSize);
    std::map<std::string, int> orderedMap;
    for (int i = 0; i < mapSize; i++) {
        keys[i] = std::to_string(i);
        orderedMap[keys[i]] = i; 
    }
    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            int value = orderedMap.at(keys[i]);
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
    std::vector<std::string> keys(mapSize);
    std::unordered_map<std::string, int> unorderedMap;
    for (int i = 0; i < mapSize; i++) {
        keys[i] = std::to_string(i);
        unorderedMap[keys[i]] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            int value = unorderedMap[keys[i]]; 
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
    std::vector<std::string> keys(mapSize);
    std::unordered_map<std::string, int> unorderedMap;
    for (int i = 0; i < mapSize; i++) {
        keys[i] = std::to_string(i);
        unorderedMap[keys[i]] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            int value = unorderedMap.at(keys[i]); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapSearchBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
