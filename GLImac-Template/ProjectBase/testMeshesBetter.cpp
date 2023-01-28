#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <cstddef>
#include <glm/glm.hpp>
#include <glimac/FreeflyCamera.hpp>

#include <glimac/Geometry.hpp>


using namespace glimac;

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
    
    // Sphere sphere = Sphere(1, 32, 16);    
    // GLuint nvertices = sphere.getVertexCount();
    // const ShapeVertex* vertices = sphere.getDataPointer();


    Geometry scene = Geometry();
    const FilePath filepath = FilePath("../assets/models/museum/rabbitPlayer.obj");
    const FilePath mtlBasePath = FilePath("../assets/models/rabbitPlayer.mtl");
    bool loadTextures = true;
    bool loadObj = scene.loadOBJ(filepath, mtlBasePath, loadTextures);

    GLuint nvertices = scene.getVertexCount(); //Nombre de sommet
    const glimac::Geometry::Vertex* vertices = scene.getVertexBuffer();

    
    //---------------------------------
    // Load les shaders
    //---------------------------------

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/normals.fs.glsl");
    program.use();

    //---------------------------------
    // Buffers et Vertices
    //---------------------------------
    
	GLuint vbos[3];
	glGenBuffers(3, vbos);
    
	// Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);
	// glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(ShapeVertex), vertices, GL_STATIC_DRAW);
	// // Debind
    // glBindBuffer(GL_ARRAY_BUFFER, 0);



    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(glimac::Geometry::Vertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    //---------------------------------
	// VAO
    //---------------------------------

	GLuint vaos[3];
	glGenVertexArrays(3, vaos);
	glBindVertexArray(*vaos);
	
	// Faire attention que le vao soit bien bindé et que que ce soit le bon vao
	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_NORMAL = 1;
	const GLuint VERTEX_ATTR_TEXTURE = 2;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
	
	glBindBuffer(GL_ARRAY_BUFFER, *vbos);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(glimac::Geometry::Vertex), offsetof(glimac::Geometry::Vertex, m_Position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(glimac::Geometry::Vertex),  (const GLvoid*)(offsetof(glimac::Geometry::Vertex, m_Normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(glimac::Geometry::Vertex),  (const GLvoid*)(offsetof(glimac::Geometry::Vertex, m_TexCoords)));
	
    // Debind vbo et vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
    

    //---------------------------------
    // Variables uniformes
    //---------------------------------

    GLuint locationMVPMatrix = glGetUniformLocation(program.getGLId(),"uMVPMatrix");
    GLuint locationMVMatrix = glGetUniformLocation(program.getGLId(),"uMVMatrix");
    GLuint locationNormalMatrix = glGetUniformLocation(program.getGLId(),"uNormalMatrix");
    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)largeur/(GLfloat)hauteur, 0.1f, 100.f); 
    glm::mat4 MVMatrix_ = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -5.f));


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

        // bindez la texture sur la cible GL_TEXTURE_2D
        // glBindTexture(GL_TEXTURE_2D,textures[0]);
         
        glm::mat4 MVMatrix = MatView*MVMatrix_;
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(locationMVMatrix,1,GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(locationNormalMatrix,1,GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(locationMVPMatrix,1,GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));

        glDrawArrays(GL_TRIANGLES, 0, nvertices); 

        // glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(0);


        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
