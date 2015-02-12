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
#include "debruijn.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Debruijn::Debruijn()
{
}


Debruijn::~Debruijn()
{
}

static const GLfloat vertices[] = {
	//// position			//color			//texture coord
	//-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 0.0, 0.0, // red
	//0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0, 1.0, 0.0, // green
	//-0.5f, 0.5f, 0.0f, 0.0, 0.0, 1.0, 0.0, 1.0, // blue
	//0.5f, 0.5f, 0.0f, 1.0, 0.0, 0.0,  1.0, 1.0, // blue
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

	-1.0f, -1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
};

void Debruijn::preLoadBufferDataHook() {
	calcDebruijn(4, 4, _seq);
}


std::vector<glm::vec3> Debruijn::cmpVertices() {
	std::vector<glm::vec3> vertices;
	float iX = 0.0f;
	float iY = 0.0f;
	float iZ = 0.0f;
	float yOffset = 50.0f;
	float xOffset = 0.9f;
	glm::vec3 iPnt(iX - xOffset * (float)(_length/2.0), iY - yOffset /2.0, iZ);
	//vertices.push_back(iPnt); // starting vertice

	for (int i = 0; i < _length; ++i) {
		vertices.push_back(iPnt + glm::vec3( (float)i * xOffset, 0., 0.));
		vertices.push_back(iPnt + glm::vec3( (float)(i+1) * xOffset, 0., 0.));
		vertices.push_back(iPnt + glm::vec3( (float)i * xOffset, yOffset, 0.));

		vertices.push_back(iPnt + glm::vec3( (float)(i+1) * xOffset, 0., 0.));
		vertices.push_back(iPnt + glm::vec3( (float)(i+1) * xOffset, yOffset, 0.));
		vertices.push_back(iPnt + glm::vec3( (float)i * xOffset, yOffset, 0.));
	}
	vertSize = vertices.size();
	return vertices;
}

std::vector<glm::vec3> Debruijn::cmpColors() {
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> vVec = cmpVertices();

	unsigned char pr = 1;
	unsigned char pg = 1;
	unsigned char pb = 0;

	glm::vec3 color;
	color = glm::vec3((float)pr, (float)pg, (float)pb);
	_colorSeq.push_back(color);
	for (int j = 0; j < 6; ++j) {
		colors.push_back(color);
	}

	for (int i = 0; i < _length - 1; ++i) {
		unsigned char r, g, b;
		switch (_seq[i]) {
		case 'a': {
			r = 0;
			g = 0;
			b = 1;
			//color = glm::vec3(0.f, 0.f, 1.f);
			break;
		}
		case 'b': {
			r = 0;
			g = 1;
			b = 0;
			//color = glm::vec3(0., 0., 1.0);
			break;
		}
		case 'c': {
			r = 0;
			g = 1;
			b = 1;
			//color = glm::vec3(0., 1.0, 0.);
			break;
		}
		case 'd': {
			r = 1;
			g = 0;
			b = 0;
			//color = glm::vec3(0., 1.0, 0.);
			break;
		}
		case 'e': {
			r = 1;
			g = 0;
			b = 1;
			//color = glm::vec3(0., 1.0, 0.);
			break;
		}
		default:
			break;
		}

		std::cout << (int) pr << (int) pg << (int) pb << " ";

		pr = pr ^ r;
		pg = pg ^ g;
		pb = pb ^ b;
		color = glm::vec3((float)pr, (float)pg, (float)pb);
		_colorSeq.push_back(color);
		for (int j = 0; j < 6; ++j) {
			colors.push_back(color);
		}
	}
			//colors[2] = glm::vec3(0., 1.0, 0.);
			//colors[1] = glm::vec3(1.0, 0., 0.);
	return colors;
}

std::vector<unsigned int> Debruijn::indices() {
	std::vector<unsigned int> indices;
	for (int i = 0; i < _length; ++i) {
		indices.push_back(i);
		indices.push_back(i+2);
		indices.push_back(i+1);

		indices.push_back(i+1);
		indices.push_back(i+3);
		indices.push_back(i+1);
	}
	return indices;
}

void Debruijn::debruijn(unsigned int t,
	unsigned int p,
	const unsigned int k,
	const unsigned int n,
	unsigned int* a,
	boost::function<void(unsigned int*, unsigned int*)> callback)
{
	if (t > n) {
		// we want only necklaces, not pre-necklaces or Lyndon words
		if (n % p == 0) {
			callback(a + 1, a + p + 1);
		}
	}
	else {
		a[t] = a[t - p];

		debruijn(t + 1, p, k, n, a, callback);

		for (unsigned int j = a[t - p] + 1; j < k; ++j) {
			a[t] = j;
			debruijn(t + 1, t, k, n, a, callback);
		}
	}
}

void Debruijn::preRenderHook() {
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
	//glViewport(0, 0, 100, 100);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _tex2Ren);
	//GLuint texLoc = glGetUniformLocation(_sProgram, "tex");
	//glUniform1i(texLoc, 0);
}

void Debruijn::postRenderHook() {
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, 800, 600);
}

void Debruijn::postLoadBufferDataHook() {
	//glBindVertexArray(_vao);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _tex2Ren);
	//glGenFramebuffers(1, &frameBuf);
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuf);
	////glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _tex2Ren, 0);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex2Ren, 0);
	//GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };


	//// Create Renderbuffer Object to hold depth and stencil buffers
	//GLuint rboDepthStencil;
	//glGenRenderbuffers(1, &rboDepthStencil);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 100, 100);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

	//GLuint val = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
	//	throw std::runtime_error("Framebuffer not initialized properly.");
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindVertexArray(0);
}



struct seq_printer {
	const std::vector<char>& _alpha;
	std::vector<char>& _storage;

	seq_printer(const std::vector<char>& alpha, std::vector<char>& storage) : _alpha(alpha), _storage(storage) {}

	void operator() (unsigned int* a, unsigned int* a_end) const {
		for (unsigned int* i = a; i < a_end; ++i) {
			std::cout << _alpha[*i];
			const char c = _alpha[*i];
			_storage.push_back(c);
		}
	}
};

void Debruijn::calcDebruijn(unsigned int k, unsigned int n, std::vector<char>& seq) {

	const int N = n;
	std::vector<char> alpha;
	alpha.push_back('a');
	alpha.push_back('b');
	alpha.push_back('c');
	alpha.push_back('d');
	alpha.push_back('e');

	_k = alpha.size();
	_n = N;

	unsigned int *a = new unsigned int[N + 1];
	a[0] = 0;

	Debruijn::debruijn(1, 1, alpha.size(), N, a, seq_printer(alpha, seq));
	if (N > 1) {
		std::cout << alpha[0];
		seq.push_back(alpha[0]);
	}

	std::cout << std::endl;

	for (auto itr = seq.begin(); itr != seq.end(); ++itr) {
		std::cout << (char)*itr;
	}

	delete[] a;
}