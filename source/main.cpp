/*
 main

 Copyright 2012 Thomas Dalling - http://tomdalling.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#define GLFW_CDECL
#include <AntTweakBar.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <fstream>

// tdogl classes
#include "tdogl/Bitmap.h"

// file classes
#include <fstream>
#include <sstream>

// user classes
#include "debruijn.h"
#include "reconstruct.h"
#include "utils3dr.h"

// constants
const glm::vec2 SCREEN_SIZE(800, 600);

static double projX;
static double projY;
static double projZ;

static double camX;
static double camY;
static double camZ;
static double betCamX;

static double visCamX;
static double visCamY;
static double visCamZ;

static double scaleFac;

static float g_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static float o_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };


// globals
GLuint gVBO = 0;
GLuint gVAO = 0;

// Cube vertices
GLfloat cubeVertices[] = {
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


// returns the full path to the file `fileName` in the resources directory of the app bundle
static std::string ResourcePath(std::string fileName) {
    char executablePath[1024] = {'\0'};
    DWORD charsCopied = GetModuleFileName(NULL, executablePath, 1024);
    if(charsCopied > 0 && charsCopied < 1024)
        return std::string(executablePath) + "\\..\\" + fileName;
    else
        throw std::runtime_error("GetModuleFileName failed a bit");
}


static std::string fileContents(const std::string& filePath) {

    //open file
    std::ifstream f;
    f.open(filePath.c_str(), std::ios::in | std::ios::binary);
    if(!f.is_open()){
        throw std::runtime_error(std::string("Failed to open file: ") + filePath);
    }

    //read whole file into stringstream buffer
    std::stringstream buffer;
    buffer << f.rdbuf();
	return buffer.str();
}


static GLuint sceneShaderProg;
static GLuint texShaderProg;
static GLuint sceneCubeVao;
static GLuint texCubeVao;

static void compileShader(GLuint& vtxS, std::string fileName) {
	std::string fileStr = fileContents(ResourcePath(fileName));
	const char* file = fileStr.c_str();
	glShaderSource(vtxS, 1, (const GLchar**) &file, NULL);
	glCompileShader(vtxS);

	GLint isCompiled = 0;
	glGetShaderiv(vtxS, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(vtxS, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> buffer(maxLength);
		glGetShaderInfoLog(vtxS, maxLength, &maxLength, &buffer[0]);

		std::cout << "Shader info log: ";
		for (auto &c : buffer) {
			std::cout << c;
		}
		std::cout << std::endl;
		glDeleteShader(vtxS);
		throw std::runtime_error("Shaders " + fileName + " not compiled properly");
	}
}

static GLuint cowVAO;
static int cowVertices;

static void loadCow() {
}


static GLuint robotVAO;

//static void loadRobot() {
//	glUseProgram(sceneShaderProg);
//	glGenVertexArrays(1, &robotVAO);
//	glBindVertexArray(robotVAO);
//
//	GLuint vbo;
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STREAM_DRAW);
//
//	GLuint vertLoc = Utils3DR::attLoc(sceneShaderProg, "vert");
//	glEnableVertexAttribArray(vertLoc);
//	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
//
//	
//	std::vector<glm::vec3> colors;
//	for (int i = 0; i < 36; ++i) {
//		colors.push_back(glm::vec3(153.0f / 255.0f, 153.f / 255.f, 0.f));
//	}
//
//	GLuint buffer;
//	glGenBuffers(1, &buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), &colors[0], GL_STATIC_DRAW);
//
//	GLuint vertColorLoc = Utils3DR::attLoc(sceneShaderProg, "vertColor");
//	glEnableVertexAttribArray(vertColorLoc);
//	glVertexAttribPointer(vertColorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	//GLuint vertTexLoc = Utils3DR::attLoc(sceneShaderProg, "vertTexCoord");
//	//glEnableVertexAttribArray(vertTexLoc);
//	//glVertexAttribPointer(vertTexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));
//
//	glBindVertexArray(0);
//	glUseProgram(0);
//}

static GLuint cam1VAO;
static GLuint cam2VAO;

static void loadRobot() {
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
		colors.push_back(glm::vec3(153.0f / 255.0f, 153.f / 255.f, 0.f));
	}

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), &colors[0], GL_STATIC_DRAW);

	GLuint vertColorLoc = Utils3DR::attLoc(sceneShaderProg, "vertColor");
	glEnableVertexAttribArray(vertColorLoc);
	glVertexAttribPointer(vertColorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//GLuint vertTexLoc = Utils3DR::attLoc(sceneShaderProg, "vertTexCoord");
	//glEnableVertexAttribArray(vertTexLoc);
	//glVertexAttribPointer(vertTexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));

	glBindVertexArray(0);
	glUseProgram(0);
}

GLuint defTex;

static void loadDefaultTexture() {
	tdogl::Bitmap bitmap = tdogl::Bitmap::bitmapFromFile("Free-Aluminum-foil-texture_01.jpg");
	bitmap.flipVertically();
	glGenTextures(1, &defTex);
	glBindTexture(GL_TEXTURE_2D, defTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width(), bitmap.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixelBuffer());
	glBindTexture(GL_TEXTURE_2D, 0);
}

static void createShaderProgram(GLuint& prog, const std::string vtxShaderPath, const std::string fragShaderPath) {
	GLuint vtxS = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vtxS, vtxShaderPath);
	GLuint fragS = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragS, fragShaderPath);
	prog = glCreateProgram();
	glAttachShader(prog, vtxS);
	glAttachShader(prog, fragS);

	glLinkProgram(prog);
	GLint isLinked = 0;
	glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog;
		glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);

		std::cout << "Program info log: ";
		for (auto &c : infoLog) {
			std::cout << c;
		}
		std::cout << std::endl;
		glDeleteProgram(prog);
		glDeleteShader(vtxS);
		glDeleteShader(fragS);
		throw std::runtime_error("Program not linked properly.");
	}
	glDetachShader(prog, vtxS);
	glDetachShader(prog, fragS);
	glDeleteShader(vtxS);
	glDeleteShader(fragS);
}

static GLuint texture;
static GLuint frameBuff;
static const int width = 400;
static Debruijn db;

void openFile() {
}

void loadDB() {
	glUseProgram(texShaderProg);
	db.loadBufferData(texShaderProg);
	glUseProgram(0);
}

void loadTexCube() {
	createShaderProgram(texShaderProg, "tex-vertex-shader.glsl", "tex-fragment-shader.glsl");
	glUseProgram(texShaderProg);

	glGenVertexArrays(1, &texCubeVao);
	glBindVertexArray(texCubeVao);

	GLuint buf;
	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STREAM_DRAW);

	GLuint vertLoc = Utils3DR::attLoc(texShaderProg, "vert");
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)0);
	
	GLuint colorLoc = Utils3DR::attLoc(texShaderProg, "vertColor");
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 3));

	glBindVertexArray(0);

	const int height = width;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);


	glGenFramebuffers(1, &frameBuff);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);

	GLuint renderBuf;
	glGenRenderbuffers(1, &renderBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuf);
	glRenderbufferStorage(GL_RENDERBUFFER,  GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Framebuffer not ready");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

static GLuint screenCapFB;
static const int screenCapWidth = 2000;
static const int screenCapHeight = screenCapWidth;

static void setupCaptureFBO() {
	glGenFramebuffers(1, &screenCapFB);
	glBindFramebuffer(GL_FRAMEBUFFER, screenCapFB);

	GLuint textureBuf;
	glGenTextures(1, &textureBuf);
	glBindTexture(GL_TEXTURE_2D, textureBuf);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenCapWidth, screenCapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	GLuint renderBuf;
	glGenRenderbuffers(1, &renderBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screenCapWidth, screenCapHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuf);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureBuf, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Screen Capture Framebuffer not ready");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void loadCube() {
	createShaderProgram(sceneShaderProg, "vertex-shader.glsl", "fragment-shader.glsl");
	glUseProgram(sceneShaderProg);

	glGenVertexArrays(1, &sceneCubeVao);
	glBindVertexArray(sceneCubeVao);

	GLuint pos;
	glGenBuffers(1, &pos);
	glBindBuffer(GL_ARRAY_BUFFER, pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * 36, cubeVertices, GL_STREAM_DRAW);

	GLuint vertLoc = Utils3DR::attLoc(sceneShaderProg, "vert");
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *) 0);

	//GLuint colorLoc = Utils3DR::attLoc(sceneShaderProg, "vertColor");
	//glEnableVertexAttribArray(colorLoc);
	//glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 3));

	//GLuint texLoc = Utils3DR::attLoc(sceneShaderProg, "vertTexCoord");
	//glEnableVertexAttribArray(texLoc);
	//glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void *)(sizeof(GLfloat) * 6));

	glBindVertexArray(0);
}


static int angle = 0;
const float scaleFactor = 5.f;
glm::mat4 model, view, proj, projView, projProj, texMapping;


static GLuint pCloudVAO;
static int pCSize = 0;

void visPointCloud(std::string fileName) {
	std::ifstream file(fileName);
	if (!file.good()) {
		throw std::runtime_error("File :" + fileName +" does not exist.");
	}
	float x, y, z; char c1, c2;

	std::vector<glm::vec3> pnts;
	std::vector<glm::vec3> colors;
	while (file >> x >> c1 >> y >> c2 >> z) {
		//std::cout << x << y << z << std::endl;
		pnts.push_back(glm::vec3(x, y, z));
		colors.push_back(glm::vec3(1.0f));
	}

	pCSize = pnts.size();

	glGenVertexArrays(1, &pCloudVAO);
	glBindVertexArray(pCloudVAO);

	glUseProgram(texShaderProg);

	GLuint buf[2];
	glGenBuffers(2, buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pnts.size(), &pnts[0], GL_STATIC_DRAW);

	GLuint vertLoc = Utils3DR::attLoc(texShaderProg, "vert");
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pnts.size(), &colors[0], GL_STATIC_DRAW);

	GLuint vertColorLoc = Utils3DR::attLoc(texShaderProg, "vertColor");
	glEnableVertexAttribArray(vertColorLoc);
	glVertexAttribPointer(vertColorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}
	//glPixelStorei();
	//glReadPixels(0, 100, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	//for (int i = 0; i < width; ++i) {
	//	for (int j = 0; j < height; ++j) {
	//		for (int k = 0; k < components; ++k) {
	//			flipPixels[(i + j * width) * components + k] = pixels[(i + (height - 1 - j) * width)  * components + k];
	//		}
	//	}
	//}
	//stbi_write_bmp("cam2.bmp", width, height, components, flipPixels);

void GLFWCALL glfwKBHandle(int key, int action) {

}

glm::mat4 robotMatrix;
static glm::vec3 handleKeyBoard() {
	const static float vel = 1.8f;
	static glm::vec3 dir(1.0f, 0.f, 0.f);
	static glm::vec3 right(0.f, 0.f, 1.f);
	static glm::vec3 pos(0.0f);
	static glm::vec3 initPos(-0.18f, 0.f, -9.87f);
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = (float)(currentTime - lastTime);
	if ((glfwGetKey('w') == GLFW_PRESS) || (glfwGetKey('W') == GLFW_PRESS)) {
		pos += deltaTime * vel * dir;
	}
	if ((glfwGetKey('s') == GLFW_PRESS) || (glfwGetKey('S') == GLFW_PRESS)) {
		pos -= deltaTime * vel * dir;
	}
	if ((glfwGetKey('a') == GLFW_PRESS) || (glfwGetKey('A') == GLFW_PRESS)) {
		pos += deltaTime * vel * right;
	}
	if ((glfwGetKey('d') == GLFW_PRESS) || (glfwGetKey('D') == GLFW_PRESS)) {
		pos -= deltaTime * vel * right;
	}
	lastTime = currentTime;
	glm::vec3 translation = pos - initPos;
	projX = pos.x;
	return translation;
}

static int shudCapture = 0;
static int shudRecon = 0;

static void TW_CALL saveCamImages(void* clientData) {
	shudCapture = 1;
}

static void TW_CALL reconstruct(void* clientData) {
	shudRecon = 1;
}


std::string s = "models/cow.obj";
// ...
void TW_CALL SetMyStdStringCB(const void *value, void * /*clientData*/)
{
	// Set: copy the value of s from AntTweakBar
	const std::string *srcPtr = static_cast<const std::string *>(value);
	s = *srcPtr;
}
void TW_CALL GetMyStdStringCB(void *value, void * /*clientData*/)
{
	// Get: copy the value of s to AntTweakBar
	std::string *destPtr = static_cast<std::string *>(value);
	TwCopyStdStringToLibrary(*destPtr, s); // the use of TwCopyStdStringToLibrary is required here
}




