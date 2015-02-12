/* **************************************
Copyright (c) 2015 C. D. Tharindu Mathew
http://mackiemathew.wordpress.com

This project is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program. If not, see <http://www.gnu.org/licenses/agpl-3.0.html>.
************************************** */
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

