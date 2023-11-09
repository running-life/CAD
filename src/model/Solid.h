#pragma once

#include "Face.h"

class Solid {
public:
	Solid():preSolid(nullptr), nextSolid(nullptr), sFaces(nullptr), edges(nullptr) {}
	void addFace(Face* face) {
		Face* tempFace = sFaces;
		while (tempFace->nextFace) {
			tempFace = tempFace->nextFace;
		}
		tempFace->nextFace = face;
		face->preFace = tempFace;
	}
	Solid* preSolid;
	Solid* nextSolid;
	Face* sFaces;
	Edge* edges;
};