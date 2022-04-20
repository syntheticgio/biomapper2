//
// Created by Joro on 4/19/22.
//

#include "src/BioMapper.h"
#include <benchmark/benchmark.h>


static void BM_VerifyFiles(benchmark::State& state) {
	std::vector <std::string> fail_list;
	BioMapper bm = BioMapper(4);
	bm.addFile("test/file1.csv", 0, 1, 2);
	bm.addFile("test/file2.csv", 0, 1, 2);
	bm.addFile("test/file3.csv", 0, 1, 2);
	bm.addFile("test/file4.csv", 0, 1, 2);
	for (auto _ : state)
		bm._verifyFiles(fail_list);
}
// Register the function as a benchmark
BENCHMARK(BM_VerifyFiles);

static void BM_ParseHeaders(benchmark::State& state) {
	BioMapper bm = BioMapper(4);
	bm.addFile("test/file1.csv", 0, 1, 2);
	bm.addFile("test/file2.csv", 0, 1, 2);
	bm.addFile("test/file3.csv", 0, 1, 2);
	bm.addFile("test/file4.csv", 0, 1, 2);
	for (auto _ : state)
		bm._parseHeaders();
}
// Register the function as a benchmark
BENCHMARK(BM_ParseHeaders);

static void BM_DetermineReferences(benchmark::State& state) {
	BioMapper bm = BioMapper(4);
	bm.addFile("test/file1.csv", 0, 1, 2);
	bm.addFile("test/file2.csv", 0, 1, 2);
	bm.addFile("test/file3.csv", 0, 1, 2);
	bm.addFile("test/file4.csv", 0, 1, 2);
	for (auto _ : state)
		bm._determineReferences();
}
// Register the function as a benchmark
BENCHMARK(BM_DetermineReferences);

static void BM_Map(benchmark::State& state) {
	BioMapper bm = BioMapper(4);
	bm.addFile("test/file1.csv", 0, 1, 2);
	bm.addFile("test/file2.csv", 0, 1, 2);
	bm.addFile("test/file3.csv", 0, 1, 2);
	bm.addFile("test/file4.csv", 0, 1, 2);
	for (auto _ : state)
		bm.map();
}
// Register the function as a benchmark
BENCHMARK(BM_Map);


BENCHMARK_MAIN();

//int main(int argv, char * argc[]) {
//    // Test function
//
//    BioMapper bm = BioMapper(4);
//    bm.addFile("file1.csv", 0, 1, 2);
//    bm.addFile("file2.csv", 0, 1, 2);
//    bm.addFile("file3.csv", 0, 1, 2);
//    bm.addFile("file4.csv", 0, 1, 2);
//
//    bm.map();
//
//
//}