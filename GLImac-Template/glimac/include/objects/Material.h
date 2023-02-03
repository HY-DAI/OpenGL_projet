#pragma once

#include <GL/glew.h>
#include<glm.hpp>

#include <glimac/Program.hpp>

namespace glimac {

class Material
{
private:
	glm::vec3 uKd; // coefficient de reflection diffuse de l'objet
	glm::vec3 uKs; // coefficient de reflection glossy de l'objet
	float uShininess; // exposant de brillance permettant de controller la taille de la "tache" de brillance glossy

	GLint diffuseTex;
	GLint specularTex;

public:
    Material() {
        this->uKd = glm::vec3(1, 1, 1);             // coefficient de reflection diffuse de l'objet
        this->uKs = glm::vec3(1, 1, 1);             // coefficient de reflection glossy de l'objet
        this->uShininess = 1.f;                         // exposant de brillance (taille de brillance glossy)
        this->diffuseTex = 0;    
        this->specularTex = 1; 
    }

	Material(	
    glm::vec3 uKd,
	glm::vec3 uKs, 
	float uShininess,    
	GLint diffuseTex,
	GLint specularTex
	)
	{
		this->uKd = uKd;
		this->uKs = uKs;
		this->uShininess = uShininess;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	~Material(){}

	//Function
    void setUniforms(
		glm::vec3 uKd,   // coefficient de reflection diffuse de l'objet
		glm::vec3 uKs,   // coefficient de reflection glossy de l'objet
		float uShininess,   // exposant de brillance (taille de brillance glossy)
		GLuint diffuseTex,
		GLuint specularTex
    ) {
        this->uShininess = uShininess;
        this->uKd = uKd;
        this->uKs = uKs;
        this->diffuseTex =  diffuseTex;
        this->specularTex = specularTex;
    } 
    
    void updateUniforms(
        Program program,
		GLuint diffuseTex,
		GLuint specularTex
    ) {
        GLuint locationKd = glGetUniformLocation(program.getGLId(), "uKd");
        GLuint locationKs = glGetUniformLocation(program.getGLId(), "uKs");
        GLuint locationShininess = glGetUniformLocation(program.getGLId(), "uShininess");
        GLuint locationTex1 = glGetUniformLocation(program.getGLId(), "texture1");
        GLuint locationTex2 = glGetUniformLocation(program.getGLId(), "texture2");
    
        glUniform1f(locationShininess, this->uShininess);
        glUniform3fv(locationKd, 1, glm::value_ptr(this->uKd));
        glUniform3fv(locationKs, 1, glm::value_ptr(this->uKs));
        glUniform1i(locationTex1,this->diffuseTex);
        glUniform1i(locationTex2,this->specularTex);
    } 
};

}