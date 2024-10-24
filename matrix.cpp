#include "matrix.h"
#include "EasyBMP.h"
#include <queue>
#include <math.h>
#include <string>
#include <fstream>

//#include "voxel.h"
using namespace std;

void matrix::load2Ddata(string dir, int treshold)
{
	//load image to determine dimensions
	BMP image;
	image.ReadFromFile(dir.c_str());
	xMax = image.TellWidth();
	yMax = image.TellHeight();
	zMax = 1;
	cout << "width: " << yMax << " height: " << yMax << endl;
	//finish loading image and dimension
	
	//create matrix
	data = new voxel** [xMax];
	for (int x = 0; x < xMax; ++x) {
		data[x] = new voxel* [yMax];
		for (int y = 0; y < yMax; ++y) {
			data[x][y] = new voxel [zMax];
			
		}
	}
	cout << "Matrix created\n";
	//matrix with voxel data created

	//pixel assignment
	for (int j = 0; j < xMax; j++) {
		for (int k = 0; k < yMax; k++) {
			if (image.GetPixel(j, k).Blue >= treshold && image.GetPixel(j, k).Green >= treshold && image.GetPixel(j, k).Red >= treshold) {
				//cout << "Loaded material\n";
				data[j][k][0].setValues(2, j, k, 0);
				n_steel++;
			}
			else {
				//cout << "Loaded pore\n";
				data[j][k][0].setValues(0, j, k, 0);
				
			}
		}
	}
	//finished pixel assignment
}


void matrix::load3Ddata(string dir, int treshold)
{
	string temp_str, temp_str2;
	int n, iter;
	BMP image;
	char const* digits = "0123456789";
	size_t start_pos = dir.find_first_of("0");
	
	image.ReadFromFile(dir.c_str());
	xMax = image.TellWidth();
	yMax = image.TellHeight();
	if (start_pos != std::string::npos)
	{
		size_t end_pos = dir.find_first_of(".");
		temp_str=dir.substr(start_pos, end_pos - start_pos);
		
		n = pow(10, temp_str.length());
		cout << temp_str << ","<<n << endl;
	}
	else
	{
		cout << "Wrong filename format for sequence of images" << endl;
		return;
	}
	

	for (int i = stoi(temp_str); i < n; i++)
	{
		temp_str2 = to_string(i);
		while (temp_str2.length() < temp_str.length()) {
			temp_str2.insert(0, "0");
		}
		temp_str2.insert(0, dir.substr(0, start_pos));
		temp_str2 += ".bmp";
		//cout << temp_str2 << endl;
		ifstream file(temp_str2.c_str());
		if (!file.good()) {
			zMax = i;
			break;
		}
		file.close();
	}
	cout << "xmax: " <<xMax<< ", ymax: " << yMax << ", zmax: " << zMax << endl;
	
	data = new voxel * *[xMax];
	for (int x = 0; x < xMax; ++x) {
		data[x] = new voxel * [yMax];
		for (int y = 0; y < yMax; ++y) {
			data[x][y] = new voxel[zMax];
		}
	}
	cout << "Memory allocated\n";

	for (int i = 0; i < zMax; i++) {
		temp_str2 = to_string(i);
		while (temp_str2.length() < temp_str.length()) {
			temp_str2.insert(0, "0");
		}
		temp_str2.insert(0, dir.substr(0, start_pos));
		temp_str2 += ".bmp";
		image.ReadFromFile(temp_str2.c_str());

		for (int j = 0; j < xMax; j++) {
			for (int k = 0; k < yMax; k++) {

				if (image.GetPixel(j, k).Blue >= treshold && image.GetPixel(j, k).Green >= treshold && image.GetPixel(j, k).Red >= treshold) {
					//cout << "Loaded material\n";
					n_steel++;
					data[j][k][i].setValues(2, j, k, i);
				}
				else {
					//cout << "Loaded pore\n";
					data[j][k][i].setValues(0, j, k, i);

				}
			}
		}

	}
	
}

