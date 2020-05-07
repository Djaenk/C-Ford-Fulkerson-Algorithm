#include "weighted_directed_graph.hpp"
#include "file_to_string.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

uint
benchmarkMaximumFlow(WeightedDirectedGraph<std::string>& graph){
	using namespace std::chrono;
	double maximum_flow;

	auto start = high_resolution_clock::now();
	maximum_flow = graph.maximumFlow("1", "2", true);
	auto stop = high_resolution_clock::now();
	auto runtime = duration_cast<microseconds>(stop - start);
	std::cout << "Edmonds-Karp Algorithm runtime: " << runtime.count() << " microseconds" << std::endl;
	std::cout << "Maximum flow: " << maximum_flow << std::endl;

	start = high_resolution_clock::now();
	maximum_flow = graph.maximumFlow("1", "2", false);
	stop = high_resolution_clock::now();
	runtime = duration_cast<microseconds>(stop - start);
	std::cout << "Brute Force runtime: " << runtime.count() << " microseconds" << std::endl;
	std::cout << "Maximum flow: " << maximum_flow << std::endl;
	return maximum_flow;
}

int main(int argc, char *argv[]){
	if (argc > 3){
		WeightedDirectedGraph<std::string> graph;
		graph.readAdjacencyMatrix(readFileToString(argv[1]));
		std::cout << graph.maximumFlow(argv[2], argv[3]) << std::endl;
	}
	else{
		for (uint i = 1; i < 4; ++i){
			WeightedDirectedGraph<std::string> graph;
			std::string graphname = "graph" + std::to_string(i);
			std::string adjacencyMatrix = readFileToString(graphname + ".txt");
			graph.readAdjacencyMatrix(adjacencyMatrix);
			std::cout << graphname << std::endl;
			benchmarkMaximumFlow(graph);
			std::cout << std::endl;
		}
	}

	return 0;
}