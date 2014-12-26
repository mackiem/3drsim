#include "reconstruct.h"
#include <cassert>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "utils3dr.h"

#define _USE_MATH_DEFINES
#include <math.h>


Reconstruct::Reconstruct()
{
	bmp1 = nullptr;
	bmp2 = nullptr;
}


Reconstruct::~Reconstruct()
{
}

bool Reconstruct::loadImages(std::string filePath1, std::string filePath2) {
	bmp1 = new Bitmap(Bitmap::bitmapFromFile(filePath1));
	bmp2 = new Bitmap(Bitmap::bitmapFromFile(filePath2));
	return true;
}

unsigned char Reconstruct::classifyPixel(Color color) {
	float red = (float) color.r/ 255.f;
	float blue = (float) color.b/ 255.f;
	float green = (float) color.g/ 255.f;

	const float alpha = 0.3;
	int i = 0;
	bool colorFound = false;
	unsigned char ch;
//	for (; i < colorSeq.size(); ++i) {
	for (auto& itr : colorMap) {
		glm::vec3 chkColor = itr.second;
		float cRed = chkColor.r;
		float cGreen = chkColor.g;
		float cBlue = chkColor.b;
		if ((red <= cRed + alpha && red >= cRed - alpha)
			&& (green <= cGreen + alpha && green >= cGreen - alpha)
			&& (blue <= cBlue + alpha && blue >= cBlue - alpha)) {
			colorFound = true;
			ch = itr.first;
			break;
		}
	}

	unsigned char cf = (!colorFound) ? (0) : (ch);

	return cf;
}

void Reconstruct::createColorMap(std::vector<glm::vec3> colorSeq) {
	const int a = 97;
	colorMap.clear();
	for (int i = 0; i < colorSeq.size(); ++i) {
		colorMap.insert(std::make_pair(97 + i, colorSeq[i]));
	}
}

std::unordered_map<int, int> Reconstruct::detEdges(unsigned char* pxlRow, const unsigned int width) {
	std::unordered_map<int, int> edges;
	int edge = 0;
	for (int i = 0; i < width-1; ++i) {
		if (pxlRow[i] != pxlRow[i + 1]) {
			// edge detected
			edges.insert(std::make_pair(edge++, i+1));
		}
	}
	return edges;
}

unsigned char** Reconstruct::classifyPixels(Bitmap *bmp) {
	assert(bmp->format() == Bitmap::Format_RGB);
	unsigned int ht1 = bmp->height();
	const unsigned int wd1 = bmp->width();
	const int comp1 = (int)bmp->format();

	std::ofstream file;
	//file.open("classify.txt");

	unsigned char **clImg = new unsigned char*[ht1];
	
	for (unsigned int j = 0; j < ht1; ++j) {
		clImg[j] = new unsigned char[wd1];
		for (unsigned int i = 0; i < wd1; ++i) {
			unsigned char* px = bmp->getPixel(i, j);
			Color color(px[0], px[1], px[2]);
			clImg[j][i] = classifyPixel(color);
			//file << cfPxl;
		}
		//file << std::endl;
	}
	//file.close();

	return clImg;
}