float matrix::porosity()
{	
	float por = 0;
	int sum = 0;
	for (int i = 0; i < xMax; i++) {
		for (int j = 0; j < yMax; j++) {
			for (int k = 0; k < zMax; k++) {
				if (data[i][j][k].getAssignment() == 0 || data[i][j][k].getAssignment() == 1) {
					sum++;
				}
			}
		}
	}
	por = (float)sum / (float)( xMax*yMax*zMax);
	return por;
}
/**
*Dilate structure
* @param direction defines direction for pore connectivity calculations
* 1 - x0 ; 2 - xmax ; 3 - y0; 4 - ymax; 5 - z0; 6 -all; 7 - whole volume
*/
void matrix::dilate(int direction)
{
	
	queue<voxel> q;
	//reset connected pores
	for (int i = 0; i < xMax; i++) {
		for (int j = 0; j < yMax; j++) {
			for (int k = 0; k < zMax; k++) {
				data[i][j][k].setProcessed(false);
				if (data[i][j][k].getAssignment() == 1) {
					data[i][j][k].setAssigment(0);
				}
			}
		}
	}
	//initial connected pores
	switch (direction) {
		//1 - x0 ; 2 - xmax ; 6 - all; 7 - whole volume
	case 1:
		for (int i = 0; i < yMax;i++) {
			for (int j = 0; j < zMax; j++) {
				if (data[0][i][j].getAssignment() == 0) {
					data[0][i][j].setAssigment(1);
					q.push(data[0][i][j]);
					data[0][i][j].setProcessed(true);
				}
			}
		}
		break;
	case 2:
		for (int i = 0; i < yMax; i++) {
			for (int j = 0; j < zMax; j++) {
				if (data[(xMax - 1)][i][j].getAssignment() == 0) {
					data[(xMax - 1)][i][j].setAssigment(1);
					q.push(data[(xMax - 1)][i][j]);
					data[(xMax - 1)][i][j].setProcessed(true);
					
				}
			}
		}
		break;
	case 6:
		for (int i = 0; i < yMax; i++) {
			for (int j = 0; j < zMax; j++) {
				if (data[0][i][j].getAssignment() == 0) {
					data[0][i][j].setAssigment(1);
					q.push(data[0][i][j]);
					data[0][i][j].setProcessed(true);
				}
			}
		}
		for (int i = 0; i < yMax; i++) {
			for (int j = 0; j < zMax; j++) {
				if (data[(xMax - 1)][i][j].getAssignment() == 0) {
					data[(xMax - 1)][i][j].setAssigment(1);
					q.push(data[(xMax - 1)][i][j]);
					data[(xMax - 1)][i][j].setProcessed(true);

				}
			}
		}
		for (int i = 0; i < xMax; i++) {
			for (int j = 0; j < zMax; j++) {
				if (data[i][0][j].getAssignment() == 0) {
					data[i][0][j].setAssigment(1);
					q.push(data[i][0][j]);
					data[i][0][j].setProcessed(true);
				}
			}
		}
		for (int i = 0; i < xMax; i++) {
			for (int j = 0; j < zMax; j++) {
				if (data[i][(yMax - 1)][j].getAssignment() == 0) {
					data[i][(yMax - 1)][j].setAssigment(1);
					q.push(data[i][(yMax - 1)][j]);
					data[i][(yMax - 1)][j].setProcessed(true);
				}
			}
		}
		for (int i = 0; i < xMax; i++) {
			for (int j = 0; j < yMax; j++) {
				if (data[i][j][0].getAssignment() == 0) {
					data[i][j][0].setAssigment(1);
					q.push(data[i][j][0]);
					data[i][j][0].setProcessed(true);
				}
			}
		}
		for (int i = 0; i < xMax; i++) {
			for (int j = 0; j < yMax; j++) {
				if (data[i][j][(zMax - 1)].getAssignment() == 0) {
					data[i][j][(zMax - 1)].setAssigment(1);
					q.push(data[i][j][(zMax - 1)]);
					data[i][j][(zMax - 1)].setProcessed(true);
				}
			}
		}

		break;
	case 7:
		for (int i = 0; i < xMax; i++) {
			for (int j = 0; j < yMax; j++) {
				for (int k = 0; k < zMax; k++) {
					if (data[i][j][k].getAssignment() == 0) {
						data[i][j][k].setAssigment(1);
					}
				}
			}
		}
		break;
	default:
		cout << "Wrong direction!" << endl;
		break;
	}
	//connected pore space
	while (!q.empty()) {
		
		//X-1
		if (q.front().x > 0) {
			if (data[q.front().x - 1][q.front().y][q.front().z].getAssignment() == 0 && !data[q.front().x - 1][q.front().y][q.front().z].isProcessed()) {
				data[q.front().x - 1][q.front().y][q.front().z].setAssigment(1);
				q.push(data[q.front().x - 1][q.front().y][q.front().z]);
				data[q.front().x - 1][q.front().y][q.front().z].setProcessed(true);
			}
		}
		//X+1
		if ((q.front().x+1) < xMax) {
			if (data[q.front().x + 1][q.front().y][q.front().z].getAssignment() == 0 && !data[q.front().x + 1][q.front().y][q.front().z].isProcessed()) {
				data[q.front().x + 1][q.front().y][q.front().z].setAssigment(1);
				q.push(data[q.front().x + 1][q.front().y][q.front().z]);
				data[q.front().x + 1][q.front().y][q.front().z].setProcessed(true);
			}
		}
		//Y-1
		if (q.front().y > 0) {
			if (data[q.front().x][q.front().y - 1][q.front().z].getAssignment() == 0 && !data[q.front().x][q.front().y - 1][q.front().z].isProcessed()) {
				data[q.front().x][q.front().y - 1][q.front().z].setAssigment(1);
				q.push(data[q.front().x][q.front().y - 1][q.front().z]);
				data[q.front().x][q.front().y - 1][q.front().z].setProcessed(true);
			}
		}
		//Y+1
		if ((q.front().y+1) < yMax) {
			if (data[q.front().x][q.front().y + 1][q.front().z].getAssignment() == 0 && !data[q.front().x][q.front().y + 1][q.front().z].isProcessed()) {
				data[q.front().x][q.front().y + 1][q.front().z].setAssigment(1);
				q.push(data[q.front().x][q.front().y + 1][q.front().z]);
				data[q.front().x][q.front().y + 1][q.front().z].setProcessed(true);
			}
		}
		//Z-1
		if (q.front().z > 0) {
			if (data[q.front().x][q.front().y][q.front().z - 1].getAssignment() == 0 && !data[q.front().x][q.front().y][q.front().z - 1].isProcessed()) {
				data[q.front().x][q.front().y][q.front().z - 1].setAssigment(1);
				q.push(data[q.front().x][q.front().y][q.front().z - 1]);
				data[q.front().x][q.front().y][q.front().z - 1].setProcessed(true);
			}
		}
		//Z+1
		if ((q.front().z + 1) < yMax) {
			if (data[q.front().x][q.front().y][q.front().z + 1].getAssignment() == 0 && !data[q.front().x][q.front().y][q.front().z + 1].isProcessed()) {
				data[q.front().x][q.front().y][q.front().z + 1].setAssigment(1);
				q.push(data[q.front().x][q.front().y][q.front().z + 1]);
				data[q.front().x][q.front().y][q.front().z + 1].setProcessed(true);
			}
		}
		q.pop();
	}

	for (int i = 0; i < xMax; i++) {
		for (int j = 0; j < yMax; j++) {
			for (int k = 0; k < zMax; k++) {
				data[i][j][k].setProcessed(false);
				
			}
		}
	}
	//check pixels to be dilated
	for (int i = 0; i < xMax; i++) {
		for (int j = 0; j < yMax; j++) {
			for (int k = 0; k < zMax; k++) {
				if (data[i][j][k].getAssignment() == 1) {
					//X-1
					if (i>0) {
						if ((data[i-1][j][k].getAssignment()==2 || data[i - 1][j][k].getAssignment() == 3)&& !data[i - 1][j][k].isProcessed()) {
							//d.push_back(data[i][j][k]);
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
							
						}
					}
					//X+1
					if ((i+1)<xMax) {
						if ((data[i + 1][j][k].getAssignment() == 2 || data[i + 1][j][k].getAssignment() == 3) && !data[i + 1][j][k].isProcessed()) {
							//d.push_back(data[i][j][k]);
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
							
						}
					}
					//Y-1
					if (j > 0) {
						if ((data[i][j - 1][k].getAssignment() == 2 || data[i][j - 1][k].getAssignment() == 3) && !data[i][j - 1][k].isProcessed()) {
							//d.push_back(data[i][j][k]);
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
							
						}
					}
					//Y+1
					if ((j + 1) < yMax) {
						if ((data[i][j + 1][k].getAssignment() == 2 || data[i][j + 1][k].getAssignment() == 3) && !data[i][j + 1][k].isProcessed()) {
							//d.push_back(data[i][j][k]);
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
							
						}
					}
					//Z-1
					if (k > 0) {
						if ((data[i][j][k - 1].getAssignment() == 2 || data[i][j][k - 1].getAssignment() == 3) && !data[i][j][k - 1].isProcessed()) {
							//d.push_back(data[i][j][k]);
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
							
						}
					}
					//Z+1
					if ((k + 1) < zMax) {
						if ((data[i][j][k + 1].getAssignment() == 2 || data[i][j][k + 1].getAssignment() == 3) && !data[i][j][k + 1].isProcessed()) {
							//d.push_back(data[i][j][k]);
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
							
						}
					}
					
					/*
					//diagonals 2D
					if (i > 0 && j > 0) {
						if (data[i - 1][j-1][k].getAssignment() == 2 || data[i - 1][j-1][k].getAssignment() == 3 && !data[i-1][j-1][k].isProcessed()) {
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					if (i > 0 && j + 1 < yMax) {
						if (data[i - 1][j+1][k].getAssignment() == 2 || data[i - 1][j+1][k].getAssignment() == 3 && !data[i - 1][j + 1][k].isProcessed()) {
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					if (i + 1 < xMax && j > 0) {
						if (data[i + 1][j-1][k].getAssignment() == 2 || data[i + 1][j-1][k].getAssignment() == 3 && !data[i + 1][j - 1][k].isProcessed()) {
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					if (i + 1 < xMax && j + 1 < yMax) {
						if (data[i + 1][j + 1][k].getAssignment() == 2 || data[i + 1][j + 1][k].getAssignment() == 3 && !data[i + 1][j + 1][k].isProcessed()) {
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					*/
				}
			}
		}
	}
}

