#pragma once
#include "Loop.h"
#include "Solid.h"

class Face {
public:
	Face* preFace;
	Face* nextFace;
	Loop* fLoops;
	Solid* fSolid;
};