#ifndef MAXIMUM_FLOW_ALGORITHMS
#define MAXIMUM_FLOW_ALGORITHMS

#include "weighted_directed_graph.hpp"

template <typename vertex>
double
WeightedDirectedGraph<vertex>::maximumFlow(const vertex& source , const vertex& sink, const bool fast){
	return fast ? EdmondsKarp(source, sink) : bruteForce(source, sink);
}

template <typename vertex>
double
WeightedDirectedGraph<vertex>::bruteForce(const vertex& source, const vertex& sink){
	typedef std::pair<vertex, vertex> edge;

	double maxFlow = 0;
	std::unordered_map<edge, double, hash_pair> flow = this->weights; //stores flow of each edge

	//initialize flows to zero
	for (auto& value : flow){
		value.second = 0;
	}

	//iterate through every possible permutation of flows
	while(flow != this->weights){
		//increment flow of first edge which is not at capacity
		for (auto& value : flow){
			if (value.second != this->weights.at(value.first)){
				value.second += 1;
				break;
			}
			else {
				//if edge is at capacity, set flow to zero and increment next edge
				value.second = 0;
			}
		}

		//calculate quantity flowing in/out of each vertex
		std::unordered_map<vertex, double> vertexFlow;
		for (const auto& value : flow){
			const edge& e = value.first;
			vertexFlow[e.first] -= value.second;
			vertexFlow[e.second] += value.second;
		}

		//check if conservation of flow is upheld
		bool flowConservation = true;
		double permutationMaxFlow = vertexFlow.at(sink);
		vertexFlow.erase(sink);
		vertexFlow.erase(source);
		for (const auto& v : vertexFlow){
			if (v.second != 0){
				flowConservation = false;
				break;
			}
		}

		//if this permutation of edge flows respects conservatino of flow and has produced
		//the largest network flow thus far, update the maximum flow
		if (flowConservation && permutationMaxFlow > maxFlow){
			maxFlow = permutationMaxFlow;
		}
	}

	return maxFlow;
}

template <typename vertex>
double
WeightedDirectedGraph<vertex>::EdmondsKarp(const vertex& source, const vertex& sink){
	typedef std::pair<vertex, vertex> edge;

	double maxFlow = 0; //sum of bottlenecks
	auto traversibleEdges = this->vertices; //manages set of edges which can be used in augmenting path
	std::unordered_map<edge, double, hash_pair> flow; //tracks how much flow has been augmented to each edge
	std::unordered_map<vertex, vertex> discoveryTree; //stores each vertex's parent in bfs tree
	discoveryTree.reserve(this->vertices.size());

	//repeatedly perform breadth first searches to find augmenting paths from source to sink until none exist
	while (true){
		discoveryTree.clear();

		//bfs to create tree of parents
		std::queue<vertex> toSearch;
		toSearch.emplace(source);
		while(!toSearch.empty()){
			const vertex currentVertex = toSearch.front();
			for (const vertex& neighbor : traversibleEdges[currentVertex]){
				edge toTraverse = std::make_pair(currentVertex, neighbor);
				if (discoveryTree.find(neighbor) == discoveryTree.end()
					&&this->weights[toTraverse] - flow[toTraverse] > 0
				){
					discoveryTree.emplace(neighbor, currentVertex);
					if (neighbor == sink){        //once the sink has been reached,
						goto calculateBottleneck; //exit the bfs
					}
					toSearch.emplace(neighbor);
				}
			}
			toSearch.pop();
		}

		//if the sink can no longer be reached due to saturated edges, algorithm has finished
		break;

		//find the bottleneck value of the augmenting path
		calculateBottleneck:
		double pathBottleneck = this->weights[std::make_pair(discoveryTree.at(sink), sink)];
		for (vertex backtrack = sink; backtrack != source; backtrack = discoveryTree.at(backtrack)){
			edge augmentingEdge = std::make_pair(discoveryTree.at(backtrack), backtrack);
			const double edgeBottleneck = this->weights.at(augmentingEdge) - flow.at(augmentingEdge);
			if (edgeBottleneck < pathBottleneck){
				pathBottleneck = edgeBottleneck;
			}
		}

		//augment flow values of forward and residual edges along augmenting path
		for (vertex backtrack = sink; backtrack != source; backtrack = discoveryTree.at(backtrack)){
			flow[std::make_pair(discoveryTree.at(backtrack), backtrack)] += pathBottleneck;
			flow[std::make_pair(backtrack, discoveryTree.at(backtrack))] -= pathBottleneck;
			traversibleEdges.at(backtrack).emplace(discoveryTree.at(backtrack));
		}

		//add augmenting path bottleneck to maximum flow
		maxFlow += pathBottleneck;
	}

	return maxFlow;
}

#endif