void matrix::erosion()
{
	//reset processed voxels
	for (int i = 0; i < xMax; i++) {
		for (int j = 0; j < yMax; j++) {
			for (int k = 0; k < zMax; k++) {
				data[i][j][k].setProcessed(false);
			}
		}
	}

	//erode
	for (int i = 0; i < xMax; i++) {
		for (int j = 0; j < yMax; j++) {
			for (int k = 0; k < zMax; k++) {
				if (data[i][j][k].getAssignment() == 2) {
					//X-1
					if (i > 0) {
						if (data[i - 1][j][k].getAssignment() == 3 && !data[i - 1][j][k].isProcessed()) {
							if (!data[i][j][k].isProcessed()) {
								n_retracted++;
							}
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					//X+1
					if ((i + 1) < xMax) {
						if (data[i + 1][j][k].getAssignment() == 3 && !data[i + 1][j][k].isProcessed()) {
							if (!data[i][j][k].isProcessed()) {
								n_retracted++;
							}
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					//Y-1
					if (j > 0) {
						if (data[i][j - 1][k].getAssignment() == 3 && !data[i][j - 1][k].isProcessed()) {
							if (!data[i][j][k].isProcessed()) {
								n_retracted++;
							}
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					//Y+1
					if ((j + 1) < yMax) {
						if (data[i][j + 1][k].getAssignment() == 3 && !data[i][j + 1][k].isProcessed()) {
							if (!data[i][j][k].isProcessed()) {
								n_retracted++;
							}
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					//Z-1
					if (k > 0) {
						if (data[i][j][k - 1].getAssignment() == 3 && !data[i][j][k - 1].isProcessed()) {
							if (!data[i][j][k].isProcessed()) {
								n_retracted++;
							}
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}
					//Z+1
					if ((k + 1) < zMax) {
						if (data[i][j][k + 1].getAssignment() == 3 && !data[i][j][k + 1].isProcessed()) {
							if (!data[i][j][k].isProcessed()) {
								n_retracted++;
							}
							data[i][j][k].setOxide();
							data[i][j][k].setProcessed(true);
						}
					}

				}
			}
		}
	}
}

float matrix::calculateDilationTime(int step, double kp, double voxelSize)
{
	double thickness = static_cast<double>(step) * voxelSize;
	double mO = 48;
	double mcr2o3 = 152;
	double time = (thickness / 10000 * densityOxide * mO / mcr2o3) * (thickness / 10000 * densityOxide * mO / mcr2o3) / kp / 60 / 60;
	return static_cast<float>(time);
}

float matrix::calculateErosionTime(int step, double kp, double voxelSize)
{
	double thickness = static_cast<double>(step) * voxelSize;
	double mO = 48;
	double mCr = 104;
	double densityCr = 7.192;
	double time = (thickness / 10000 * densityCr * mO / mCr) * (thickness / 10000 * densityCr * mO / mCr) / kp / 60 / 60;
	return static_cast<float>(time);
}

void matrix::oxideGrowth(int nrSteps, double kp, double voxelSize, int direction, fstream &results,float cr_content)
{
	deque<float> dilationTimes;
	deque<float> erosionTimes;
	
	for (int i = 0; i < nrSteps; i++) {
		dilationTimes.push_back(calculateDilationTime(i+1,kp,voxelSize));
		erosionTimes.push_back(calculateErosionTime(i+1, kp, voxelSize));
	}
	for (int i = 0; i < dilationTimes.size(); i++) {
		cout << "Step: " << i << ", Dilation time: " << dilationTimes[i] << ", Erosion time: " << erosionTimes[i] << endl;
	}

	results << "Dilation step;Oxidation time [h];Porosity [%]; Cr content [%]" << endl;
	results << 0 << ";" << 0 << ";" << to_string(porosity() * 100) << ";" << calculateCrContent(cr_content) << endl;

	for (int i = 0; i < nrSteps; i++) {
		if (dilationTimes.front() < erosionTimes.front()) {
			//dilate
			cout << "DIlation! Time: " <<dilationTimes.front()<< endl;
			dilate(7);
			results << i + 1 << ";" << dilationTimes.front() << ";" << to_string(porosity() * 100) << ";" << calculateCrContent(cr_content) << ";" << endl;

			dilationTimes.pop_front();
		}
		else {
			//erode
			cout << "Erosion! Time: "<<dilationTimes.front() << endl;
			erosion();
			results << i + 1 << ";" << dilationTimes.front() << ";" << to_string(porosity() * 100) << ";" << calculateCrContent(cr_content) << endl;

			dilationTimes.pop_front();
			erosionTimes.pop_front();
		}
	}
}

void matrix::draw2D(int zlayer, string path)
{
	BMP image;
	image.SetSize(xMax, yMax);
	image.SetBitDepth(4);
	for (int l = 0; l < xMax; l++) {
		for (int k = 0; k < yMax; k++) {
			//0 - unconnected pore; 1 - connected pore; 2 - steel; 3 - oxide
			//unconnected - gray
			if (data[l][k][zlayer].getAssignment() == 0) {
				image(l, k)->Red = 0;
				image(l, k)->Blue = 0;
				image(l, k)->Green = 0;
			}
			//connected - green
			if (data[l][k][zlayer].getAssignment() == 1) {
				image(l, k)->Red = 0;
				image(l, k)->Blue = 0;
				image(l, k)->Green = 0;
			}

			//material - blue
			if (data[l][k][zlayer].getAssignment() == 2) {
				image(l, k)->Red = 0;
				image(l, k)->Blue = 255;
				image(l, k)->Green = 0;
			}
			//oxide - red
			if (data[l][k][zlayer].getAssignment() == 3) {
				image(l, k)->Red = 255;
				image(l, k)->Blue = 0;
				image(l, k)->Green = 0;
			}
		}
	}
	string extension = ".bmp";
	string path2=path.substr(0, path.length() - extension.length());
	path2 += to_string(zlayer);
	path2 += extension;
	image.WriteToFile(path2.c_str());
}

void matrix::draw3D(string path)
{	
	string filename;
	BMP image;
	image.SetSize(xMax, yMax);
	image.SetBitDepth(8);
	for (int i = 0; i < zMax; i++) {
		draw2D(i, path);
	}
}

float matrix::calculateCrContent(float initial_cr_content)
{
	int n_oxide = 0;
	int n_steel_after = 0;
	float cr_content;
	for (int i = 0; i < xMax; i++) {
		for (int j = 0; j < yMax; j++) {
			for (int k = 0; k < zMax; k++) {
				if (data[i][j][k].getAssignment() == 2) {
					n_steel_after++;
				}
				if (data[i][j][k].getAssignment() == 3) {
					n_oxide++;
				}
			}
		}
	}
	cr_content = (initial_cr_content / 100 * static_cast<float>(n_steel) * densitySteel - 104. / 152. * static_cast<float>(n_oxide) * densityOxide) / static_cast<float>(n_steel_after) / densitySteel;
	return cr_content * 100;
}


