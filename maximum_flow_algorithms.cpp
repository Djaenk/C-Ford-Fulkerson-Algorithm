#include "weighted_directed_graph.hpp"

template <typename vertex>
double
WeightedDirectedGraph<vertex>::maximumFlow(const vertex& source , const vertex& sink, const bool optimized){
	return EdmondsKarp(source, sink);
}

template <typename vertex>
double
WeightedDirectedGraph<vertex>::EdmondsKarp(const vertex& source, const vertex& sink){
	typedef std::pair<vertex, vertex> edge;

	double maxFlow = 0;
	auto traversibleEdges = this->vertices;
	std::unordered_map<edge, double> flow;
	std::unordered_map<edge, double> capacity = weights;

	while (true){
		//perform bfs to find augmenting path from source to sink
		std::unordered_map<vertex, vertex> discoveryTree;
		discoveryTree.reserve(this->vertices.size());
		std::queue<vertex> toSearch;
		toSearch.emplace(source);
		while(!toSearch.empty()){
			const vertex& currentVertex = toSearch.front();
			for (const vertex& neighbor : traversibleEdges[currentVertex]){
				edge toTraverse = std::make_pair(currentVertex, neighbor);
				if (capacity[toTraverse] - flow[toTraverse] > 0
					&& discoveryTree.find(neighbor) == discoveryTree.end()
				){
					discoveryTree.emplace(neighbor, currentVertex);
					if (neighbor == sink){ //if the sink has been reached,
						goto constructPath;       //exit the bfs entirely
					}
					toSearch.emplace(neighbor);
				}
			}
			toSearch.pop();
		}

		//if the sink cannot be reached, stop finding augmenting paths
		break;

		//given the tree of parents, construct the path
		constructPath:
		std::unordered_set<edge> augmentingPath;
		for (vertex backtrack = sink; backtrack != source; backtrack = discoveryTree.at(backtrack)){
			augmentingPath.emplace(std::make_pair(discoveryTree.at(backtrack), backtrack));
		}

		//find the bottleneck value of the augmenting path
		double pathBottleneck = capacity[*augmentingPath.begin()];
		for (const edge& edge : augmentingPath){
			double edgeBottleneck = capacity[edge] - flow[edge];
			if (edgeBottleneck < pathBottleneck){
				pathBottleneck = edgeBottleneck;
			}
		}

		//augment flow values of forward and residual edges along augmenting path
		for (const edge& edge : augmentingPath){
			flow[edge] += pathBottleneck;
			flow[std::make_pair(edge.second, edge.first)] -= pathBottleneck;
			traversibleEdges.at(edge.second).emplace(edge.first);
		}

		//add augmenting path bottleneck to maximum flow
		maxFlow += pathBottleneck;
	}

	return maxFlow;
}