#pragma once
#include "Solid.h"

class EulerOperation {
	
	/**
	 * @brief make vertex, face, solid
	 * @param p input variable, vertex position
	 * @param vertex output variable, the vertex this function created 
	 * @param solid output variable, the solid this function created
	*/
	static void mvfs(const Point& p, Vertex*& vertex, Solid*& solid) {
		solid = new Solid;
		Face* face = new Face;
		solid->addFace(face);
		vertex = new Vertex(p);
	}

	/**
	 * @brief make vertex, edge
	 * @param v1 the vertex in the loop
	 * @param v2p the position of new vertex
	 * @param loop the loop where the new edge to be constructed is located
	*/
	static void mev(Vertex* v1, const Point& v2p, Loop* loop) {
		Vertex* v2 = new Vertex(v2p);
		Edge* edge = Edge::makeEdge(v1, v2);
		loop->addHalfEdge(edge->firstHalfEdge, edge->secondHalfEdge);
		loop->lFace->fSolid->addEdge(edge);
	}

	static void mef(Vertex* v1, Vertex* v2, Loop* loop, Face*& newFace) {
		// make an edge
		Edge* edge = Edge::makeEdge(v1, v2);
		
		Loop* newLoop = loop->insertEdgeSplitLoop(v1, v2, edge);
		newFace = new Face();
		newFace->addLoop(newLoop);
		newFace->fSolid = loop->lFace->fSolid;
		loop->lFace->fSolid->addEdge(edge);
	}


	/**
	 * @brief delete an edge, make an inner loop
	 * @param v1 the vertex on outer loop
	 * @param v2 the vertex on inner loop
	 * @param loop the large loop(becomes the outer loop latter)
	 * @param newLoop inner loop
	*/
	static void kemr(Vertex* v1, Vertex* v2, Loop* loop, Loop*& newLoop) {
		Edge* edgeV1V2 = nullptr;
		newLoop = loop->deleteEdgeSplitLoop(v1, v2, edgeV1V2);

		// set property
		newLoop->isInner = true;
		newLoop->lFace = loop->lFace;
		
		loop->lFace->addLoop(newLoop);
		loop->lFace->fSolid->deleteEdge(edgeV1V2);
	}

};