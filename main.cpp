#include "weighted_directed_graph.hpp"
#include "file_to_string.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(){
	WeightedDirectedGraph<int> graph;
	graph.readAdjacencyMatrix(readFileToString("test_graph.txt"));
	std::cout << graph.toAdjacencyMatrix();
	for (const auto& edge : graph.breadthFirstSearch(3)){
		std::cout << edge.first << ' ' << edge.second << std::endl;
	}
	std::cout << graph.maximumFlow(2, 1) << std::endl;

	return 0;
}