#include "voxel.h"
void voxel::setOxide() {
	assignment = 3;
}

void voxel::setAssigment(short a) {
	assignment = a;
}

short voxel::getAssignment() {
	return assignment;
}

void voxel::setProcessed(bool b) {
	this->processed = b;
}

bool voxel::isProcessed() {
	return processed;
}

void voxel::setValues(short a, int x, int y, int z) {
	this->assignment = a;
	this->x = x;
	this->y = y;
	this->z = z;
}