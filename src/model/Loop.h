#pragma once
#include "Edge.h"
#include "Face.h"

class Face;

class Loop {
public:
	Loop() :preLoop(nullptr), nextLoop(nullptr), lHalfEdges(nullptr), lFace(nullptr), isInner(false) {}

	// TODO: add error checking: if there is no vertex equals the v1 of first half-edge in this loop(except that there is no half-edge in this loop)
	void addHalfEdge(HalfEdge* firstHE, HalfEdge* secondHe) {
		// find preHalfEdge
		HalfEdge* tempHE = findHE(firstHE->v1);
		if (!tempHE) {
			lHalfEdges = firstHE;
			firstHE->nxt = secondHe;
			firstHE->pre = secondHe;
			secondHe->pre = firstHE;
			secondHe->nxt = firstHE;
			return;
		}
		firstHE->pre = tempHE;
		firstHE->nxt = secondHe;
		secondHe->pre = firstHE;
		secondHe->nxt = tempHE->nxt;
		tempHE->nxt->pre = secondHe;
		tempHE->nxt = firstHE;
	}

	/**
	 * @brief insert a new edge and split origin loop. After splitting, this loop contain the half-edge from v1 to v2
	 * @param v1 vertex1
	 * @param v2 vertex2
	 * @param newEdge new edge between v1 and v2
	 * @return new loop contain the half-edge from v2 to v1
	*/
	Loop* insertEdgeSplitLoop(Vertex* v1, Vertex* v2, Edge* newEdge) {

		/**
		 * bug exists
		// find half-edge who points to v1
		HalfEdge* hePointToV1 = this->findHE(v1);
		// find half-edge who points to v2
		HalfEdge* hePointToV2 = this->findHE(v2);
		 *
		 */
		
		// find half-edge who points to v1
		HalfEdge* hePointToV1 = this->findHED(v1, v2);
		// find half-edge who points to v2
		HalfEdge* hePointToV2 = this->findHED(v2, v1);
		

		HalfEdge* hePointToV1Nxt = hePointToV1->nxt;
		HalfEdge* hePointToV2Nxt = hePointToV2->nxt;

		// add the half-edge from v1 to v2
		hePointToV1->nxt = newEdge->firstHalfEdge;
		newEdge->firstHalfEdge->pre = hePointToV1;
		newEdge->firstHalfEdge->nxt = hePointToV2Nxt;
		hePointToV2Nxt->pre = newEdge->firstHalfEdge;

		// make a new loop
		Loop* newLoop = new Loop();
		newLoop->lHalfEdges = newEdge->secondHalfEdge;
		newEdge->secondHalfEdge->nxt = hePointToV1Nxt;
		hePointToV1Nxt->pre = newEdge->secondHalfEdge;
		newEdge->secondHalfEdge->pre = hePointToV2;
		hePointToV2->nxt = newEdge->secondHalfEdge;

		return newLoop;
	}

	/**
	 * @brief delete an edge and split origin loop. After splitting, this loop contain v1
	 * @param v1 vertex1
	 * @param v2 vertex2
	 * @param edge edge contain v1 and v2
	 * @return 
	*/
	Loop* deleteEdgeSplitLoop(Vertex* v1, Vertex* v2, Edge*& edge) {
		// find the half-edge from v1 to v2
		HalfEdge* heV1ToV2 = findHE(v1, v2);

		std::cout << *heV1ToV2;

		edge = heV1ToV2->edge;
		
		// inner ring
		Loop* innerLoop = new Loop();
		HalfEdge* heV2ToV1 = heV1ToV2->twin;
		heV2ToV1->pre->nxt = heV1ToV2->nxt;
		heV1ToV2->nxt->pre = heV2ToV1->pre;
		innerLoop->lHalfEdges = heV1ToV2->nxt;

		// outer loop
		heV2ToV1->nxt->pre = heV1ToV2->pre;
		heV1ToV2->pre->nxt = heV2ToV1->nxt;

		return innerLoop;
	}


	// TODO: if v doesn't exist in this loop, the function will enter an infinite loop
	/**
	 * @brief find the half-edge whose v2(end vertex) equals v
	 * @param v to be found
	 * @return the target half-edge which contain v or the head half-edge
	*/
	HalfEdge* findHE(Vertex* v) {
		HalfEdge* tempHE = lHalfEdges;
		if (!tempHE) {
			return nullptr;
		}
		while (tempHE->v2 != v) {
			tempHE = tempHE->nxt;
		}
		return tempHE;
	}

	/**
	 * @brief find the half-edge in certain direction
	 * @param v1 the vertex which the target half-edge point to
	 * @param v2 we should pass by v2 before find the target half-edge
	 * @return the the target half-edge point to v1
	*/
	HalfEdge* findHED(Vertex* v1, Vertex* v2) {
		HalfEdge* tempHe = lHalfEdges;
		while (!tempHe) {
			std::cout << "null" << std::endl;
			return nullptr;
		}
		while (!(tempHe->v1 == v2 || tempHe->v2 == v2)) {
			tempHe = tempHe->nxt;
		}
		while (tempHe->v2 != v1) {
			tempHe = tempHe->nxt;
		}
		return tempHe;
	}

	HalfEdge* findHE(Vertex* v1, Vertex* v2) {
		HalfEdge* tempHe = lHalfEdges;
		while (!tempHe) {
			std::cout << "null" << std::endl;
			return nullptr;
		}
		while (tempHe->v1 != v1 || tempHe->v2 != v2) {
			tempHe = tempHe->nxt;
		}
		return tempHe;
	}

	std::vector<Point*> getPoints() {
		std::vector<Point*> ans;
		if (!lHalfEdges) {
			return ans;
		}
		ans.push_back(lHalfEdges->v1->point);
		HalfEdge* tempHE = lHalfEdges->nxt;
		while (tempHE != lHalfEdges) {
			ans.push_back(tempHE->v1->point);
			tempHE = tempHE->nxt;
		}
		// if there is noly one half-edge in this loop
		if (ans.size() == 1) {
			ans.push_back(lHalfEdges->v2->point);
		}
		return ans;
	}

	friend std::ostream& operator << (std::ostream& os, const Loop& loop) {
		if (!loop.lHalfEdges) {
			return os << std::endl;
		}
		int count = 1;

		os << "HE" << count << ":" << (*loop.lHalfEdges);
		
		HalfEdge* tempHE = loop.lHalfEdges->nxt;
		while (tempHE != loop.lHalfEdges) {
			count++;
			os << "HE" << count << ":" << (*tempHE);
			tempHE = tempHE->nxt;
		}
		return os;
	}

	Loop* preLoop;
	Loop* nextLoop;
	HalfEdge* lHalfEdges;
	Face* lFace;
	bool isInner;
};