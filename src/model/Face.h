#pragma once
#include "Loop.h"
#include "Solid.h"

class Solid;

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

	// assume we have only one outer loop int one face when call this function
	std::vector<Point*> getOuterLoopPoints() {
		Loop* tempLoop = fLoopsOuter;
		return fLoopsOuter->getPoints();
	}

	std::vector<std::vector<Point*> > getInnerLoopPoints() {
		std::vector<std::vector<Point*> > ans;
		if (!fLoopsInner) {
			return ans;
		}
		ans.push_back(fLoopsInner->getPoints());
		Loop* tempLoop = fLoopsInner->nextLoop;
		while (tempLoop != fLoopsInner) {
			ans.push_back(tempLoop->getPoints());
			tempLoop = tempLoop->nextLoop;
		}
		return ans;
	}

	friend std::ostream& operator << (std::ostream& os, const Face& face) {
		os << "---------------------------------------------" << std::endl;
		// outer loop
		if (!face.fLoopsOuter) {
			os << "---------------------------------------------" << std::endl;
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
			os << "---------------------------------------------" << std::endl;
			return os;
		}
		count = 1;
		os << "inner loop" << count << ":" << std::endl;
		os << *(face.fLoopsInner);

		tempLoop = face.fLoopsInner->nextLoop;
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