#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <cstddef>
#include <glm/glm.hpp>

#include <glimac/common.hpp>
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
        std::cout << "imgTriforce null " << std::endl;

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

std::vector<ShapeVertex> listOfVertex(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals) {
    std::vector<ShapeVertex> shapeVertices;
    
    for (auto i = 0; i < vertices.size(); i ++) {
        ShapeVertex crtVertex;
        
        crtVertex.position = vertices[i];
        crtVertex.texCoords = uvs[i];
        crtVertex.normal = normals[i];
        
        shapeVertices.push_back(crtVertex);
    }
    return shapeVertices; //&shapeVertices[0];
}


struct EarthProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/directionallight_tex.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uCloudTexture");
    }
};

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
    
    // Read our .obj file
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
    std::map<int, std::string> materialIndices;
	bool res = loadOBJ("../assets/models/museum/scene_full.obj", positions, uvs, normals, materialIndices);
	// bool res = loadOBJ("../assets/models/museum/rabbitPNJ.obj", positions, uvs, normals, materialIndices);
    const std::vector<ShapeVertex> vertices = listOfVertex(positions,uvs,normals);  
    GLuint nvertices = vertices.size();

    std::vector<glm::vec3> positions2;
	std::vector<glm::vec2> uvs2;
	std::vector<glm::vec3> normals2; // Won't be used at the moment.
	bool res2 = loadOBJ("../assets/models/museum/wagon.obj", positions2, uvs2, normals2);
    const std::vector<ShapeVertex> vertices2 = listOfVertex(positions2,uvs2,normals2);  
    GLuint nvertices2 = vertices2.size();
    
    std::vector<glm::vec3> positions3;
	std::vector<glm::vec2> uvs3;
	std::vector<glm::vec3> normals3; // Won't be used at the moment.
    bool res3 = loadOBJ("../assets/models/museum/rabbitPlayer.obj", positions3, uvs3, normals3);
    const std::vector<ShapeVertex> vertices3 = listOfVertex(positions3,uvs3,normals3);  
    GLuint nvertices3 = vertices3.size();
        
    // Read la trajectoire
	std::vector<glm::vec3> verticesTraj;
	bool resTraj = loadVertices("../assets/models/museum/trajectoire.obj", verticesTraj);
    GLuint nverticesTraj = verticesTraj.size();
	// std::cout  << verticesTraj.size() << std::endl;

        // std::cout << "obj1 loaded, nvertices = " <<nvertices << std::endl;

    //---------------------------------
    // Textures
    //---------------------------------

    std::vector<std::string> texPaths = {
        "../assets/textures/baccante.jpg",
        "../assets/textures/DioSkuros.jpg",
        "../assets/textures/edmon.jpg",
        "../assets/textures/wood.jpg",
        "../assets/textures/printemps.jpg",
        "../assets/textures/caroline.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/cabanel_fallen_angel.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/cabanel_phedre.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/courbet_autoportrait.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/courbet_mer.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/fragonard_balance.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/hujundi_crane.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/fragonard_armide.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/jundi_red.jpg",
        "../assets/textures/courbet_autoportrait.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/gris.jpg",
        "../assets/textures/gris.jpg"
    };

    GLuint texSize = texPaths.size();
    GLuint textures[texSize];
    glGenTextures(texSize, textures);

    for (int i=0; i<texSize; i++)
        loadAndBindTextures(texPaths[i], textures, i);

    // pour savoir quel texture pour quel point
    std::vector<int> material_positions;
    for(auto const& imap: materialIndices)
        material_positions.push_back(imap.first);

        // std::cout << "texture loaded" << std::endl;

    //---------------------------------
    // Load les shaders
    //---------------------------------

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/directionallight_tex.fs.glsl");
    program.use();

        // std::cout << "shader loaded" << std::endl;
    //---------------------------------
    // Buffers et Vertices
    //---------------------------------
    
	GLuint vbos[3];  // pour les vertices, uvs, and normals
	glGenBuffers(3, vbos);
	// glBindBuffer(GL_ARRAY_BUFFER, *vbos);
    
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(ShapeVertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, nvertices2 * sizeof(ShapeVertex), &vertices2[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, nvertices3 * sizeof(ShapeVertex), &vertices3[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

        // std::cout << "vbo ok" << std::endl;

    //---------------------------------
	// VAO
    //---------------------------------

	// Faire attention que le vao soit bien bindé et que que ce soit le bon vao
	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_NORMAL = 1;
	const GLuint VERTEX_ATTR_TEXTURE = 2;


	GLuint vaos[3];
	glGenVertexArrays(3, vaos);

    // bind vao et vbo
	glBindVertexArray(vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);	
	// Faire attention que le vao soit bien bindé et que que ce soit le bon vao
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    // glBindBuffer(GL_ARRAY_BUFFER, *vbos);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex), offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, texCoords)));
	// debind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // bind vao et vbo
	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);  
    // EnableVertexAttribArray   
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    // Associer les vertices avec pointer 
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex), offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, texCoords)));
	// debind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
     // bind vao et vbo
	glBindVertexArray(vaos[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);	
	// Faire attention que le vao soit bien bindé et que que ce soit le bon vao
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    // glBindBuffer(GL_ARRAY_BUFFER, *vbos);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex), offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE,sizeof(ShapeVertex),  (const GLvoid*)(offsetof(ShapeVertex, texCoords)));
	// debind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Debind vao
	glBindVertexArray(0);
    

        // std::cout << "vao ok" << std::endl;

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


        // std::cout << "variables uniformes ok" << std::endl;

    //---------------------------------
    // Boucle des drawings
    //---------------------------------
    FreeflyCamera Freefly = FreeflyCamera();
    glm::ivec2 lastMousePos;
    bool full_move = false;
    int x=0;
        
    // Décalage du wagon et du lapinou
    glm::vec3 vecOffset = glm::vec3(0,1.5,0); 
    glm::mat4 offset = glm::translate(glm::rotate(glm::mat4(1.f),glm::radians(-90.f),glm::vec3(0,1,0)),vecOffset);
    
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
        // glBindVertexArray(*vaos);
        glBindVertexArray(vaos[0]);

        
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

        for (int i=0; i<material_positions.size()-1; i++) {     
            // bindez la texture sur la cible GL_TEXTURE_2D
            glUniform1i(locationTex, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,textures[i%texSize]);
                
        // std::cout << "material_positions[i] = " << material_positions[i] << std::endl;
        
            glDrawArrays(GL_TRIANGLES, material_positions[i], material_positions[i+1]-material_positions[i]); 
        }
            // prendre en compte le dernier element des indices de matrice
            glUniform1i(locationTex, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,textures[3]);
                        
            glDrawArrays(GL_TRIANGLES, material_positions[material_positions.size()-1], nvertices-material_positions[material_positions.size()-1]); 


        for (int t=0; t<5000; t++) {

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

        // pour dessiner ton 2e obj
        glBindVertexArray(vaos[1]);    

        float angle = 0.f;
        if (x>0) angle = glm::acos(glm::dot(glm::normalize(verticesTraj[x]-verticesTraj[x-1]), glm::vec3(0,0,1)));
        //glm::mat4 Model = glm::translate(glm::mat4(1.f),(verticesTraj[x+1]-precVec))*offset;//*(float)(u/1000));
        glm::mat4 Model = glm::translate(glm::mat4(1.f),verticesTraj[x])
            *glm::rotate(glm::mat4(1.f),angle,glm::vec3(0,1,0))*offset;//*(float)(u/1000));
        glm::mat4 MVMatrix2 = MatView*Model;


        if (windowManager.isKeyPressed(SDLK_y)) {
            full_move = false;
        }  
        if (windowManager.isKeyPressed(SDLK_t)) {
            full_move = true;
        }     
        if (!full_move) {
            Freefly.setPosition(verticesTraj[x]+3.5f*vecOffset);
        }


        glm::mat4 NormalMatrix2 = glm::transpose(glm::inverse(MVMatrix2));
        glUniformMatrix4fv(locationMVMatrix,1,GL_FALSE, glm::value_ptr(MVMatrix2));
        glUniformMatrix4fv(locationNormalMatrix,1,GL_FALSE, glm::value_ptr(NormalMatrix2));
        glUniformMatrix4fv(locationMVPMatrix,1,GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix2));


   
        // bindez la texture sur la cible GL_TEXTURE_2D
        glUniform1i(locationTex, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        // dessiner ton 2e obj
        glDrawArrays(GL_TRIANGLES, 0, nvertices2); 

        
        // pour dessiner ton 3e obj
        glBindVertexArray(vaos[2]);        
        
        glUniformMatrix4fv(locationMVMatrix,1,GL_FALSE, glm::value_ptr(MVMatrix2));
        glUniformMatrix4fv(locationNormalMatrix,1,GL_FALSE, glm::value_ptr(NormalMatrix2));
        glUniformMatrix4fv(locationMVPMatrix,1,GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix2));

        // bindez la texture sur la cible GL_TEXTURE_2D
        //glUniform1i(locationTex, 0);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D,earthTexture);
        // dessiner ton 2e obj
        glDrawArrays(GL_TRIANGLES, 0, nvertices3); 

        glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(0);

            // time = windowManager.getTime()-timeRef;
        }
        x=x+1;
        x%=verticesTraj.size();

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}