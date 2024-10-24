#include "EasyBMP.h"
#include "matrix.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
	BMP image;
	string strTemp;
	float cr_content = 22.0; //Initial Cr content in the alloy
	double kp = 1.5e-14; // Parabolic rate law constant - kinetics of the oxidation
	double voxelSize = 0.175; //resolution of the image data = thickness of the oxide layer after one step of oxidation
	int steps = 18; //number of oxidation steps for dilation/erosion procedure
	int treshold = 1; //treshold value for binarizing input image
	string dir = "Example//"; //directory of the input image data
	string filename = "Fe22Cr_0001"; //file name - only bitmap data supported
	fstream resultsFile("Example//resultsFe22Cr.csv", ios::out); //directory and filename for results export

	//load image
	strTemp = dir + filename + ".bmp";
	matrix a;
	a.load2Ddata(strTemp, 1);

	a.draw2D(0, "Example//before_oxidation.bmp");
	a.oxideGrowth(steps, kp, voxelSize, 7, resultsFile, cr_content);
	a.draw2D(0, "Example//after_oxidation.bmp");
	resultsFile.close();
	
	return 0;
}