#pragma once
#include "Loop.h"
#include "Solid.h"

class Solid;

class Face {
public:
	Face() : preFace(nullptr), nextFace(nullptr), fLoopsInner(nullptr), fLoopsOuter(nullptr), fSolid(nullptr) {
	}

	~Face() {
		//Loop* tempLoop = fLoopsOuter;
		//if (tempLoop) {
		//	tempLoop->preLoop->nextLoop = nullptr;
		//}
		//
		//while (tempLoop) {
		//	tempLoop = tempLoop->nextLoop;
		//	delete tempLoop->preLoop;
		//}

		//tempLoop = fLoopsInner;
		//if (tempLoop) {
		//	tempLoop->preLoop->nextLoop = nullptr;
		//}

		//while (tempLoop) {
		//	tempLoop = tempLoop->nextLoop;
		//	delete tempLoop->preLoop;
		//}
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

	friend std::ostream& operator << (std::ostream& os, const Face& face) {
		os << "---------------------------------------------" << std::endl;
		// outer loop
		if (!face.fLoopsOuter) {
			return os;
		}
		int count = 1;
		os << "outer loop" << count << ":" << std::endl;
		os << *(face.fLoopsOuter);

		Loop* tempLoop = face.fLoopsOuter->nextLoop;
		while (tempLoop != face.fLoopsOuter) {
			count++;
			os << "outer loop" << count << ":" << std::endl;
			os << *(tempLoop);
			tempLoop = tempLoop->nextLoop;
		}

		// inner loop
		if (!face.fLoopsInner) {
			return os;
		}
		int count = 1;
		os << "inner loop" << count << ":" << std::endl;
		os << *(face.fLoopsInner);

		Loop* tempLoop = face.fLoopsInner->nextLoop;
		while (tempLoop != face.fLoopsInner) {
			count++;
			os << "inner loop" << count << ":" << std::endl;
			os << *(tempLoop);
			tempLoop = tempLoop->nextLoop;
		}

		os << "---------------------------------------------" << std::endl;
		return os;
	}

	Face* preFace;
	Face* nextFace;
	Loop* fLoopsInner;
	Loop* fLoopsOuter;

	Solid* fSolid;
};