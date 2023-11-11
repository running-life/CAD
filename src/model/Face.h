#pragma once
#include "Loop.h"
#include "Solid.h"

class Face {
public:
	Face() : preFace(nullptr), nextFace(nullptr), fLoops(nullptr), fSolid(nullptr){
		// head node
		fLoops = new Loop;
		fLoops->nextLoop = fLoops;
		fLoops->preLoop = fLoops;
	}
	void addLoop(Loop* loop) {
		loop->lFace = this;

		Loop* tempLoop = fLoops;
		while (tempLoop->nextLoop != fLoops) {
			tempLoop = tempLoop->nextLoop;
		}
		loop->nextLoop = fLoops;
		loop->preLoop = tempLoop;
		tempLoop->nextLoop->preLoop = loop;
		tempLoop->nextLoop = loop;
	}
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