void Reconstruct::colorPerPixel(std::string& fName, std::unordered_map<int, std::vector<unsigned char>>& corrMap) {

	const int lOffset = 1000;
	const float alpha = 0.01;
	int camHit = 0;
	Bitmap cam1 = Bitmap::bitmapFromFile(fName);
	const int ht1 = cam1.height();
	const int wd1 = cam1.width();
	for (int j = 0; j < ht1 - 1; j = j + lOffset) {
		for (int i = 0; i < wd1 - 1; ++i) {
			unsigned char* px = cam1.getPixel(i, j);
			const int pos = j * wd1 + i;
			auto &itr = corrMap.find(pos);

			float red = (float)px[0] / 255.f;
			float green = (float)px[1] / 255.f;
			float blue = (float)px[2] / 255.f;

			for (auto& colorItr : colorMap) {
				glm::vec3 chkColor = colorItr.second;
				float cRed = chkColor.r;
				float cGreen = chkColor.g;
				float cBlue = chkColor.b;

				if ((red <= cRed + alpha && red >= cRed - alpha)
					&& (green <= cGreen + alpha && green >= cGreen - alpha)
					&& (blue <= cBlue + alpha && blue >= cBlue - alpha)) {
					if (itr != corrMap.end()) {
						auto& seq = itr->second;
						for (int l = 0; l < 3; l++)
						{
							unsigned char ch = px[l] == 0 ? '0' : '1';
							//unsigned char ch = '1';
							seq.push_back(ch);
						}
						camHit++;
					} else {
						std::vector<unsigned char> seq;
						for (int l = 0; l < 3; l++)
						{
							unsigned char ch = px[l] == 0 ? '0' : '1';
							//unsigned char ch = '1';
							seq.push_back(ch);
						}
						corrMap.insert(std::make_pair(pos, seq));
					}
				}

			}
		}
	}

}

// count black pixels
// for each pixel, store values
// for each pixel, check how much is unique

void Reconstruct::correspondAllImages(std::string referenceImage) {
	Bitmap blackImage = Bitmap::bitmapFromFile(referenceImage);

	const int lOffset = 1000;
	std::unordered_map<int, std::vector<unsigned char>> corrMap1;
	std::unordered_map<int, std::vector<unsigned char>> corrMap2;
	const int ht = blackImage.height();
	const int wd = blackImage.width();
	// count number of black pixels
	const float alpha = 0.001;
	int totPxls = 0;
	for (int j = 0; j < ht - 1; j = j + lOffset) {
		for (int i = 0; i < wd - 1; ++i) {
			unsigned char* px = blackImage.getPixel(i, j);
			float red = (float) px[0]/ 255.f;
			float green = (float) px[1]/ 255.f;
			float blue = (float) px[2]/ 255.f;
			float cRed = 0.f;
			float cGreen = 0.f;
			float cBlue = 0.f;
			if ((red <= cRed + alpha && red >= cRed - alpha)
				&& (green <= cGreen + alpha && green >= cGreen - alpha)
				&& (blue <= cBlue + alpha && blue >= cBlue - alpha)) {
				totPxls += 1;
			}
		}
		std::cout << j << " ";
	}

	// open file 1 and file 2 append color code to maps
	
	int suffix = 0;

	std::string fPre("cam");
	std::ifstream fs;
	std::string fName;

	int cam1Hit = 0;
	int cam2Hit = 0;
	int noOfFiles = 0;

	do {
		// camera 1
		fs.close();
		fs.clear();
		int camera = 1;
		fName = fPre;
		fName.append(std::to_string(camera)).append("_").append(std::to_string(suffix)).append(".bmp");
		fs.open(fName);
		if (fs.good()) {
			colorPerPixel(fName, corrMap1);
			noOfFiles++;
		}
		std::cout << "Finished analyzing " << fName << "... " << std::endl;
		fs.close();
		fs.clear();

		// camera 2
		camera = 2;
		fName = fPre;
		fName.append(std::to_string(camera)).append("_").append(std::to_string(suffix)).append(".bmp");
		fs.open(fName);
		if (fs.good()) {
			colorPerPixel(fName, corrMap2);
		}
		std::cout << "Finished analyzing " << fName << "... " << std::endl;
		suffix++;
	} while (fs.good());


	std::cout << "cam1 hit: " << cam1Hit << " cam2 hit: " << cam2Hit << std::endl;

	int correctPxls = 0;
	const int ht1 = 2000;
	const int wd1 = 2000;
	for (int j = 0; j < ht1 - 1; j = j + lOffset) {
		for (int i = 0; i < wd1 - 1; ++i) {
			const int pos = j * wd1 + i;
			auto &itr = corrMap1.find(pos);
			if (itr != corrMap1.end()) {
				int kfound = 0;
				for (int k = 0; k < wd1 - 1; ++k) {
					const int kPos = j * wd1 + k;
					auto &kItr = corrMap2.find(kPos);
					if (kItr != corrMap2.end()) {
						kfound++;
						//std::string str1(itr->second.begin(), itr->second.end());
						//std::string str2(kItr->second.begin(), kItr->second.end());
						//std::cout << str1 << " " << str2 << std::endl;
						if (itr->second == kItr->second) {
							correctPxls++;
						}
					}
				}
				//std::cout << "For j, i : " << j << "," << i << "K hits: " << kfound << std::endl;
			}
		}
	}

	std::ofstream results;
	results.open("results.txt", std::fstream::app);

	std::setprecision(3);
	std::cout << "Correct pixels: " << correctPxls << " total Pixels: " << totPxls << std::endl;
	std::cout << "Correct pixel code: " << (float)correctPxls / (float)totPxls * 100.f<< "%" << std::endl;
	std::cout << "Avg. no of corresponding pixels: " << (float)correctPxls / (float)corrMap1.size() << "" << std::endl;
	results << "No of. Files: " << noOfFiles << ", Avg. no of corresponding pixels: " << (float)correctPxls / (float)corrMap1.size() << std::endl;

}

