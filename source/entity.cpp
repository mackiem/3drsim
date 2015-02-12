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
#include "entity.h"
#include <glm/gtc/type_ptr.hpp>
#include "utils3dr.h"
#include "tdogl\Bitmap.h"
#include "OBJLoader.h"

// Cube vertices
static GLfloat cubeVertices[] = {
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

static GLfloat floorVertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

static GLfloat imgFrame[] = {
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

void Entity::compileShaders() {
	
	//_sProgram = glCreateProgram();

	//glBindFragDataLocation(_sProgram, 0, "finalColor");

	//glLinkProgram(_sProgram);
	//glUseProgram(_sProgram);
}

void Entity::loadBufferData(GLuint shaderProg) {

	preLoadBufferDataHook();
	compileShaders();

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(3, _buffers);
	
	_vertices = cmpVertices();

	glBindBuffer(GL_ARRAY_BUFFER, _buffers[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices[0]) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

	GLuint pos = glGetAttribLocation(shaderProg, (const GLchar*) "vert");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	_colors = cmpColors();

	//std::cout << _vertices.size() << std::endl;
	//for (int i = 0; i < _colors.size(); i++)
	//{
	//	std::cout << _colors[i].r << ", " << _colors[i].g << ", " << _colors[i].b << "  " 
	//		<< _vertices[i].x << ", " << _vertices[i].y << ", " << _vertices[i].z << std::endl;
	//}

	glBindBuffer(GL_ARRAY_BUFFER, _buffers[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_colors[0]) * _colors.size(), &_colors[0], GL_STATIC_DRAW);
	
	GLuint color = glGetAttribLocation(shaderProg, (const GLchar*) "vertColor");
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	postLoadBufferDataHook();
}

void Entity::render( glm::mat4 view, glm::mat4 proj) {
	preRenderHook();
	GLuint modLoc = glGetUniformLocation(_sProgram, (const GLchar*) "model");
	glUniformMatrix4fv(modLoc, 1, GL_FALSE, glm::value_ptr(_model));

	
	GLuint viewLoc = glGetUniformLocation(_sProgram, (const GLchar*) "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLuint projLoc = glGetUniformLocation(_sProgram, (const GLchar*) "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	
	//glDrawArrays(GL_TRIANGLES, 0, (_vertices.size()/3));
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	postRenderHook();
}


void Cube::loadCube(GLuint sceneShaderProg, GLuint& robotVAO, glm::vec3 color) {
	glUseProgram(sceneShaderProg);
	glGenVertexArrays(1, &robotVAO);
	glBindVertexArray(robotVAO);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STREAM_DRAW);

	GLuint vertLoc = Utils3DR::attLoc(sceneShaderProg, "vert");
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);

	
	std::vector<glm::vec3> colors;
	for (int i = 0; i < 36; ++i) {
		colors.push_back(color);
	}

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), &colors[0], GL_STATIC_DRAW);

	GLuint vertColorLoc = Utils3DR::attLoc(sceneShaderProg, "vertColor");
	glEnableVertexAttribArray(vertColorLoc);
	glVertexAttribPointer(vertColorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//GLuint vertTexLoc = attLoc(sceneShaderProg, "vertTexCoord");
	//glEnableVertexAttribArray(vertTexLoc);
	//glVertexAttribPointer(vertTexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));

	glBindVertexArray(0);
	glUseProgram(0);
}


void Robot::load(GLuint sceneShaderProg) {
	loadCube(sceneShaderProg, vao, glm::vec3(153.f/255.f, 153.f/255.f, 0.f));
}

void Robot::render(GLuint shaderProg, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture) {
	glm::mat4 model;
	model = glm::translate(model, projPos);
	model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.5f));
	//model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	Utils3DR::renderObject(shaderProg, vao, model, projPos, cameraMatrix, 36, texture);
}

void Camera::load(GLuint sceneShaderProg) {
	loadCube(sceneShaderProg, vao, glm::vec3(90.f/255.f));
}

void Camera::render(GLuint shaderProg, glm::vec3 camPos, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture) {
	glm::mat4 model;
	model = glm::translate(model, camPos);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.2f));
	//model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	Utils3DR::renderObject(shaderProg, vao, model, projPos, cameraMatrix, 36, texture);
}

