#ifndef WEIGHTED_DIRECTED_GRAPH
#define WEIGHTED_DIRECTED_GRAPH

#include "simple_graph.hpp"

template <typename vertex>
class WeightedDirectedGraph : public Graph<vertex>{
	private:
		double bruteForce(const vertex&, const vertex&);
		double EdmondsKarp(const vertex&, const vertex&);
	protected:
		//inherit pair hashing function from base graph class
		struct hash_pair : public Graph<vertex>::hash_pair{};

		std::unordered_map<std::pair<vertex, vertex>, double, hash_pair> weights;

		//prevent public use of inherited addEdge function, which does not
		//require a weight to be provided
		using Graph<vertex>::addEdge;

	public:
		void addEdge(const vertex&, const vertex&, const double);
		void removeEdge(const vertex&, const vertex&);
		void clear();

		double maximumFlow(const vertex&, const vertex&, const bool = true);

		std::string toAdjacencyMatrix(std::string delimiter = ",");
		void readAdjacencyMatrix(std::string, const char delimiter = ',');
};

template <typename vertex>
void
WeightedDirectedGraph<vertex>::addEdge(const vertex& target1, const vertex& target2, const double weight){
	if (this->vertices.find(target1) == this->vertices.end()
	|| this->vertices.find(target2) == this->vertices.end()){
		throw std::out_of_range("Edge to be added requires nonexistent vertex or vertices in Weighted Directed Graph.");
	}
	else if (this->vertices.at(target1).find(target2) != this->vertices.at(target1).end()){
		throw std::invalid_argument("Edge to be added already exists in Weighted Directed Graph.");
	}
	else{
		this->vertices.at(target1).emplace(target2);
		weights.emplace(std::make_pair(target1, target2), weight);
		this->edgeCount++;
	}
}

template <typename vertex>
void
WeightedDirectedGraph<vertex>::removeEdge(const vertex& target1, const vertex& target2){
	if (this->vertices.find(target1) == this->vertices.end()
	|| this->vertices.find(target2) == this->vertices.end()){
		throw std::out_of_range("Edge to by removed requires nonexistent vertex or vertices in Graph.");
	}
	else if (this->vertices.at(target1).find(target2) == this->vertices.at(target1).end()){
		throw std::invalid_argument("Edge to be removed does not exist in Graph.");
	}
	else{
		this->vertices.at(target1).erase(target2);
		weights.erase(std::make_pair(target1, target2));
		this->edgeCount--;
	}
}

template <typename vertex>
void
WeightedDirectedGraph<vertex>::clear(){
	Graph<vertex>::clear();
	weights.clear();
}

template <typename vertex>
std::string
WeightedDirectedGraph<vertex>::toAdjacencyMatrix(const std::string delimiter){
	std::ostringstream output;

	//graph name
	this->ID != "" ? output << this->ID : output << "Graph";

	//column vertices
	for (const auto& target2 : this->vertices){
		output << delimiter << target2.first;
	}

	output << std::endl;

	for (const auto& target1 : this->vertices){
		const auto& neighbors = target1.second;

		//row vertices
		output << target1.first;

		//edges
		for (const auto& target2 : this->vertices){
			output << delimiter;
			neighbors.find(target2.first) != neighbors.end()?
				output << weights.at(std::make_pair(target1.first, target2.first)):
				output << 0;
		}

		output << std::endl;
	}
	return output.str();
}

template <typename vertex>
void
WeightedDirectedGraph<vertex>::readAdjacencyMatrix(std::string graph, const char delimiter){
	clear();
	std::istringstream input(graph);
	
	//read graph name
	std::string columns;
	std::getline(input, columns);
	std::istringstream columnstream(columns);
	std::getline(columnstream, this->ID, delimiter);

	//read vertices
	std::string vertexString;
	std::vector<vertex> vertexLabels;
	while(std::getline(columnstream, vertexString, delimiter)){
		std::istringstream vertexStream(vertexString);
		vertex vertexValue;
		vertexStream >> std::noskipws >> vertexValue;
		this->addVertex(vertexValue);
		vertexLabels.emplace_back(vertexValue);
	}

	std::string line;
	while (std::getline(input, line)){
		std::stringstream linestream(line);
		std::getline(linestream, vertexString, delimiter);
		std::istringstream vertexStream(vertexString);
		vertex vertexValue;
		vertexStream >> std::noskipws >> vertexValue;

		//read edges
		std::string weight;
		for(uint i = 0; std::getline(linestream, weight, delimiter); i++){
			if (weight != "0"){
				addEdge(vertexValue, vertexLabels[i], std::stod(weight));
			}
		}
	}
}

#endif