#pragma once
#include "Vertex.h"
#include "Loop.h"

class HalfEdge;


class Edge {
public:
	HalfEdge* firstHalfEdge;
	HalfEdge* secondHalfEdge;
};


class HalfEdge {
public:
	static std::vector<HalfEdge*> allHalfEdge;
	HalfEdge* twin;
	HalfEdge* pre;
	HalfEdge* nxt;
	Loop* heLoop;
};