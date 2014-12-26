#include "utils3dr.h"
#include <GL/glew.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <fstream>

void Utils3DR::saveCamImage(const int screenCapWidth, const int screenCapHeight, std::string imgFile, bool overwrite) {
	const int width = screenCapWidth;
	const int height = screenCapHeight;
	const int components = 3;
	unsigned char* pixels = new unsigned char[width * height * components];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	unsigned char* flipPixels = new unsigned char[width * height * components];
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			for (int k = 0; k < components; ++k) {
				flipPixels[(i + j * width) * components + k] = pixels[(i + (height - 1 - j) * width)  * components + k];
			}
		}
	}
	stbi_write_bmp(imgFile.c_str(), width, height, components, flipPixels);
	delete[] pixels;
	delete[] flipPixels;
}

void Utils3DR::convertQuaternionToMatrix(const float *quat, float *mat)
{
	float yy2 = 2.0f * quat[1] * quat[1];
	float xy2 = 2.0f * quat[0] * quat[1];
	float xz2 = 2.0f * quat[0] * quat[2];
	float yz2 = 2.0f * quat[1] * quat[2];
	float zz2 = 2.0f * quat[2] * quat[2];
	float wz2 = 2.0f * quat[3] * quat[2];
	float wy2 = 2.0f * quat[3] * quat[1];
	float wx2 = 2.0f * quat[3] * quat[0];
	float xx2 = 2.0f * quat[0] * quat[0];
	mat[0 * 4 + 0] = -yy2 - zz2 + 1.0f;
	mat[0 * 4 + 1] = xy2 + wz2;
	mat[0 * 4 + 2] = xz2 - wy2;
	mat[0 * 4 + 3] = 0;
	mat[1 * 4 + 0] = xy2 - wz2;
	mat[1 * 4 + 1] = -xx2 - zz2 + 1.0f;
	mat[1 * 4 + 2] = yz2 + wx2;
	mat[1 * 4 + 3] = 0;
	mat[2 * 4 + 0] = xz2 + wy2;
	mat[2 * 4 + 1] = yz2 - wx2;
	mat[2 * 4 + 2] = -xx2 - yy2 + 1.0f;
	mat[2 * 4 + 3] = 0;
	mat[3 * 4 + 0] = mat[3 * 4 + 1] = mat[3 * 4 + 2] = 0;
	mat[3 * 4 + 3] = 1;
}

void Utils3DR::saveImage(const int width, const int height, unsigned char* buffer, std::string fileName, const unsigned int components) {
	stbi_write_bmp(fileName.c_str(), width, height, components, buffer);
}

void Utils3DR::saveCamImage(const int camera, const int screenCapWidth, const int screenCapHeight) {
	static int suffix = 0;
	std::string fPre("cam");
	std::ifstream fs;
	std::string fName;
	do {
		fs.close();
		fs.clear();
		fName = fPre;
		fName.append(std::to_string(camera)).append("_").append(std::to_string(suffix)).append(".bmp");
		fs.open(fName);
	} while (fs.good() && ++suffix);

	saveCamImage(screenCapWidth, screenCapHeight, fName);
}

GLint Utils3DR::attLoc(GLuint prog, const std::string& attName) {
	std::string att = attName;
	GLint loc = glGetAttribLocation(prog, att.c_str());
	if (loc == -1) {
		throw std::runtime_error("Attribute " + attName + " does not exist.");
	}
	return loc;
}

GLuint Utils3DR::uniform(GLuint shaderProg, std::string uniName) {
	GLint loc = glGetUniformLocation(shaderProg, uniName.c_str());
	if (loc == -1) {
		throw std::runtime_error("Uniform location not found for " + uniName);
	}
	return loc;
}

void Utils3DR::renderObject(GLuint shaderProg, GLuint objVAO, glm::mat4 mod, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture) {
	glm::mat4 model, view, proj, projView, projProj, texMapping;
	glBindVertexArray(objVAO);
	glUseProgram(shaderProg);

	glUniformMatrix4fv(uniform(shaderProg, "model"), 1, GL_FALSE, glm::value_ptr(mod));

	//view = glm::lookAt(glm::vec3(10.f, 10.f, 10.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(uniform(shaderProg, "view"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));

	proj = glm::perspective(45.0f, 800.f / 600.f, 0.1f, 100.f);
	glUniformMatrix4fv(uniform(shaderProg, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	// projective texture mapping section
	projView = glm::lookAt(projPos, projPos + glm::vec3(0.f, 0.f, -10.f), glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(uniform(shaderProg, "projView"), 1, GL_FALSE, glm::value_ptr(projView));
	projProj = glm::perspective(45.0f, 600.f / 600.f, 0.1f, 100.f);
	glUniformMatrix4fv(uniform(shaderProg, "projProj"), 1, GL_FALSE, glm::value_ptr(projProj));
	texMapping = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)), glm::vec3(0.5f, 0.5f, 0.5f));
	texMapping = glm::mat4(0.5f, 0.f, 0.f, 0.f,
		0.f, 0.5f, 0.f, 0.f,
		0.f, 0.f, 0.5f, 0.f,
		0.5f, 0.5f, 0.5f, 1.0f);
	//texMapping = glm::mat4(0.5f, 0.f, 0.f, 0.5f,
	//	0.f, 0.5f, 0.f, 0.5f,
	//	0.f, 0.f, 0.5f, 0.5f,
	//	0.f, 0.f, 0.f, 1.0f);
	glUniformMatrix4fv(uniform(shaderProg, "texMapping"), 1, GL_FALSE, glm::value_ptr(texMapping));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(uniform(shaderProg, "projTex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, noOfVertices);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}