void Reconstruct::setCameraParams(float fovy1, float aspect1, float nearPlane1, glm::vec3 eye1, glm::vec3 center1, glm::vec3 up1, 
	float fovy2, float aspect2, float nearPlane2, glm::vec3 eye2, glm::vec3 center2, glm::vec3 up2) {
	Camera cam1;
	cam1.aspect = aspect1;
	cam1.fovy = fovy1;
	cam1.nearPlane = nearPlane1;
	cam1.eye = eye1;
	cam1.center = center1;
	cam1.up = up1;
	cam1.right = glm::cross(center1 - eye1, up1);

	Camera cam2;
	cam2.aspect = aspect2;
	cam2.fovy = fovy2;
	cam2.nearPlane = nearPlane2;
	cam2.eye = eye2;
	cam2.center = center2;
	cam2.up = up2;
	cam2.right = glm::cross(center2 - eye2, up2);

	this->cam1 = cam1;
	this->cam2 = cam2;
}

	const int offset = 10;

void Reconstruct::classifyPixels() {
	unsigned char** clBmp1 = classifyPixels(bmp1);
	for (int j = 0; j < bmp1->height(); j += offset) {
		edges1.insert(std::make_pair(j, detEdges(clBmp1[j], bmp1->width())));
	}

	//	std::cout << std::endl;
	//for (int i = 690; i < 885; ++i) {
	//	std::cout << clBmp1[1000][i];
	//}
	//std::cout << std::endl;

	//unsigned char* px = bmp1->getPixel(700, 1000);
	//unsigned char pxl = classifyPixel(Color(px[0], px[1], px[2]));
	//std::cout <<  pxl << std::endl;

	//auto e1000 = detEdges(clBmp1[1000], 2000);

	for (auto& itr : edges1.find(1000)->second) {
		std::cout << "Edge " << itr.first << " : " << itr.second << std::endl;
	}

	unsigned char** clBmp2 = classifyPixels(bmp2);
	for (int j = 0; j < bmp2->height(); j += offset) {
		edges2.insert(std::make_pair(j, detEdges(clBmp2[j], bmp2->width()) ));
	}

	for (auto& itr : edges2.find(1000)->second) {
		std::cout << "Edge " << itr.first << " : " << itr.second << std::endl;
	}

	for (int j = 0; j < bmp1->height(); ++j) {
		delete[] clBmp1[j];
	}
	delete[] clBmp1;

	for (int j = 0; j < bmp2->height(); ++j) {
		delete[] clBmp2[j];
	}
	delete[] clBmp2;
}

void findEdges();

