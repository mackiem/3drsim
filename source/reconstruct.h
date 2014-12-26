#pragma once
#include <string>
#include "tdogl/Bitmap.h"
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>

using namespace tdogl;

class Reconstruct
{
private:
	struct Color {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		Color(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {};
	};

	struct Camera {
		float fovy;
		float aspect;
		float nearPlane;
		glm::vec3 eye;
		glm::vec3 center;
		glm::vec3 right;
		glm::vec3 up;
	};

	Camera cam1;
	Camera cam2;
	Bitmap *bmp1;
	Bitmap *bmp2;
	unsigned char* corresp;
	std::unordered_map<unsigned char, glm::vec3> colorMap;
	void copyPixelRow(Bitmap* bmp, int row, unsigned char*& destBuf, const int wd, const int comp = 3);
	unsigned char classifyPixel(Color color);
	void colorPerPixel(std::string& fName, std::unordered_map<int, std::vector<unsigned char>>& corrMap);
	glm::vec3 nPlaneiPoint(float nearPlaneWd, float nearPlaneHt, float edgeLoc, float j, Camera cam1);
	std::unordered_map<int , std::unordered_map<int, int>> edges1;
	std::unordered_map<int , std::unordered_map<int, int>> edges2;
	


public:
	void setCameraParams(float fovy1, float aspect1, float nearPlane1, glm::vec3 eye1, glm::vec3 center1, glm::vec3 up1, 
						 float fovy2, float aspect2, float nearPlane2, glm::vec3 eye2, glm::vec3 center2, glm::vec3 up2);

	void reconstruct();
	glm::vec3 iPoint(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	std::unordered_map<int, int> detEdges(unsigned char* pxlRow, const unsigned int width);
	void createColorMap(std::vector<glm::vec3> colorSeq);
	bool loadImages(std::string filePath1, std::string filePath2);
	unsigned char** classifyPixels(Bitmap *bmp);
	void classifyPixels();
	int** correspondImages();
	float calcOpt(float **opt, int *corr, unsigned char* pxRow1, int j, unsigned char* pxRow2, int i);
	float score(unsigned char* pxRow1, unsigned char* pxRow2, const int j, const int i, const int comp = 3);
	int* corrRow(unsigned char* pxRow1, const int wd1, unsigned char *pxRow2, const int wd2);
	float evalCorresPondence();
	void correspondAllImages(std::string referenceImage);
	Reconstruct();
	~Reconstruct();
};

