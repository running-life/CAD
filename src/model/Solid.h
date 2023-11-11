#pragma once

#include "Face.h"

class Solid {
public:
	Solid():preSolid(nullptr), nextSolid(nullptr), sFaces(nullptr), edges(nullptr) {
		// head node
		sFaces = new Face;
		sFaces->nextFace = sFaces;
		sFaces->preFace = sFaces;

		edges = new Edge;
		edges->preEdge = edges;
		edges->nextEdge = edges;

	}
	void addFace(Face* face) {
		face->fSolid = this;

		Face* tempFace = sFaces;
		while (tempFace->nextFace != sFaces) {
			tempFace = tempFace->nextFace;
		}
		face->nextFace = sFaces;
		face->preFace = tempFace;
		tempFace->nextFace->preFace = face;
		tempFace->nextFace = face;
	}

	void addEdge(Edge* edge) {
		Edge* tempEdge = edges;
		while (tempEdge->nextEdge != edges) {
			tempEdge = tempEdge->nextEdge;
		}
		edge->nextEdge = edges;
		edge->preEdge = tempEdge;
		tempEdge->nextEdge->preEdge = edge;
		tempEdge->nextEdge = edge;
	}
	Solid* preSolid;
	Solid* nextSolid;
	Face* sFaces;
	Edge* edges;
};