glm::vec3 Reconstruct::iPoint(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	//let a = dot(d0, d0)
	//	b = dot(d0, d1)
	//	c = dot(d1, d1)
	//	d = dot(d0, (P0 - P1))
	//	e = dot(d1, (P0 - P1))
	//	f = dot((P0 - P1), (P0 - P1))

	//	Thus we have 2 eqns in 2 unknowns, solution is

	//	sc = (b*e - c*d) / (a*c - b ^ 2)
	//	tc = (a*e - b*d) / (a*c - b ^ 2)

	float p = glm::dot(b, b);
	float q = glm::dot(b, d);
	float r = glm::dot(d, d);
	
	glm::vec3 f = a - c;

	float s = glm::dot(b, f);
	float t = glm::dot(d, f);
	float u = glm::dot(f, f);

	float denom = p*r - pow(q, 2);
	if (denom < 1.0e-6) {
		throw std::runtime_error("Lines are parallel");
	}
	float	sc = (q*t - r*s) / denom;
	float	tc = (p*t - q*s) / denom;

	glm::vec3 mid = 0.5f * (a + sc * b + c + tc * d);
	return mid;
}

glm::vec3 Reconstruct::nPlaneiPoint(float nearPlaneWd, float nearPlaneHt, float edgeLoc, float j, Camera cam1) {
	float wd = nearPlaneWd * (float)edgeLoc/ (float)bmp1->width();
	// from lower left
	float ht = nearPlaneHt * (float)j / (float)bmp1->height();

	float yFromMid = -(nearPlaneHt) / 2.f + ht;
	float xFromMid = -(nearPlaneWd) / 2.f + wd;

	glm::vec3 center = cam1.eye + cam1.nearPlane * (cam1.center - cam1.eye);
	glm::vec3 pntB = center + glm::normalize(cam1.right) * xFromMid;
	pntB += glm::normalize(cam1.up) * yFromMid;
	return pntB;
}

