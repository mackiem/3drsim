#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Utils3DR
{
public:
	static void saveCamImage(const int screenCapWidth, const int screenCapHeight, std::string fileName, bool overwrite = true);
	static void saveCamImage(const int camera, const int screenCapWidth, const int screenCapHeight);
	static void saveImage(const int width, const int height, unsigned char* buffer, std::string fileName, const unsigned int components = 3);

	static GLint attLoc(GLuint prog, const std::string& attName);
	static GLuint uniform(GLuint shaderProg, std::string uniName);
	static void renderObject(GLuint shaderProg, GLuint objVAO, glm::mat4 mod, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture);
	static void convertQuaternionToMatrix(const float *quat, float *mat);
};

