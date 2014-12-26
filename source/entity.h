#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#define BUFF_NUM 4
#define POSITION 0
#define COLOR 1
#define NORMAL 2

class Entity {
protected:
	void compileShaders();
	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec3> _colors;
	std::vector<glm::vec3> _normals;
	virtual void preLoadBufferDataHook() {};
	virtual void postLoadBufferDataHook() {};
	virtual void preRenderHook() {};
	virtual void postRenderHook() {};
	glm::mat4 _model;
public:
	int vertSize = 0;
	const std::vector<glm::vec3>& vertices() { return _vertices; };
	const std::vector<glm::vec3>& normals() { return _normals; };
	virtual void loadBufferData(GLuint shaderProg);
	GLuint _vao;
	GLuint _buffers[BUFF_NUM];
	virtual std::vector<glm::vec3> cmpVertices() = 0;
	virtual std::vector<glm::vec3> cmpColors() = 0;
	GLuint _sProgram;
	void render( glm::mat4 view, glm::mat4 proj);
	void foo();
};

class ImgFrame {
	GLuint vao;
	GLuint tex;
	int width, height;
public:
	void load(GLuint sceneShaderProg, int width, int height);
	void loadImage(std::string bmpFile);
	//void render(GLuint shaderProg, glm::vec3 camPos, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture);
	void render(GLuint shaderProg);

};

class Floor {
	GLuint vao;
	GLuint tex;
public:
	void load(GLuint sceneShaderProg);
	void render(GLuint shaderProg, glm::vec3 camPos, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture);
};

class Cube {
protected:
	GLuint vao;
	void loadCube(GLuint sceneShaderProg, GLuint& vao, glm::vec3 color);
};

class Robot : public Cube {
	public:
		void load(GLuint sceneShaderProg);
		void render(GLuint shaderProg, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture);
};


class Camera : public Cube {
	public:
		void load(GLuint sceneShaderProg);
		void render(GLuint shaderProg, glm::vec3 camPos, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture);
};

class Model {
	GLuint vao;
	GLuint tex;
	int verticeNum;
public:
	void load(GLuint sceneShaderProg, std::string fileName);
	void render(GLuint shaderProg, float* quart, double scaleFac, glm::vec3 projPos, glm::mat4 cameraMatrix, const int noOfVertices, GLuint texture);
};