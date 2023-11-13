#pragma once

#include "Face.h"
#include <iostream>

class Face;

class Solid {
public:
	Solid():preSolid(nullptr), nextSolid(nullptr), sFaces(nullptr), edges(nullptr) {}
	void addFace(Face* face) {
		face->fSolid = this;
		if (!sFaces) {
			sFaces = face;
			face->nextFace = face;
			sFaces->preFace = face;
			return;
		}
		face->nextFace = sFaces;
		face->preFace = sFaces->preFace;
		sFaces->preFace->nextFace = face;
		sFaces->preFace = face;
	}

	void deleteFace(Face* face) {
		face->preFace->nextFace = face->nextFace;
		face->nextFace = face->preFace;
		delete face;
	}

	void addEdge(Edge* edge) {
		if (!edges) {
			edges = edge;
			edge->nextEdge = edge;
			edge->preEdge = edge;
			return;
		}

		edge->nextEdge = edges;
		edge->preEdge = edges->preEdge;
		edges->preEdge->nextEdge = edge;
		edges->preEdge = edge;
	}

	void deleteEdge(Edge* edge) {
		edge->preEdge->nextEdge = edge->nextEdge;
		edge->nextEdge->preEdge = edge->preEdge;
		delete edge;
	}

	/**
	 * @brief count the number of edges
	 * @return the number of edges
	*/
	int edgeNum() {
		if (!edges)
			return 0;
		int ans = 1;
		Edge* tempEdge = edges->nextEdge;
		while (tempEdge != edges) {
			ans++;
			tempEdge = tempEdge->nextEdge;
		}
		return ans;
	}

	Solid* preSolid;
	Solid* nextSolid;
	Face* sFaces;
	Edge* edges;
};