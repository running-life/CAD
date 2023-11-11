#pragma once
#include "Vertex.h"
#include "Loop.h"

class HalfEdge;


class Edge {
public:
	Edge():v1(nullptr), v2(nullptr), firstHalfEdge(nullptr), secondHalfEdge(nullptr), preEdge(nullptr), nextEdge(nullptr) {}
	/**
	 * @brief make an edge and two half-edges
	 * @param _v1 one vertex of this edge
	 * @param _v2 another vertex of this edge
	 * @return 
	*/
	static Edge* makeEdge(Vertex* _v1, Vertex* _v2) {
		Edge* edge = new Edge;

		HalfEdge* fHE = new HalfEdge(_v1, _v2);
		HalfEdge* sHE = new HalfEdge(_v2, _v1);
		fHE->twin = sHE;
		fHE->edge = edge;
		sHE->twin = fHE;
		sHE->edge = edge;

		edge->v1 = _v1;
		edge->v2 = _v2;
		edge->firstHalfEdge = fHE;
		edge->secondHalfEdge = sHE;
		
		return edge;
	}
	Vertex* v1;
	Vertex* v2;
	HalfEdge* firstHalfEdge;	// v1->v2
	HalfEdge* secondHalfEdge;	// v2->v1
	Edge* preEdge;
	Edge* nextEdge;

};


class HalfEdge {
public:
	HalfEdge():v1(nullptr), v2(nullptr), twin(nullptr), pre(nullptr), nxt(nullptr), heLoop(nullptr){}
	HalfEdge(Vertex* _v1, Vertex* _v2):v1(_v1), v2(_v2), twin(nullptr), pre(nullptr), nxt(nullptr), heLoop(nullptr) {}
	Vertex* v1;		//start vertex
	Vertex* v2;		//end vertex
	HalfEdge* twin;
	HalfEdge* pre;
	HalfEdge* nxt;
	Edge* edge;
	Loop* heLoop;
};