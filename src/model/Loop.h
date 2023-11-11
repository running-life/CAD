#pragma once
#include "Edge.h"
#include "Face.h"

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
	 * @brief this loop contain the half-edge from v1 to v2
	 * @param hePointToV1 half-edge point to v1
	 * @param hePointToV2 half-edge point to v2
	 * @param newEdge new edge between v1 and v2
	 * @return new loop contain the half-edge from v2 to v1
	*/
	Loop* splitLoop(HalfEdge* hePointToV1, HalfEdge* hePointToV2, Edge* newEdge) {
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

	Loop* preLoop;
	Loop* nextLoop;
	HalfEdge* lHalfEdges;
	Face* lFace;
	bool isInner;
};