static int overwriteImg = 1;
static int scene = 0;
static int corr = 0;
static TwBar* bar;
static Robot robot;
static Floor objFloor;
static Camera cam1;
static Camera cam2;
static ImgFrame imgFrame;
static Model modelObj;

static void TW_CALL loadModel(void* clientData) {
	modelObj.load(sceneShaderProg, s);
}

static void render() {
	if (scene == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);

		glClearColor(0.f, 0.f, 0.f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glBindVertexArray(texCubeVao);
		//glm::vec3 robotPos = handleKeyBoard();
		static glm::vec3 initPos(-0.18f, 0.f, -9.87f);
		glm::vec3 pos(projX, projY, projZ);
		glm::vec3 robotPos = pos - initPos;
		glUseProgram(texShaderProg);
		glBindVertexArray(db._vao);

		//model = glm::rotate(glm::mat4(1.0f), (float)angle++, glm::vec3(0.f, 1.0f, 0.f));
		model = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor) / 30.0f);
		glUniformMatrix4fv(Utils3DR::uniform(texShaderProg, "model"), 1, GL_FALSE, glm::value_ptr(model));

		view = glm::lookAt(glm::vec3(0.f, 0.f, -10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(Utils3DR::uniform(texShaderProg, "view"), 1, GL_FALSE, glm::value_ptr(view));

		proj = glm::perspective(60.0f, 2.f / 2.f, 0.1f, 100.f);
		glUniformMatrix4fv(Utils3DR::uniform(texShaderProg, "proj"), 1, GL_FALSE, glm::value_ptr(proj));


		glViewport(0, 0, width, width);
		glDrawArrays(GL_TRIANGLES, 0, db.vertSize);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glClearColor(1.f, 0.5f, 0.5f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glUseProgram(0);
		glViewport(0, 0, 800, 600);
		// load big cube
		model = glm::rotate(glm::mat4(1.0f), (float)(360), glm::vec3(0.f, 1.0f, 0.f));
		model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
		glm::mat4 cameraMatrix = glm::lookAt(glm::vec3(10.f, 10.f, 10.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		//Utils3DR::renderObject(sceneShaderProg, sceneCubeVao, model);

		//const float scaleFac = 0.5f;
		// load cow
		modelObj.render(sceneShaderProg, o_Rotation, scaleFac, robotPos, cameraMatrix, 36, texture);

		// load robot cube
		robot.render(sceneShaderProg, robotPos, cameraMatrix, 36, texture);

		// render cameras
		glm::vec3 userOffset(camX, camY, camZ);
		model = glm::mat4(1.0f);
		glm::vec3 eye1(glm::vec3(0.09f, 0.f, 6.33f) + userOffset);
		glm::vec3 center1(glm::vec3(0.f, 0.f, 0.f) + userOffset);
		glm::vec3 up1(glm::vec3(0.f, 1.f, 0.f));

		const glm::vec3 eyeOffset(betCamX, 0.f, 0.f);
		glm::vec3 eye2(eye1 + eyeOffset);
		glm::vec3 center2(center1 + eyeOffset);

		cam1.render(sceneShaderProg, eye1, robotPos, cameraMatrix, 36, texture);

		cam2.render(sceneShaderProg, eye2, robotPos, cameraMatrix, 36, texture);

		// render floor
		objFloor.render(sceneShaderProg, robotPos, robotPos, cameraMatrix, 6, texture);


		// render camera 1 view
		glEnable(GL_SCISSOR_TEST);
		glViewport(0, 110, 133.33, 100);
		glScissor(0, 110, 133.33, 100);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 synCamMatrix = glm::lookAt(eye1, center1, up1);

		// load cow
		modelObj.render(sceneShaderProg, o_Rotation, scaleFac, robotPos, synCamMatrix, 36, texture);

		// load robot cube
		//robot.render(sceneShaderProg, robotPos, synCamMatrix, 36, texture);
		objFloor.render(sceneShaderProg, robotPos, robotPos, synCamMatrix, 6, texture);

		// render camera 2 view
		glViewport(0, 0, 133.33, 100);
		glScissor(0, 0, 133.33, 100);
		//glClearColor(0.f, 0.f, 0.f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 synCamMatrix2 = glm::lookAt(eye2, center2, up1);

		// load cow
		modelObj.render(sceneShaderProg, o_Rotation, scaleFac, robotPos, synCamMatrix2, 36, texture);

		// load robot cube
		//robot.render(sceneShaderProg, robotPos, synCamMatrix2, 36, texture);
		objFloor.render(sceneShaderProg, robotPos, robotPos, synCamMatrix2, 6, texture);

		glDisable(GL_SCISSOR_TEST);

		if (shudCapture == 1) {
			glBindFramebuffer(GL_FRAMEBUFFER, screenCapFB);
			glClearColor(1.f, 0.5f, 0.5f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, screenCapWidth, screenCapHeight);
			glm::mat4 synCamMatrix = glm::lookAt(eye1, center1, up1);

			// load cow
			modelObj.render(sceneShaderProg, o_Rotation, scaleFac, robotPos, synCamMatrix, 36, texture);

			if (overwriteImg) {
				Utils3DR::saveCamImage(screenCapWidth, screenCapHeight, "cam1_0.bmp");
			} else {
				Utils3DR:: saveCamImage(1, screenCapWidth, screenCapHeight);
			}

			// render camera 2
			glClearColor(1.f, 0.5f, 0.5f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, screenCapWidth, screenCapHeight);
			glm::mat4 synCamMatrix2 = glm::lookAt(eye2, center2, up1);

			// load cow
			modelObj.render(sceneShaderProg, o_Rotation, scaleFac, robotPos, synCamMatrix2, 36, texture);

			// load robot cube
			if (overwriteImg) {
				Utils3DR::saveCamImage(screenCapWidth, screenCapHeight, "cam2_0.bmp");
			} else {
				Utils3DR:: saveCamImage(2, screenCapWidth, screenCapHeight);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			shudCapture = 0;
		}

		if (shudRecon == 1) {

			const float fovy = 45.0f;
			const float aspect = 800.f / 600.f;
			const float nearPlane = 0.1f;

			std::ofstream myfile;
			myfile.open("corr.txt");

			Reconstruct recon;
			recon.setCameraParams(fovy, aspect, nearPlane, eye1, center1, up1, fovy, aspect, nearPlane, eye2, center2, up1);
			recon.createColorMap(db._colorSeq);
			//recon.correspondAllImages("cam_reference.bmp");
			recon.loadImages("cam1_0.bmp", "cam2_0.bmp");
			recon.createColorMap(db._colorSeq);
			recon.classifyPixels();
			recon.reconstruct();
			//int** corrMat = recon.correspondImages();
			//for (int j = 0; j < 5; ++j) {
			//	for (int i = 0; i < 2000; ++i) {
			//		myfile << corrMat[j][i] << " ";
			//	}
			//	myfile << std::endl;
			//}
			//for (int j = 0; j < 5; ++j) {
			//	delete[] corrMat[j];
			//}
			//delete[] corrMat;

			myfile.close();
			imgFrame.loadImage("corr.bmp");
			visPointCloud("output.txt");
			scene = 1;
			shudRecon = 0;
			float tmp[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			std::copy(tmp, tmp + 4, g_Rotation);
		}
	}
	else {
		if (corr == 1) {
			imgFrame.render(sceneShaderProg);
		} else {
			glClearColor(0.f, 0.f, 0.f, 1);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(texShaderProg);
			glBindVertexArray(pCloudVAO);

			float qMat[16];
			Utils3DR::convertQuaternionToMatrix(g_Rotation, qMat);
			model = glm::make_mat4(qMat);
			model = glm::rotate(model, (float)(180.f), glm::vec3(0.f, 0.0f, 1.f));
			model = glm::scale(model, glm::vec3(scaleFactor, scaleFactor, scaleFactor) / 12.0f);
			//compensate for square image
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f/1.33f));
			glUniformMatrix4fv(Utils3DR::uniform(texShaderProg, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glm::vec3 visOffset(visCamX, visCamY, visCamZ);
			view = glm::lookAt(glm::vec3(0.f, 0.f, -7.f) + visOffset, glm::vec3(0.f, 0.f, 0.f) + visOffset, glm::vec3(0.f, 1.f, 0.f));
			glUniformMatrix4fv(Utils3DR::uniform(texShaderProg, "view"), 1, GL_FALSE, glm::value_ptr(view));

			proj = glm::perspective(60.0f, 8.f / 6.f, 0.1f, 100.f);
			glUniformMatrix4fv(Utils3DR::uniform(texShaderProg, "proj"), 1, GL_FALSE, glm::value_ptr(proj));


			glViewport(0, 0, 800, 600);
			glPointSize(2.0f);
			glDrawArrays(GL_POINTS, 0, pCSize);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	//if (glfwGetKey('1')) {
	//	scene = (scene == 0) ? 1 : 0;
	//}

	TwDraw();
	//handleCameraCapture();
	glfwSwapBuffers();
	//glfwPollEvents();
}







// the program starts here
void AppMain() {
    // initialise GLFW
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    // open a window with GLFW
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if(!glfwOpenWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

	glfwSetWindowTitle("3D Reconstruction Simulator");

	projX = projY = projZ = 0.f;
	camX = camY = camZ = 0.f;
	betCamX = 0.2f;
	scaleFac = 0.5f;
	glfwEnable(GLFW_MOUSE_CURSOR);
	glfwEnable(GLFW_KEY_REPEAT);


	TwWindowSize(800, 600);
	// Ant Tweak Bar
	TwInit(TW_OPENGL_CORE, NULL);
	bar = TwNewBar("3DRSim");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.

	TwAddButton(bar, "Capture Images", saveCamImages, NULL, " label='Capture Images' key=p group='Workflow'");
	//TwAddButton(bar, "Capture Images", saveCamImages, NULL, " label='Capture Images' key=p group='Workflow'");
	TwAddButton(bar, "Reconstruct", reconstruct, NULL, " label='Reconstruct' key=c group='Workflow'");
	TwAddVarCB(bar, "s", TW_TYPE_STDSTRING, SetMyStdStringCB, GetMyStdStringCB, NULL, " label='Path' group='Workflow'");
	TwAddButton(bar, "Load Model", loadModel, NULL, " label='Load Model' group='Workflow'");

	TwAddVarRW(bar, "Obj. Rotation", TW_TYPE_QUAT4F, &o_Rotation,
		" label='Rotation' opened=true help='Change the object orientation.' group='Object'");
	TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &scaleFac,
		" label='Scale' min=0.0 max=5.0 step=0.01 keyIncr=d keyDecr=D help='Z Pos' group='Object'");
	TwAddVarRW(bar, "Proj. X", TW_TYPE_DOUBLE, &projX,
		" label='X Pos' min=-10 max=10 step=0.01 keyIncr=a keyDecr=A help='X Pos' group='Projector'");
	TwAddVarRW(bar, "Proj. Y", TW_TYPE_DOUBLE, &projY,
		" label='Y Pos' min=-10 max=10 step=0.01 keyIncr=s keyDecr=S help='Y Pos' group='Projector'");
	TwAddVarRW(bar, "Proj. Z", TW_TYPE_DOUBLE, &projZ,
		" label='Z Pos' min=-10 max=10 step=0.01 keyIncr=d keyDecr=D help='Z Pos' group='Projector'");

	TwAddVarRW(bar, "Cam. X", TW_TYPE_DOUBLE, &camX,
		" label='X Pos' min=-10 max=10 step=0.01 keyIncr=a keyDecr=A help='X Pos' group='Cameras'");
	TwAddVarRW(bar, "Cam. Y", TW_TYPE_DOUBLE, &camY,
		" label='Y Pos' min=-10 max=10 step=0.01 keyIncr=s keyDecr=S help='Y Pos' group='Cameras'");
	TwAddVarRW(bar, "Cam. Z", TW_TYPE_DOUBLE, &camZ,
		" label='Z Pos' min=-10 max=10 step=0.01 keyIncr=d keyDecr=D help='Z Pos' group='Cameras'");
	TwAddVarRW(bar, "Distance Cam", TW_TYPE_DOUBLE, &betCamX,
		" label='Distance' min=-10 max=10 step=0.01 keyIncr=d keyDecr=D help='Z Pos' group='Cameras'");
	TwAddVarRW(bar, "Overwrite Img.", TW_TYPE_BOOL32, &overwriteImg,
		" label='Overwrite Image' help='Toggle reconstruction display mode.' group='Reconstruction'");




	TwAddVarRW(bar, "VisCam. X", TW_TYPE_DOUBLE, &visCamX,
		" label='X Pos' min=-10 max=10 step=0.01 keyIncr=a keyDecr=A help='X Pos' group='Reconstruction'");
	TwAddVarRW(bar, "VisCam. Y", TW_TYPE_DOUBLE, &visCamY,
		" label='Y Pos' min=-10 max=10 step=0.01 keyIncr=s keyDecr=S help='Y Pos' group='Reconstruction'");
	TwAddVarRW(bar, "VisCam. Z", TW_TYPE_DOUBLE, &visCamZ,
		" label='Z Pos' min=-10 max=10 step=0.01 keyIncr=d keyDecr=D help='Z Pos' group='Reconstruction'");
	TwAddVarRW(bar, "Vis. Rotation", TW_TYPE_QUAT4F, &g_Rotation,
		" label='Rotation' opened=true help='Change the object orientation.' group='Reconstruction'");
	TwAddVarRW(bar, "vis", TW_TYPE_BOOL32, &scene,
		" label='Reconstruction' key=t help='Toggle reconstruction display mode.' group='Reconstruction'");
	TwAddVarRW(bar, "corr", TW_TYPE_BOOL32, &corr,
		" label='Correspondence' help='Toggle reconstruction display mode.' group='Reconstruction'");


	// Set GLFW event callbacks
	// - Redirect window size changes to the callback function WindowSizeCB
	//glfwSetWindowSizeCallback(WindowSizeCB);
	 //- Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
	// - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
	// - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
	// - Directly redirect GLFW key events to AntTweakBar
	glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
	//// - Directly redirect GLFW char events to AntTweakBar
	glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);


    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");

	while (glGetError() != GL_NO_ERROR) {}

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	loadCube();
	loadTexCube();

	robot.load(sceneShaderProg);
	cam1.load(sceneShaderProg);
	cam2.load(sceneShaderProg);
	objFloor.load(sceneShaderProg);
	imgFrame.load(sceneShaderProg, 100, 100);
	//modelObj.load(sceneShaderProg, "models/cow.obj");
	modelObj.load(sceneShaderProg, s);


	// load DB after tex cube or no shaders!
	loadDB();

	setupCaptureFBO();

	imgFrame.loadImage("corr.bmp");
	visPointCloud("output.txt");
    //// load vertex and fragment shaders into opengl
    //LoadShaders();

    //// load the texture
    //LoadTexture();

    //// create buffer and fill it with the points of the triangle
    //LoadTriangle();

    // run while the window is open
    while(glfwGetWindowParam(GLFW_OPENED)){
        // draw one frame
		render();
		// check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			std::cerr << "OpenGL Error " << error << ": " << (const char*)gluErrorString(error) << std::endl;
        //Render();
    }

    // clean up and exit
	TwTerminate();
    glfwTerminate();
}

int main(int argc, char *argv[]) {
    try {
        AppMain();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}