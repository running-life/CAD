#pragma once
#include "Loop.h"
#include "Solid.h"

class Face {
public:
	Face() : preFace(nullptr), nextFace(nullptr), fLoopsInner(nullptr), fLoopsOuter(nullptr), fSolid(nullptr) {
	}
	void addLoop(Loop* loop) {
		loop->lFace = this;
		Loop* tempLoop;
		if (loop->isInner) {
			if (!fLoopsInner) {
				fLoopsInner = loop;
				loop->nextLoop = loop;
				loop->preLoop = loop;
				return;
			}
			tempLoop = fLoopsInner;
		}
		else {
			if (!fLoopsOuter) {
				fLoopsOuter = loop;
				loop->nextLoop = loop;
				loop->preLoop = loop;
				return;
			}
			tempLoop = fLoopsOuter;
		}
		loop->preLoop = tempLoop->preLoop;
		loop->nextLoop = tempLoop;
		tempLoop->preLoop->nextLoop = loop;
		tempLoop->preLoop = loop;
	}
	Face* preFace;
	Face* nextFace;
	Loop* fLoopsInner;
	Loop* fLoopsOuter;

	Solid* fSolid;
};