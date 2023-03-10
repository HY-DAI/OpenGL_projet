
#include <map>

#ifndef OBJLOADER_H
#define OBJLOADER_H

bool loadOBJMTL(
	const char * pathOBJ, 
	const char * pathMTL, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs, 
	std::vector<glm::vec3> & out_normals
);

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals, 
	std::map<int, std::string> &out_materialIndices
);

bool loadMTL(
	const char * pathMTL, 
	std::map<int, std::string> &out_materialIndices
);

bool loadVertices(
	const char * path, 
	std::vector<glm::vec3> & out_vertices
);

bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

#endif