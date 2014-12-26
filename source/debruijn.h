#pragma once
#include <GL/glew.h>
#include <GL/glfw.h>
#include <boost/function.hpp>
#include <vector>
#include "entity.h"

#define POSITION 0
#define COLOR 1
#define NORMAL 2

class Debruijn : public Entity
{
private:
	void debruijn(unsigned int t,
		unsigned int p,
		const unsigned int k,
		const unsigned int n,
		unsigned int* a,
		boost::function<void(unsigned int*, unsigned int*)> callback);
	const int _length = 125;
		int val = 0;
	//void compileShaders();
	std::vector<glm::vec3> cmpVertices();
	std::vector<glm::vec3> cmpColors();
	std::vector<unsigned int> indices();
	std::vector<char> _seq;
	void preLoadBufferDataHook();
	//std::vector<glm::vec3> _vertices;
	//std::vector<glm::vec3> _colors;
	GLuint _tex2Ren;
	GLuint frameBuf;

protected:
	virtual void postLoadBufferDataHook();


public:
	unsigned int _k, _n;
	std::vector<glm::vec3> _colorSeq;
	void setTexture(GLuint texToRender) { _tex2Ren = texToRender; };
	//GLuint _vao;
	//GLuint _buffers[2];
	//GLuint _sProgram;
	//void render( glm::mat4 view, glm::mat4 proj);
	GLuint _ibuf;

	virtual void preRenderHook();
	virtual void postRenderHook();

	void calcDebruijn(unsigned int k, unsigned int n, std::vector<char>& seq);
	void createSeq();
	Debruijn();
	~Debruijn();
};

