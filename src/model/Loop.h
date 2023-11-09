#pragma once
#include "Edge.h"
#include "Face.h"

class Loop {
public:
	

	Loop* preLoop;
	Loop* nextLoop;
	HalfEdge* lHalfEdges;
	Face* lFace;
	bool 
};