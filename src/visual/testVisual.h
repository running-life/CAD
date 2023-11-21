#pragma once
#include "earcut.h"
#include "EulerOperationTest.h"
#include <GL/glew.h>
#include <algorithm>

using vPoint = std::array<float, 2>;

void setPointAndPolygon(std::vector<Point*>& outer, std::vector<std::vector<Point*>>& inner, std::vector<Eigen::Vector3f>& visualPoints, std::vector<std::vector<vPoint>>& polygon);
void setPointAndPolygon(std::vector<Point*>& points, std::vector<Eigen::Vector3f>& visualPoints, std::vector<std::vector<vPoint>>& polygon);

double max(double x1, double x2, double x3);
double min(double x1, double x2, double x3);


void visualCube(std::vector<Eigen::Vector3f>& allPoints, std::vector<unsigned int>& indices) {
	//Solid* cube = constructCube();
	Solid* cube = holeTest();
	std::vector<Point*> outer;
	std::vector<std::vector<Point*>> inner;
	std::vector<Eigen::Vector3f> visualPoints;
	std::vector<std::vector<vPoint>> polygon;
	int faceNum = cube->faceNum();
	Face* currFace = cube->sFaces;
	for (int i = 0; i < faceNum; ++i) {
		outer.clear();
		inner.clear();
		visualPoints.clear();
		polygon.clear();
		outer = currFace->getOuterLoopPoints();
		inner = currFace->getInnerLoopPoints();
		int currPointNum = allPoints.size();
		setPointAndPolygon(outer, inner, visualPoints, polygon);
		std::vector<unsigned int> tempIndices = mapbox::earcut<unsigned>(polygon);
		allPoints.insert(allPoints.end(), visualPoints.begin(), visualPoints.end());
		std::transform(std::begin(tempIndices), std::end(tempIndices), std::back_inserter(indices), [&](double i) {
			return i + currPointNum;
			});
		currFace = currFace->nextFace;
	}
}


int visualCube(unsigned int& VBO, unsigned int& VAO, unsigned int& EBO) {
	Solid* cube = constructCube();
	std::vector<Point*> outer;
	std::vector<std::vector<Point*>> inner;
	std::vector<Eigen::Vector3f> visualPoints;
	std::vector<std::vector<vPoint>> polygon;
	int faceNum = cube->faceNum();
	std::cout << "face num:" << faceNum << std::endl;
	std::vector<Eigen::Vector3f> allPoints;
	std::vector<unsigned int> indices;
	Face* currFace = cube->sFaces;
	for (int i = 0; i < faceNum; ++i) {
		outer.clear();
		inner.clear();
		visualPoints.clear();
		polygon.clear();
		outer = currFace->getOuterLoopPoints();
		inner = currFace->getInnerLoopPoints();
		int currPointNum = allPoints.size();
		setPointAndPolygon(outer, inner, visualPoints, polygon);
		std::vector<unsigned int> tempIndices = mapbox::earcut<unsigned int>(polygon);
		std::cout << "tempIndices" << tempIndices.size() << std::endl;
		allPoints.insert(allPoints.end(), visualPoints.begin(), visualPoints.end());
		std::transform(std::begin(tempIndices), std::end(tempIndices), std::back_inserter(indices), [&](double i) {
			return i + currPointNum;
			});
		currFace = currFace->nextFace;
	}
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allPoints.size() * sizeof(Eigen::Vector3f), allPoints.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(GLdouble), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	std::cout << "pointNum" << allPoints.size() << std::endl;
	std::cout << "indicesNum" << indices.size() << std::endl;
	for (auto& temp: indices) {
		std::cout << temp << " ";
	}
	return allPoints.size();
}

void setPointAndPolygon(std::vector<Point*>& outer, std::vector<std::vector<Point*>>& inner, std::vector<Eigen::Vector3f>& visualPoints, std::vector<std::vector<vPoint>>& polygon) {
	setPointAndPolygon(outer, visualPoints, polygon);
	for (auto& innerPoints : inner) {
		// reverse
		std::cout << "enter this" << std::endl;
		std::reverse(innerPoints.begin(), innerPoints.end());
		setPointAndPolygon(innerPoints, visualPoints, polygon);
	}
}

void setPointAndPolygon(std::vector<Point*>& points ,std::vector<Eigen::Vector3f>& visualPoints, std::vector<std::vector<vPoint>>& polygon) {
	int visualPointsSize = visualPoints.size();
	int pointsSize = points.size();

	// set the ignore dimension according to the first three points
	int ignoreDimension = 0;
	if (max(points[0]->x(), points[1]->x(), points[2]->x()) == min(points[0]->x(), points[1]->x(), points[2]->x()))
		ignoreDimension = 0;
	else if (max(points[0]->y(), points[1]->y(), points[2]->y()) == min(points[0]->y(), points[1]->y(), points[2]->y()))
		ignoreDimension = 1;
	else
		ignoreDimension = 2;
	
	std::transform(std::begin(points), std::end(points), std::back_inserter(visualPoints), [](Point* p) {
		return Eigen::Vector3f(p->position);
		});


	std::vector<vPoint> tempvPoint;
	std::transform(std::begin(visualPoints) + visualPointsSize, std::end(visualPoints), std::back_inserter(tempvPoint), [&](Eigen::Vector3f p){
		
		if (ignoreDimension == 0)
			return vPoint({ p.y(), p.z() });
		else if (ignoreDimension == 1)
			return vPoint({ p.x(), p.z() });
		return vPoint({ p.x(), p.y() });
		});
	polygon.push_back(tempvPoint);
}

double max(double x1, double x2, double x3) {
	return std::max(std::max(x1, x2), x3);
}

double min(double x1, double x2, double x3) {
	return std::min(std::min(x1, x2), x3);
}




