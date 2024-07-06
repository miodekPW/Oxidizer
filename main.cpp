#include "EasyBMP.h"
#include "matrix.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
	BMP image;
	string strTemp;
	float porowatosc;
	float cr_content = 27.0; //Initial Cr content in the alloy
	double kp = 1.3e-14; // Parabolic rate law constant - kinetics of the oxidation
	double voxelSize = 0.175; //resolution of the image data = thickness of the oxide layer after one step of oxidation
	int steps = 22; //number of oxidation steps for dilation/erosion procedure
	string dir = "SEM//1//"; //directory of the input image data
	string przedrostek = "Fe22Cr_0001"; //file name - only bitmap data supported
	fstream plik("SEM//1//resultsFe27Cr.csv", ios::out); //directory and filename for results export

	//load image
	strTemp = dir + przedrostek + ".bmp";
	//strTemp = "Model4//Fe22Cr_128_duze_4_000.bmp";
	
	matrix a;
	//a.load3Ddata(strTemp, 1);
	
	a.load2Ddata(strTemp, 1);
	
	a.oxideGrowth(steps, kp, voxelSize, 7, plik,cr_content);
	a.draw2D(0, "SEM//1//after_oxidation_steps.bmp");
	plik.close();
	
	
	
	
	return 0;
}