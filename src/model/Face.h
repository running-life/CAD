#pragma once
#include "Loop.h"
#include "Solid.h"

class Face {
public:
	Face() : preFace(nullptr), nextFace(nullptr), fLoops(nullptr), fSolid(nullptr){}
	~Face() {
		while (fLoops) {
			Loop* loop;
			fLoops = fLoops->nextLoop;
			delete loop;
		}
	}
	Face* preFace;
	Face* nextFace;
	Loop* fLoops;
	Solid* fSolid;
};