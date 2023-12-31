#pragma once
#include "Solid.h"

class EulerOperation {
public:
	
	/**
	 * @brief make vertex, face, solid
	 * @param p input variable, vertex position
	 * @param vertex output variable, the vertex this function created 
	 * @param solid output variable, the solid this function created
	 * @param face new face
	*/
	static void mvfs(const Point& p, Vertex*& vertex, Solid*& solid, Face*& face) {
		solid = new Solid;
		face = new Face;
		solid->addFace(face);
		vertex = new Vertex(p);
	}

	/**
	 * @brief make vertex, edge
	 * @param v1 the vertex in the loop
	 * @param v2p the position of new vertex
	 * @param loop the loop where the new edge to be constructed is located
	*/
	static void mev(Vertex* v1, const Point& v2p, Loop* loop, Vertex*& v2) {
		v2 = new Vertex(v2p);
		Edge* edge = Edge::makeEdge(v1, v2);
		loop->addHalfEdge(edge->firstHalfEdge, edge->secondHalfEdge);
		loop->lFace->fSolid->addEdge(edge);
	}

	/**
	 * @brief make edge, face
	 * @param v1 vertex1
	 * @param v2 vertex2
	 * @param loop the loop which contains v1 and v2
	 * @param newFace this face contain a new loop which have the half-edge from v2 to v1
	*/
	static void mef(Vertex* v1, Vertex* v2, Loop* loop, Face*& newFace) {
		// make an edge
		Edge* edge = Edge::makeEdge(v1, v2);
		
		Loop* newLoop = loop->insertEdgeSplitLoop(v1, v2, edge);

		newFace = new Face();
		newFace->addLoop(newLoop);
		newFace->fSolid = loop->lFace->fSolid;
		loop->lFace->fSolid->addEdge(edge);
		loop->lFace->fSolid->addFace(newFace);
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

	/**
	 * @brief convert face2 to a inner loop of face1
	 * @param face1 we'll add a loop into this face
	 * @param face2 we assume face2 contain only one loop
	*/
	static void kfmrh(Face* face1, Face* face2) {
		face2->fLoopsOuter->isInner = true;
		face1->addLoop(face2->fLoopsOuter);
		face2->fLoopsOuter = nullptr;
		face2->fSolid->deleteFace(face2);
	}

	static void sweep(Face *face, Eigen::Vector3f direction, float d) {
		Loop* outerLoop = face->fLoopsOuter;
		Face* bottomFace = sweepLoop(outerLoop, direction, d);
		Loop* innerLoop = face->fLoopsInner;
		if (!innerLoop) {
			std::cout << "yes" << std::endl;
			return;
		}
		
		Face* innerFace = sweepLoop(innerLoop, direction, d);
		kfmrh(bottomFace, innerFace);
		while (1) {
			std::cout << "inner" << std::endl;
			innerLoop = innerLoop->nextLoop;
			if (innerLoop == face->fLoopsInner)
				break;
			Face* innerFace = sweepLoop(innerLoop, direction, d);
			kfmrh(bottomFace, innerFace);
		}
	}

private:
	/**
	 * @brief sweep a loop
	 * @param loop 
	 * @return the bottom face
	*/
	static Face* sweepLoop(Loop* loop, Eigen::Vector3f direction, float d) {
		Eigen::Vector3f up = direction * d;
		std::vector<Vertex*> allVertex = loop->getVertex();

		Vertex* preSweptVertex = nullptr;
		Vertex* sweptVertex = nullptr;
		Vertex* firstSweptVertex = nullptr;
		
		for (Vertex*& originVertex : allVertex) {
			Face* tempFace = nullptr;
			Point tempPoint = Point(originVertex->point->position + up);
			mev(originVertex, tempPoint, loop, sweptVertex);
			if (!preSweptVertex) {
				preSweptVertex = sweptVertex;
				firstSweptVertex = sweptVertex;
				continue;
			}
			mef(preSweptVertex, sweptVertex, loop, tempFace);
			preSweptVertex = sweptVertex;
		}
		Face* tempFace = nullptr;
		mef(preSweptVertex, firstSweptVertex, loop, tempFace);
		return tempFace;
	}
	

};