#pragma once
// Author : Darshit Nasit

#include <iostream>
#include <vector>
#include <map>
using namespace std;


template<typename Datatype>
class Edge
{
public:
	Edge() { weight = 1.0; }

	Edge(const Datatype& first, const Datatype& second, double weight = 1.0) {
		this->first = first;
		this->second = second;
		this->weight = weight;
	}

	const Datatype getFirst() const { return first; }
	const Datatype getSecond() const { return second; }
	double getWeight() const { return weight; }

private:
	Datatype first;
	Datatype second;
	double weight;
};


template<typename Datatype>
class Graph
{
public:
	void insert_node(const Datatype&);
	void insert_edge(const Edge<Datatype>&);
	void remove_node(const Datatype&);
	void remove_edge(const Edge<Datatype>&);
	void change_weight(const Edge<Datatype>&, double);
	
	int total_components();

	bool has_edge(const Datatype&, const Datatype&);
	bool has_connection(const Datatype&, const Datatype&);

private:
	void dfs_util(const Datatype&, map<Datatype, bool>&);

private:
	map<Datatype, map<Datatype, double> > graph;
};


template<typename Datatype>
void Graph<Datatype> ::insert_node(const Datatype& node) {
	if (graph.find(node) != graph.end())
		throw exception("Node already exist in graph");
	graph[node] = map<Datatype, double>();
}

template<typename Datatype>
void Graph<Datatype> ::insert_edge(const Edge<Datatype>& edge) {
	Datatype first = edge.getFirst();
	Datatype second = edge.getSecond();
	double weight = edge.getWeight();

	if (graph.find(first) == graph.end() or graph.find(second) == graph.end())
		throw exception("Respected nodes of edge do not exist in the graph");

	const map<Datatype, double>& firstMap = graph[first];
	if (firstMap.find(second) != firstMap.end())
		throw exception("Edge already exists");

	graph[first][second] = weight;
	graph[second][first] = weight;
}

template<typename Datatype>
void Graph<Datatype> ::remove_node(const Datatype& node) {
	if (graph.find(node) == graph.end())
		throw exception("Node does not exist in graph");

	for (auto& [first, second] : graph[node])
		graph[first].erase(node);

	graph.erase(node);
}

template<typename Datatype>
void Graph<Datatype> ::remove_edge(const Edge<Datatype>& edge) {
	Datatype first = edge.getFirst();
	Datatype second = edge.getSecond();

	if (graph.find(first) == graph.end() or graph.find(second) == graph.end())
		throw exception("Respected nodes of edge do not exist in the graph");

	map<Datatype, double>& firstMap = graph[first];
	if (firstMap.find(second) == firstMap.end())
		throw exception("Given edge not found in the graph");

	firstMap.erase(second);
	graph[second].erase(first);
}

template<typename Datatype>
void Graph<Datatype> ::change_weight(const Edge<Datatype>& edge, double weight) {
	Datatype first = edge.getFirst();
	Datatype second = edge.getSecond();

	if (graph.find(first) == graph.end() or graph.find(second) == graph.end())
		throw exception("Respected nodes of edge do not exist in the graph");

	map<Datatype, double>& firstMap = graph[first];
	if (firstMap.find(second) == firstMap.end())
		throw exception("Given edge not found in the graph");

	firstMap[second] = weight;
	graph[second][first] = weight;
}

template<typename Datatype>
int Graph<Datatype> ::total_components() {
	int count = 0;
	map<Datatype, bool> visited;
	for (auto& [first, second] : graph) {
		if (visited.find(first) == visited.end()) {
			dfs_util(first, visited);
			count++;
		}
	}
	return count;
}

template<typename Datatype>
bool Graph<Datatype> ::has_edge(const Datatype& first, const Datatype& second) {
	if (graph.find(first) == graph.end() or graph.find(second) == graph.end())
		throw exception("Respected nodes of edge do not exist in the graph");

	const map<Datatype, double>& firstMap = graph[first];
	return firstMap.find(second) != firstMap.end();
}

template<typename Datatype>
bool Graph<Datatype> ::has_connection(const Datatype& first, const Datatype& second) {
	if (graph.find(first) == graph.end() or graph.find(second) == graph.end())
		throw exception("Respected nodes of edge do not exist in the graph");

	map<Datatype, bool> visited;
	dfs_util(first, visited);
	return visited.find(second) != visited.end();
}

template<typename Datatype>
void Graph<Datatype> ::dfs_util(const Datatype& node, map<Datatype, bool>& visited) {
	visited[node] = true;
	for (auto& [first, second] : graph[node]) {
		if (visited.find(first) == visited.end())
			dfs_util(first, visited);
	}
}