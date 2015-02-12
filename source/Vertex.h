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

#include <glm/glm.hpp>

/**
 * This structure defines a vertex data.
 */
struct Vertex {
	float position[3];
	float color[3];
	float normal[3];
	float texCoord[3];

	Vertex() {}

	Vertex(glm::vec3 pos, glm::vec3 c, glm::vec3 n, glm::vec3 tex) {
		position[0]=pos.x;
		position[1]=pos.y;
		position[2]=pos.z;

		color[0]=c.x;
		color[1]=c.y;
		color[2]=c.z;

		normal[0]=n.x;
		normal[1]=n.y;
		normal[2]=n.z;

		texCoord[0]=tex.x;
		texCoord[1]=tex.y;
		texCoord[2]=tex.z;
	}
};
