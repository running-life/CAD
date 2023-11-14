#pragma once
#include "EulerOperation.h"

Solid* constructCube() {
	std::cout << "begin to construct" << std::endl;
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
	EulerOperation::mef(cubeVertex[0], cubeVertex[3], topFaceLoop, bottomFace);

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