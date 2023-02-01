#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <cstddef>
#include <glm/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/objloader.hpp>
#include <glimac/FreeflyCamera.hpp>




using namespace glimac;

// ------------------------------------------
// Fonctions pour automatiser
// ------------------------------------------

std::unique_ptr<Image> loadAndBindTextures(std::string img_src, GLuint *textures, int position)
{

    std::unique_ptr<Image> img_ptr = loadImage(img_src);
    if (!img_ptr)
        std::cout << "img ptr null " << std::endl;

    // binder la texture sur la cible GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, textures[position]);
    // envoyer img au GPU pour stockée dans la texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 img_ptr->getWidth(), img_ptr->getHeight(),
                 0, GL_RGBA, GL_FLOAT,
                 (const void *)img_ptr->getPixels());
    // spécifier les filtres à appliquer
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // débinder la texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return img_ptr;
}



// ------------------------------------------
// MAIN
// ------------------------------------------

int main(int argc, char** argv) {

    // Initialize SDL and open a window
    GLuint largeur = 800;
    GLuint hauteur = 600;
    SDLWindowManager windowManager(largeur, hauteur, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    //---------------------------------
    // Spheres
    //---------------------------------
    
    // Sphere sphere = Sphere(1, 32, 16); // sphere de rayon 1 et discrétisé selon 32 segments sur la lattitude et 16 sur la longitude
    // GLuint nvertices = sphere.getVertexCount(); //Nombre de sommet
    // const ShapeVertex* vertices = sphere.getDataPointer();//pointeur vers le tableau de sommets

    
    // Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ("../assets/models/museum/scene_full.obj", vertices, uvs, normals);



    //---------------------------------
    // Textures => load doit être fait avant la boucle de rendu
    //---------------------------------
    GLuint texSize = 5;

    GLuint textures[texSize];
    glGenTextures(texSize, textures);

    std::unique_ptr<Image> imgTerre = loadAndBindTextures("../assets/textures/EarthMap.jpg", textures, 0);
    std::unique_ptr<Image> imgFragonard_a = loadAndBindTextures("../assets/textures/fragonard_armide.jpg", textures, 1);
    std::unique_ptr<Image> imgFragonard_b = loadAndBindTextures("../assets/textures/fragonard_balance.jpg", textures, 2);
    std::unique_ptr<Image> imgCourbet_a = loadAndBindTextures("../assets/textures/courbet_autoportrait.jpg", textures, 3);
    std::unique_ptr<Image> imgCourbet_m = loadAndBindTextures("../assets/textures/courbet_mer.jpg", textures, 4);


    //---------------------------------
    // Load les shaders
    //---------------------------------

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/directionallight_tex.fs.glsl");
    program.use();

    //---------------------------------
    // Buffers et Vertices
    //---------------------------------
    
	GLuint vbos[3];  // pour les vertices, uvs, and normals
	glGenBuffers(3, vbos);
	// glBindBuffer(GL_ARRAY_BUFFER, *vbos);

    // glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(ShapeVertex), vertices, GL_STATIC_DRAW);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

    
	// Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind
    	    
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


    //---------------------------------
	// VAO
    //---------------------------------

	// Faire attention que le vao soit bien bindé et que que ce soit le bon vao
	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_NORMAL = 1;
	const GLuint VERTEX_ATTR_TEXTURE = 2;


	GLuint vaos[3];
	glGenVertexArrays(3, vaos);
	glBindVertexArray(*vaos);
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);
	
    
	// glBindVertexArray(vaos[0]);
	// glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);	
	glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,0, (void*)0 );
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	// glBindVertexArray(vaos[1]);
	// glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	// glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	// glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	// glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
	glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,0, (void*)0 );
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	// glBindVertexArray(vaos[2]);
	// glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	// glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	// glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	// glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
	glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,0, (void*)0 );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	


    // glBindBuffer(GL_ARRAY_BUFFER, *vbos);
    // glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex), offsetof(ShapeVertex, position));
    // glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, normal)));
    // glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, texCoords)));
	

    // Debind vao
	glBindVertexArray(0);
    

    //---------------------------------
    // Variables uniformes
    //---------------------------------

    GLuint locationMVPMatrix = glGetUniformLocation(program.getGLId(),"uMVPMatrix");
    GLuint locationMVMatrix = glGetUniformLocation(program.getGLId(),"uMVMatrix");
    GLuint locationNormalMatrix = glGetUniformLocation(program.getGLId(),"uNormalMatrix");
    
    // Récupère la location des textures dans le shader
    GLuint locationTex = glGetUniformLocation(program.getGLId(),"uEarthTexture");
    glUniform1i(locationTex, 0);
    
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_ALWAYS); 

    // Autres variables uniformes
    glm::vec3 uKd = glm::vec3(1, 1, 1);             // coefficient de reflection diffuse de l'objet
    glm::vec3 uKs = glm::vec3(1, 1, 1);             // coefficient de reflection glossy de l'objet
    GLfloat uShininess = 1;                         // exposant de brillance (taille de brillance glossy)
    glm::vec3 uLightDir_vs = glm::vec3(1, 1, -1);    // direction incidente
    glm::vec3 uLightIntensity = glm::vec3(1, 1, 1); // intensite de la lumière incidente
    
    GLuint locationKd = glGetUniformLocation(program.getGLId(), "uKd");
    GLuint locationKs = glGetUniformLocation(program.getGLId(), "uKs");
    GLuint locationShininess = glGetUniformLocation(program.getGLId(), "uShininess");
    GLuint locationLightDir_vs = glGetUniformLocation(program.getGLId(), "uLightDir_vs");
    GLuint locationLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");


    //---------------------------------
    // Boucle des drawings
    //---------------------------------
    FreeflyCamera Freefly = FreeflyCamera();
    glm::ivec2 lastMousePos;
    
    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(*vaos);
        // glBindVertexArray(vaos[0]);


        bool ClickDroit = windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT);
        glm::ivec2 Souris = windowManager.getMousePosition();

        if (ClickDroit)
        {
            int dx = Souris.x - lastMousePos.x;
            int dy = Souris.y - lastMousePos.y;

            Freefly.rotateLeft(-dy);
            Freefly.rotateUp(-dx);
        }

        if (windowManager.isKeyPressed(SDLK_z))
        {
            Freefly.moveFront(0.1);
        }
        else if (windowManager.isKeyPressed(SDLK_q))
        {
            Freefly.moveLeft(0.1);
        }
        else if (windowManager.isKeyPressed(SDLK_s))
        {
            Freefly.moveFront(-0.1);
        }
        else if (windowManager.isKeyPressed(SDLK_d))
        {
            Freefly.moveLeft(-0.1);
        } // Pareille pour reculer et pour aller a gauche et a droite

        lastMousePos = Souris;

        glm::mat4 MatView = Freefly.getViewMatrix();
        
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)largeur/(GLfloat)hauteur, 0.1f, 400.f); 
        glm::mat4 MVMatrix = MatView*glm::mat4(1.f);
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        
        glUniformMatrix4fv(locationMVMatrix,1,GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(locationNormalMatrix,1,GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(locationMVPMatrix,1,GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));

        glUniform1f(locationShininess, uShininess);
        glUniform3fv(locationKd, 1, glm::value_ptr(uKd));
        glUniform3fv(locationKs, 1, glm::value_ptr(uKs));
        glUniform3fv(locationLightIntensity, 1, glm::value_ptr(uLightIntensity));
        glm::vec3 lightDir_vs = glm::vec3(MatView * glm::vec4(uLightDir_vs, 0));
        glUniform3fv(locationLightDir_vs, 1, glm::value_ptr(lightDir_vs));


        // bindez la texture sur la cible GL_TEXTURE_2D
        glUniform1i(locationTex, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textures[1]);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size()); 
        // glDrawArrays(GL_TRIANGLES, 0, nvertices); 

        glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
