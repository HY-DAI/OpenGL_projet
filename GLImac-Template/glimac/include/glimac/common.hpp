#pragma once

#include <GL/glew.h>
#include "glm.hpp"

namespace glimac {

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoords;
	glm::vec3 color;
	glm::vec3 normal;
};

}
