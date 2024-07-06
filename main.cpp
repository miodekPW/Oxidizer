#include "EasyBMP.h"
#include "matrix.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
	BMP image;
	string strTemp;
	float cr_content = 27.0; //Initial Cr content in the alloy
	double kp = 1.3e-14; // Parabolic rate law constant - kinetics of the oxidation
	double voxelSize = 0.175; //resolution of the image data = thickness of the oxide layer after one step of oxidation
	int steps = 5; //number of oxidation steps for dilation/erosion procedure
	string dir = "Example//"; //directory of the input image data
	string filename = "Fe22Cr_0001"; //file name - only bitmap data supported
	fstream resultsFile("Example//resultsFe27Cr.csv", ios::out); //directory and filename for results export

	//load image
	strTemp = dir + filename + ".bmp";
	strTemp = "Wyniki//Model4//Fe22Cr_128_duze_4_000.bmp";
	matrix a;
	a.load3Ddata(strTemp, 1);
	
	//a.load2Ddata(strTemp, 1);
	
	a.oxideGrowth(steps, kp, voxelSize, 7, resultsFile,cr_content);
	a.draw3D("Wyniki//TestDraw3D//after_oxidation.bmp");
	resultsFile.close();
	
	return 0;
}