void Reconstruct::reconstruct() {
	float nearPlaneHt1 = cam1.nearPlane * tan(180.f / M_PI * (cam1.fovy / 2.f));
	float nearPlaneWd1 = cam1.aspect * nearPlaneHt1;

	float nearPlaneHt2 = cam2.nearPlane * tan(180.f / M_PI * (cam2.fovy / 2.f));
	float nearPlaneWd2 = cam2.aspect * nearPlaneHt2;

	// widths are equal, start with 1000 :)
	// for edge 1, shoot ray

	std::vector<glm::vec3> pnts;

	// create correspondence struct
	const int resizeFactor = 20;
	const int ht1 = bmp1->height() / resizeFactor;
	const int wd1 = bmp1->width() / resizeFactor;
	const int comp = 3;
	unsigned char* corr = new unsigned char[ht1 * wd1 * comp];


	for (unsigned int j = 0; j < ht1; ++j) {
		for (unsigned int i = 0; i < wd1; ++i) {
			for (unsigned int k = 0; k < 3; ++k) {
				corr[(j * ht1 + i) * comp + k] = 255;
			}
		}
	}


	for (int j = 0; j < bmp1->width(); j+=offset) {
		auto& edgesForHt1 = edges1.find(j)->second;
		auto& edgesForHt2 = edges2.find(j)->second;
		if (edgesForHt1.size() != edgesForHt2.size()) {
			// insert a pixel of red rows
			for (int i = 0; i < edgesForHt1.size(); ++i) {
				auto& edge1 = edgesForHt1.find(i);
				if (edge1 != edgesForHt1.end()) {
					int wdVal = edge1->second / resizeFactor;
					int htVal = j / resizeFactor;
					for (unsigned int k = 0; k < 3; ++k) {
						if (k == 0) {
							corr[(htVal * ht1 + wdVal) * comp + k] = 255;
						}
						else {
							corr[(htVal * ht1 + wdVal) * comp + k] = 0;
						}
					}
				
				}
			}
			continue;
		}
		for (int i = 0; i < edgesForHt1.size(); ++i) {
			auto& edge1 = edgesForHt1.find(i);
			if (edge1 != edgesForHt1.end()) {
				glm::vec3 pntB = nPlaneiPoint(nearPlaneWd1, nearPlaneHt1, edge1->second, j, cam1);
				glm::vec3 pntA(cam1.eye);
				glm::vec3 vecB(pntB - pntA);

				auto& edge2 = edgesForHt2.find(i);
				if (edge2 != edgesForHt2.end()) {
					glm::vec3 pntD = nPlaneiPoint(nearPlaneWd2, nearPlaneHt2, edge2->second, j, cam2);
					glm::vec3 pntC(cam2.eye);
					glm::vec3 vecD(pntD - pntC);

					try {
						pnts.push_back(iPoint(pntA, vecB, pntC, vecD));
						// insert green pixel
						int wdVal = edge1->second / resizeFactor;
						int htVal = j / resizeFactor;
						for (unsigned int k = 0; k < 3; ++k) {
							if (k == 1) {
								corr[(htVal * ht1 + wdVal) * comp + k] = 255;
							}
							else {
								corr[(htVal * ht1 + wdVal) * comp + k] = 0;
							}
						}
					}
					catch (std::runtime_error &e) {
						// ignore - parallel lines
						// insert red pixel
						int wdVal = edge1->second / resizeFactor;
						int htVal = j / resizeFactor;
						for (unsigned int k = 0; k < 3; ++k) {
							if (k == 0) {
								corr[(htVal * ht1 + wdVal) * comp + k] = 255;
							}
							else {
								corr[(htVal * ht1 + wdVal) * comp + k] = 0;
							}
						}
					}

				} else {
					throw std::runtime_error("Edge not found in edge data struct");
				}
			}
			else {
				throw std::runtime_error("Edge not found in edge data struct");
			}
		}
		if (j == 1360) {

		std::cout << "completed :" << j << std::endl;
		}
		std::cout << "completed :" << j << std::endl;
	}

	Utils3DR::saveImage(wd1, ht1, corr, "corr.bmp");

	std::ofstream reconFile;
	reconFile.open("output.txt");
	for (auto& pnt : pnts) {
		for (int i = 0; i < 3; ++i) {
			reconFile << pnt[i];
			if (i != 2) {
				reconFile << ", ";
			}
		}
		reconFile << std::endl;
	}
	reconFile.close();


}

void Reconstruct::copyPixelRow(Bitmap* bmp, int row, unsigned char*& destBuf, const int wd, const int comp) {
		destBuf = new unsigned char[wd * comp];
		for (int i = 0; i < wd; ++i) {
			unsigned char* px = bmp->getPixel(row, i);
			for (int k = 0; k < comp; ++k) {
				destBuf[i * comp + k] = px[k];
			}
		}
}

int* Reconstruct::corrRow(unsigned char* pxRow1, const int wd1, unsigned char *pxRow2, const int wd2) {
	float **opt = new float*[wd1];
	for (int i = 0; i < wd1; ++i) {
		(opt)[i] = new float[wd2];
	}

	// initialize to -1
	for (int j = 0; j < wd1; ++j) {
		for (int i = 0; i < wd2; ++i) {
			opt[j][i] = -1.f;
		}
	}

	int *corr = new int[wd1];
	calcOpt(opt, corr, pxRow1, wd1 - 1, pxRow2, wd2 - 1);

	for (int i = 0; i < wd1; ++i) {
		delete[] (opt)[i];
	}
	delete[] opt;

	return corr;
}

