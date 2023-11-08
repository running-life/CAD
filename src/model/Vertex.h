#pragma once
#include "Point.h"
#include <unordered_set>
#include <vector>


class Vertex {
public:
	Vertex() {}

	
	//static std::vector<Vertex*> list;
private:
	Point point;
	unsigned int handle;
	std::unordered_set<unsigned int> inHalfEdge;
	std::unordered_set<unsigned int> outHalfEdge;
};
