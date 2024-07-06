#pragma once
class voxel
{
public:
	//fields
	short assignment = 0; //assignment of this voxel -> 0 - unconnected pore; 1 - connected pore; 2 - steel; 3 - oxide
	int x, y, z; //location of this voxel in array
	bool processed = false;

	//constructor
	voxel(short a, int x, int y, int z) {
		this->assignment = a;
		this->x = x;
		this->y = y;
		this->z = z;
	}
	voxel() {
		assignment = 0;
		x = 0;
		y = 0;
		z = 0;
	}


	//methods
	void setOxide(); //set assignment as oxide
	void setAssigment(short a); //set custom assignment
	short getAssignment();
	void setProcessed(bool b);
	bool isProcessed();
	void setValues(short a, int x, int y, int z);

	
};