float clamp(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

float consistency(const int q, const float e) {
	const float a = 0.1f;
	const float b = 0.9f;
	if (q == 1) {
		return clamp((e - a) / (b - a), -1.f, 1.f);
	}
	else if (q == 0) {
		return clamp(1 - ((std::abs(e) - a) / (b - a)), -1.f, 1.f);
	} 
	else if (q == -1) {
		return consistency(1, -e);
	}
	else {
		throw std::runtime_error("Unexpected value");
	}
}

float Reconstruct::score(unsigned char* pxRow1, unsigned char* pxRow2, const int j, const int i, const int comp) {
	if ((j == 0) || (i == 0)) {
		return 0.f;
	}
	unsigned char rNow = pxRow1[j * comp];
	unsigned char rPrev = pxRow1[(j-1) * comp];

	unsigned char gNow = pxRow1[(j * comp) + 1];
	unsigned char gPrev = pxRow1[((j-1) * comp) + 1];

	unsigned char bNow = pxRow1[(j * comp) + 2];
	unsigned char bPrev = pxRow1[((j-1) * comp) + 2];

	int qR1 = (float)std::round(((float)(rNow - rPrev))/255.f);
	int qG1 = (float)std::round(((float)(gNow - gPrev))/255.f);
	int qB1 = (float)std::round(((float)(bNow - bPrev))/255.f);

	unsigned char rNow2 = pxRow2[j * comp];
	unsigned char rPrev2 = pxRow2[(j-1) * comp];

	unsigned char gNow2 = pxRow2[(j * comp) + 1];
	unsigned char gPrev2 = pxRow2[((j-1) * comp) + 1];

	unsigned char bNow2 = pxRow2[(j * comp) + 2];
	unsigned char bPrev2 = pxRow2[((j-1) * comp) + 2];

	float eR2 = (float)(rNow2 - rPrev2) / 255.f;
	float eG2 = (float)(gNow2 - gPrev2) / 255.f;
	float eB2 = (float)(bNow2 - bPrev2) / 255.f;

	float conR = consistency(qR1, eR2);
	float conG = consistency(qG1, eG2);
	float conB = consistency(qB1, eB2);

	return std::min(std::min(conR, conG), conB);
}

float Reconstruct::calcOpt(float **opt, int *corr, unsigned char* pxRow1, int j, unsigned char* pxRow2, int i) {
	if (opt[j][i] >= 0.f) {
		return opt[j][i];
	}
	if ((i == 0) || (j == 0)) {
		opt[j][i] = 0;
		return opt[j][i];
	}

	float currScore = calcOpt(opt, corr, pxRow1, j - 1, pxRow2, i - 1) + score(pxRow1, pxRow2, j, i);
	float scorePrevJ = calcOpt(opt, corr, pxRow1, j - 1, pxRow2, i);
	float scorePrevI = calcOpt(opt, corr, pxRow1, j, pxRow2, i - 1);

	if ((scorePrevJ <= currScore) && (scorePrevI <= currScore)) {
		opt[j][i] = currScore;
		corr[j] = i;
	}
	else if ((currScore <= scorePrevJ) && (scorePrevI <= scorePrevJ)) {
		opt[j][i] = scorePrevJ;
		corr[j] = corr[j - 1];
	}
	else if ((currScore <= scorePrevI) && (scorePrevJ <= scorePrevI)) {
		opt[j][i] = scorePrevI;
		corr[j] = i - 1;
	}
	else {
		throw std::runtime_error("Unknown condition occured;");
	}

	return opt[j][i];
}

int** Reconstruct::correspondImages() {
	assert(bmp1->format() == Bitmap::Format_RGB);
	unsigned int ht1 = bmp1->height();
	const unsigned int wd1 = bmp1->width();
	const int comp1 = (int)bmp1->format();

	const int ht2 = bmp2->height();
	const int wd2 = bmp2->width();
	const int comp2 = bmp2->format();
	int** corrMatrix = new int*[ht1];
	//for (unsigned int j = 0; j < ht1; ++j) {
	for (unsigned int j = 0; j < 5; ++j) {
		// copy a pixel row
		unsigned char* pxRow1 = nullptr;
		copyPixelRow(bmp1, j, pxRow1, wd1);

		unsigned char* pxRow2 = nullptr;
		copyPixelRow(bmp2, j, pxRow2, wd2);

		corrMatrix[j] = corrRow(pxRow1, wd1, pxRow2, wd2);
	}
	return corrMatrix;
}