void Floor::load(GLuint sceneShaderProg) {
	glUseProgram(sceneShaderProg);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STREAM_DRAW);

	GLuint vertLoc = Utils3DR::attLoc(sceneShaderProg, "vert");
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);

	GLuint vertColorLoc = Utils3DR::attLoc(sceneShaderProg, "vertColor");
	glEnableVertexAttribArray(vertColorLoc);
	glVertexAttribPointer(vertColorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
	
	GLuint vertTexLoc = Utils3DR::attLoc(sceneShaderProg, "vertTexCoord");
	glEnableVertexAttribArray(vertTexLoc);
	glVertexAttribPointer(vertTexLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));

	//tdogl::Bitmap bitmap = tdogl::Bitmap::bitmapFromFile("resources/Free-Aluminum-foil-texture_01.jpg");
	tdogl::Bitmap bitmap = tdogl::Bitmap::bitmapFromFile("resources/1_floor.jpg");
	bitmap.flipVertically();
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width(), bitmap.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixelBuffer());

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void ImgFrame::load(GLuint sceneShaderProg, int width, int height) {
	this->width = width;
	this->height = height;

	glUseProgram(sceneShaderProg);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(imgFrame), imgFrame, GL_STREAM_DRAW);

	GLuint vertLoc = Utils3DR::attLoc(sceneShaderProg, "vert");
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);

	GLuint vertColorLoc = Utils3DR::attLoc(sceneShaderProg, "vertColor");
	glEnableVertexAttribArray(vertColorLoc);
	glVertexAttribPointer(vertColorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
	
	GLuint vertTexLoc = Utils3DR::attLoc(sceneShaderProg, "vertTexCoord");
	glEnableVertexAttribArray(vertTexLoc);
	glVertexAttribPointer(vertTexLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));

	//tdogl::Bitmap bitmap = tdogl::Bitmap::bitmapFromFile("resources/Free-Aluminum-foil-texture_01.jpg");
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width(), bitmap.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixelBuffer());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}


void ImgFrame::loadImage(std::string bmpFile) {
	tdogl::Bitmap bitmap = tdogl::Bitmap::bitmapFromFile(bmpFile);
	bitmap.flipVertically();
	glBindTexture(GL_TEXTURE_2D, tex);	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bitmap.width(), bitmap.height(), GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixelBuffer());
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void ImgFrame::render(GLuint shaderProg) {
		glm::mat4 model, view, proj, projView, projProj, texMapping;
		const static int scaleFactor = 1.0f;
		glClearColor(0.f, 0.f, 0.f, 1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProg);
		glBindVertexArray(vao);

		//model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor) / 12.0f);
		//model = glm::rotate(model, 120.f, glm::vec3(0.f, 1.f, 0.f));
		model = glm::scale(model, glm::vec3(1.33f, 1.0f, 1.f));
		glUniformMatrix4fv(Utils3DR::uniform(shaderProg, "model"), 1, GL_FALSE, glm::value_ptr(model));

		view = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(Utils3DR::uniform(shaderProg, "view"), 1, GL_FALSE, glm::value_ptr(view));

		proj = glm::perspective(60.0f, 8.f / 6.f, 0.1f, 100.f);
		glUniformMatrix4fv(Utils3DR::uniform(shaderProg, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glViewport(0, 0, 800, 600);
		const int i = 2;
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(Utils3DR::uniform(shaderProg, "tex"), i);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
}


void Floor::render(GLuint shaderProg, glm::vec3 camPos, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture) {
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(50.f));
	//model = glm::translate(model, camPos);
	glm::vec3 initPos(-0.18f, 0.f, -9.87f);
	model = glm::translate(model, glm::vec3(-.6f, 0.f, -10.3f) - initPos);
	const int i = 1;
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(Utils3DR::uniform(shaderProg, "tex"), i);
	Utils3DR::renderObject(shaderProg, vao, model, projPos, cameraMatrix, 6, texture);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::render(GLuint shaderProg, float* quart, double scaleFac, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture) {
	float qMat[16];
	Utils3DR::convertQuaternionToMatrix(quart, qMat);
	glm::mat4 model = glm::make_mat4(qMat);
	model = glm::scale(model, glm::vec3(scaleFac));

	Utils3DR::renderObject(shaderProg, vao, model, projPos, cameraMatrix, verticeNum, texture);
}

void Model::load(GLuint sceneShaderProg, std::string fileName) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glUseProgram(sceneShaderProg);

	std::vector<Vertex> vertices;
	OBJLoader::load(fileName.c_str(), vertices);

	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoord;
	glm::vec3 color(0.f);
	for (auto& vtx : vertices) {
		pos.push_back(glm::vec3(vtx.position[0], vtx.position[1], vtx.position[2]));
		//colors.push_back(glm::vec3(vtx.color[0], vtx.color[1], vtx.color[2]));
		colors.push_back(color);
		normals.push_back(glm::vec3(vtx.normal[0], vtx.normal[1], vtx.normal[2]));
		texCoord.push_back(glm::vec2(vtx.texCoord[0], vtx.texCoord[1]));
	}
	verticeNum = vertices.size();

	GLuint buffers[4];
	glGenBuffers(4, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pos.size(), &pos[0], GL_STATIC_DRAW);

	GLuint vertLoc = Utils3DR::attLoc(sceneShaderProg, "vert");
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), &colors[0], GL_STATIC_DRAW);
	
	//GLuint vertColorLoc = Utils3DR::attLoc(sceneShaderProg, "vertColor");
	//glEnableVertexAttribArray(vertColorLoc);
	//glVertexAttribPointer(vertColorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoord.size(), &texCoord[0], GL_STATIC_DRAW);

	//GLuint texCoordLoc = Utils3DR::attLoc(sceneShaderProg, "vertTexCoord");
	//glEnableVertexAttribArray(texCoordLoc);
	//glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}
