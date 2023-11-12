#pragma once

#include "Face.h"

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
	Solid* preSolid;
	Solid* nextSolid;
	Face* sFaces;
	Edge* edges;
};