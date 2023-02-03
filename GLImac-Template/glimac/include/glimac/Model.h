#pragma once

#include"Mesh.h"
#include"Texture.h"
// #include"Shader.h"
#include"Material.h"
#include"Objloader.hpp"

#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>


namespace glimac {

class Model
{
private:
	Material * materials;
	std::vector<Mesh*> meshes;
	glm::vec3 position;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint texture1;
    GLint texture2;

	std::vector<Vertex> listOfVertex(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals) {
		std::vector<Vertex> shapeVertices;
		
		for (auto i = 0; i < vertices.size(); i ++) {
			Vertex crtVertex;
			
			crtVertex.position = vertices[i];
			crtVertex.texCoords = uvs[i];
			crtVertex.normal = normals[i];
			crtVertex.color = glm::vec3(1.f, 1.f, 1.f);
			
			shapeVertices.push_back(crtVertex);
		}
		return shapeVertices; 
	}

	std::vector<Vertex> sub(std::vector<Vertex> const &v, int m, int n) {
		std::vector<Vertex> sub;
		for (int i=m; i<n; i++)
			sub.push_back(v[i]);
		return sub;
	}

public:
    Program m_Program;

    Model(const char* objFile, FilePath VShaderPath, FilePath FShaderPath):
        m_Program(loadProgram(VShaderPath,FShaderPath)) {

        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        texture1 = glGetUniformLocation(m_Program.getGLId(), "texture1");
        texture2 = glGetUniformLocation(m_Program.getGLId(), "texture2");
    
		
		// Read our .obj file
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals; // Won't be used at the moment.
		std::map<int, std::string> materialIndices;
		bool res = loadOBJ(objFile, positions, uvs, normals, materialIndices);
		const std::vector<Vertex> vertices = listOfVertex(positions,uvs,normals);  
		GLuint nvertices = vertices.size();

        std::vector<int> material_positions;
        for(auto const& imap: materialIndices)
            material_positions.push_back(imap.first);

		std::vector<Vertex> subvector;
		// sous ensemble de vertices associé au mesh
        for (int i=0; i<material_positions.size()-1; i++) {    
			subvector =  sub(vertices, material_positions[i],material_positions[i+1]);
			// subvector = {material_positions.begin() + material_positions[i], material_positions.begin() +material_positions[i+1]-1 };
			// rentrer les meshs de l'objet
			this->meshes.push_back(new Mesh(subvector.data(), subvector.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
				glm::vec3(0.f),
				glm::vec3(0.f),
				glm::vec3(1.f)));
        }
		
		subvector =  sub(vertices, material_positions[material_positions.size()-1],nvertices);
		this->meshes.push_back(new Mesh(subvector.data(), subvector.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)));

	}


	~Model()
	{
		for (auto*& i : this->meshes)
			delete i;
	}

	//Functions
	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}

	
	void setModel(const glm::mat4 ModelMatrix)
	{
		for (auto& i : this->meshes)
			i->updateModel(ModelMatrix);
	}

	Program getProgram() {
		return this->m_Program;
	}

    void updateUniforms(
		int id,
        Program program,
		GLuint diffuseTex,
		GLuint specularTex
    ) {   
		this->materials[id].updateUniforms(program, diffuseTex, specularTex);
	}

	void updateUniforms(glm::mat4 MatView)
	{      
		// Initialize SDL and open a window
		GLuint largeur = 800;
		GLuint hauteur = 600;

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)largeur/(GLfloat)hauteur, 0.1f, 400.f); 
        glm::mat4 MVMatrix = MatView*this->meshes[0]->getModelMatrix();
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        
        glUniformMatrix4fv(uMVMatrix,1,GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix,1,GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(uMVPMatrix,1,GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
	}

	void render(glm::mat4 MatView, std::vector<Texture> textures) {
		GLuint texSize = textures.size();
        for (int i=0; i<meshes.size()-1; i++) {  
			textures[i%texSize].bind(0);
			meshes[i]->render(MatView, this->m_Program);
			textures[i%texSize].unbind();
		}
		textures[texSize-1].bind(0);
		meshes[meshes.size()-1]->render(MatView, this->m_Program);
		textures[texSize-1].unbind();
	}
};

}