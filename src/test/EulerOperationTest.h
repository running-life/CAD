#pragma once
#include "EulerOperation.h"

Solid* constructCube() {
	
	Point cubePoint[] = { {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, 
					 {0.5, -0.5, -0.5} , {0.5, 0.5, -0.5} , {-0.5, 0.5, -0.5} , {-0.5, -0.5, -0.5} };

	Vertex* cubeVertex[8] = {};
	Solid* cubeSolid = nullptr;
	Face* topFace = nullptr;
	EulerOperation::mvfs(cubePoint[0], cubeVertex[0], cubeSolid, topFace);

	


	Loop* topFaceLoop = new Loop();
	topFace->addLoop(topFaceLoop);

	

	EulerOperation::mev(cubeVertex[0], cubePoint[1], topFaceLoop, cubeVertex[1]);
	EulerOperation::mev(cubeVertex[1], cubePoint[2], topFaceLoop, cubeVertex[2]);
	EulerOperation::mev(cubeVertex[2], cubePoint[3], topFaceLoop, cubeVertex[3]);



	Face* bottomFace = nullptr;
	EulerOperation::mef(cubeVertex[3], cubeVertex[0], topFaceLoop, bottomFace);

	
	// chose the bottomFaceLoop
	Loop* bottomFaceLoop = bottomFace->fLoopsOuter;
	EulerOperation::mev(cubeVertex[0], cubePoint[4], bottomFaceLoop, cubeVertex[4]);
	EulerOperation::mev(cubeVertex[1], cubePoint[5], bottomFaceLoop, cubeVertex[5]);
	EulerOperation::mev(cubeVertex[2], cubePoint[6], bottomFaceLoop, cubeVertex[6]);
	EulerOperation::mev(cubeVertex[3], cubePoint[7], bottomFaceLoop, cubeVertex[7]);

	Face* frontFace = nullptr;
	EulerOperation::mef(cubeVertex[5], cubeVertex[4], bottomFaceLoop, frontFace);
	
	Face* rightFace = nullptr;
	EulerOperation::mef(cubeVertex[6], cubeVertex[5], bottomFaceLoop, rightFace);

	Face* backFace = nullptr;
	EulerOperation::mef(cubeVertex[7], cubeVertex[6], bottomFaceLoop, backFace);
	
	

	Face* leftFace = nullptr;
	EulerOperation::mef(cubeVertex[7], cubeVertex[4], bottomFaceLoop, leftFace);
	
	std::cout << (*cubeSolid);


	return cubeSolid;
}


Solid* holeTest() {
	Point cubePoint[] = { {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5},
					 {0.5, -0.5, -0.5} , {0.5, 0.5, -0.5} , {-0.5, 0.5, -0.5} , {-0.5, -0.5, -0.5} };

	Point holePoint[] = { {0.25, -0.25, 0.5}, {0.25, 0.25, 0.5}, {-0.25, 0.25, 0.5}, {-0.25, -0.25, 0.5},
					 {0.25, -0.25, -0.5} , {0.25, 0.25, -0.5} , {-0.25, 0.25, -0.5} , {-0.25, -0.25, -0.5} };

	Vertex* cubeVertex[8] = {};
	Vertex* holeVertex[8] = {};
	Solid* cubeSolid = nullptr;
	Face* topFace = nullptr;
	EulerOperation::mvfs(cubePoint[0], cubeVertex[0], cubeSolid, topFace);

	Loop* topFaceLoop = new Loop();
	topFace->addLoop(topFaceLoop);

	// std::cout << (*cubeSolid);

	EulerOperation::mev(cubeVertex[0], cubePoint[1], topFaceLoop, cubeVertex[1]);
	EulerOperation::mev(cubeVertex[1], cubePoint[2], topFaceLoop, cubeVertex[2]);
	EulerOperation::mev(cubeVertex[2], cubePoint[3], topFaceLoop, cubeVertex[3]);



	Face* bottomFace = nullptr;
	EulerOperation::mef(cubeVertex[3], cubeVertex[0], topFaceLoop, bottomFace);

	// chose the bottomFaceLoop
	Loop* bottomFaceLoop = bottomFace->fLoopsOuter;
	EulerOperation::mev(cubeVertex[0], cubePoint[4], bottomFaceLoop, cubeVertex[4]);
	EulerOperation::mev(cubeVertex[1], cubePoint[5], bottomFaceLoop, cubeVertex[5]);
	EulerOperation::mev(cubeVertex[2], cubePoint[6], bottomFaceLoop, cubeVertex[6]);
	EulerOperation::mev(cubeVertex[3], cubePoint[7], bottomFaceLoop, cubeVertex[7]);

	Face* frontFace = nullptr;
	EulerOperation::mef(cubeVertex[5], cubeVertex[4], bottomFaceLoop, frontFace);


	Face* rightFace = nullptr;
	EulerOperation::mef(cubeVertex[6], cubeVertex[5], bottomFaceLoop, rightFace);

	Face* backFace = nullptr;
	EulerOperation::mef(cubeVertex[7], cubeVertex[6], bottomFaceLoop, backFace);

	Face* leftFace = nullptr;
	EulerOperation::mef(cubeVertex[4], cubeVertex[7], bottomFaceLoop, leftFace);

	std::cout << *leftFace;


	// make hole

	EulerOperation::mev(cubeVertex[0], holePoint[0], topFaceLoop, holeVertex[0]);
	EulerOperation::mev(holeVertex[0], holePoint[1], topFaceLoop, holeVertex[1]);
	EulerOperation::mev(holeVertex[1], holePoint[2], topFaceLoop, holeVertex[2]);
	EulerOperation::mev(holeVertex[2], holePoint[3], topFaceLoop, holeVertex[3]);


	Face* holeBottomFace = nullptr;
	EulerOperation::mef(holeVertex[0], holeVertex[3], topFaceLoop, holeBottomFace);

;
	

	Loop* topFaceInnerLoop = nullptr;


	EulerOperation::kemr(cubeVertex[0], holeVertex[0], topFaceLoop, topFaceInnerLoop);



	Loop* holeBottomFaceLoop = holeBottomFace->fLoopsOuter;
	EulerOperation::mev(holeVertex[0], holePoint[4], holeBottomFaceLoop, holeVertex[4]);
	


	EulerOperation::mev(holeVertex[1], holePoint[5], holeBottomFaceLoop, holeVertex[5]);
	EulerOperation::mev(holeVertex[2], holePoint[6], holeBottomFaceLoop, holeVertex[6]);
	EulerOperation::mev(holeVertex[3], holePoint[7], holeBottomFaceLoop, holeVertex[7]);

	Face* holeFrontFace = nullptr;
	EulerOperation::mef(holeVertex[4], holeVertex[5], holeBottomFaceLoop, holeFrontFace);

	

	Face* holeRightFace = nullptr;
	EulerOperation::mef(holeVertex[5], holeVertex[6], holeBottomFaceLoop, holeRightFace);

	

	Face* holeBackFace = nullptr;
	EulerOperation::mef(holeVertex[6], holeVertex[7], holeBottomFaceLoop, holeBackFace);

	//std::cout << *holeBottomFace;

	Face* holeLeftFace = nullptr;
	EulerOperation::mef(holeVertex[7], holeVertex[4], holeBottomFaceLoop, holeLeftFace);

	

	EulerOperation::kfmrh(bottomFace, holeBottomFace);

	std::cout << *cubeSolid;

	return cubeSolid;
}