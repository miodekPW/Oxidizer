#pragma once
#include <vector>
#include "EasyBMP.h"
#include "voxel.h"
using namespace std;

class matrix
{
public:
	//fields
	
	int xMax, yMax, zMax; // dimensions of the data
	voxel*** data; //array for voxel data
	float densityOxide = 5.22; 
	float densitySteel = 7.7;
	int n_steel = 0;
	int n_retracted = 0;

	//methods
	void load2Ddata(string dir,int treshold); // load 2D bitmap as an input data. dir is directory and filename, treshold is grayscale value above which pixel is considered as material
	void load3Ddata(string dir,int treshold); // load 3D data as series of bitmaps representing subsequent slices of microstructure. dir is directory and filename, treshold is grayscale value above which voxel is considered as material
	float porosity(); //calculate porosity
	void dilate(int direction); //dilate procedure
	void draw2D(int zlayer, string path); //export 2D bitmap. zlayer is 
	void draw3D(string path);
	float calculateCrContent(float initial_cr_content);
	void erosion();
	float calculateDilationTime(int step, double kp, double voxelSize);
	float calculateErosionTime(int step, double kp, double voxelSize);
	void oxideGrowth(int nrSteps, double kp, double voxelSize, int direction, fstream &results,float cr_content);

};

