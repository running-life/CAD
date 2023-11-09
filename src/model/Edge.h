#pragma once
#include "Vertex.h"
#include "Loop.h"

class HalfEdge;


class Edge {
public:
	Edge():v1(nullptr), v2(nullptr), firstHalfEdge(nullptr), secondHalfEdge(nullptr){}
	Vertex* v1;
	Vertex* v2;
	HalfEdge* firstHalfEdge;
	HalfEdge* secondHalfEdge;
};


class HalfEdge {
public:
	HalfEdge():v1(nullptr), v2(nullptr), twin(nullptr), pre(nullptr), nxt(nullptr), heLoop(nullptr){}
	Vertex* v1;
	Vertex* v2;
	HalfEdge* twin;
	HalfEdge* pre;
	HalfEdge* nxt;
	Loop* heLoop;
};