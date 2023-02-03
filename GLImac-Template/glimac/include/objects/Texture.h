#pragma once

#include<iostream>
#include<string>

#include<GL/glew.h>


namespace glimac {

class Texture
{
private:
	GLuint id; // texture position
	GLuint *textures;
	unsigned int type;

public:

	Texture(const char* filePath, GLuint *textures, int id, GLenum type)
	{
        this->id = id;
        this->textures = textures;
        this->type = type;
        loadAndBindTextures(filePath, textures, id, type);
	}


	~Texture()
	{
		glDeleteTextures(1, &this->id);
	}

	inline GLuint getID() const { return this->id; }

	void bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(this->type, this->id);
	}

	void unbind()
	{
		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}

    bool loadAndBindTextures(std::string img_src, GLuint *textures, int position, GLenum type)
    {
        std::unique_ptr<Image> img_ptr = loadImage(img_src);
        if (!img_ptr)
            std::cout << "imgTriforce null " << std::endl;

        // binder la texture sur la cible GL_TEXTURE_2D
        glBindTexture(type, textures[position]); // type = GL_TEXTURE_2D
        // envoyer img au GPU pour stockée dans la texture object
        glTexImage2D(type, 0, GL_RGBA,
                    img_ptr->getWidth(), img_ptr->getHeight(),
                    0, GL_RGBA, GL_FLOAT,
                    (const void *)img_ptr->getPixels());
        // spécifier les filtres à appliquer
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // débinder la texture
        glBindTexture(type, 0);

        return true;
    }



};

}