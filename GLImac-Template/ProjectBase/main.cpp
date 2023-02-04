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

std::vector<Vertex> listOfVertex(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals)
{
    std::vector<Vertex> shapeVertices;

    for (auto i = 0; i < vertices.size(); i++)
    {
        Vertex crtVertex;

        crtVertex.position = vertices[i];
        crtVertex.texCoords = uvs[i];
        crtVertex.normal = normals[i];

        shapeVertices.push_back(crtVertex);
    }
    return shapeVertices; //&shapeVertices[0];
}

std::vector<Vertex> loadShape(std::string obj_src, std::map<int, std::string> &materialIndices)
{
    // Read our .obj file
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    bool res = loadOBJ(&obj_src[0], positions, uvs, normals, materialIndices);
    return listOfVertex(positions, uvs, normals);
}

void initVBO(GLuint *vbos, int idx, float nvertices, std::vector<Vertex> vertices)
{
    // Bind vector of vertex to vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbos[idx]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initVBOs(GLuint *vbos, int nshapes, std::vector<GLuint> Lnvertices, std::vector<std::vector<Vertex>> Lvertices)
{
    for (int i = 0; i < nshapes; i++)
        initVBO(vbos, i, Lnvertices[i], Lvertices[i]);
}

void initVAOs(GLuint *vaos, GLuint *vbos, int nshapes)
{
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXTURE = 2;

    for (int i = 0; i < nshapes; i++)
    {
        // bind vao et vbo
        glBindVertexArray(vaos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
        // Faire attention que le vao soit bien bindé et que que ce soit le bon vao
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
        // glBindBuffer(GL_ARRAY_BUFFER, *vbos);
        glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
        glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(offsetof(Vertex, normal)));
        glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)(offsetof(Vertex, texCoords)));
        // debind vbo
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Debind vao
        glBindVertexArray(0);
    }
}

glm::ivec2 checkActions(SDLWindowManager &windowManager, FreeflyCamera &Freefly, glm::ivec2 &lastMousePos, bool full_move)
{
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
        Freefly.moveFront(0.3);
    }
    else if (windowManager.isKeyPressed(SDLK_q))
    {
        Freefly.moveLeft(0.3);
    }
    else if (windowManager.isKeyPressed(SDLK_s))
    {
        Freefly.moveFront(-0.3);
    }
    else if (windowManager.isKeyPressed(SDLK_d))
    {
        Freefly.moveLeft(-0.3);
    }

    return Souris;
}

// ------------------------------------------
// MAIN
// ------------------------------------------

int main(int argc, char **argv)
{

    // Initialize SDL and open a window
    GLuint largeur = 800;
    GLuint hauteur = 600;
    SDLWindowManager windowManager(largeur, hauteur, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if (GLEW_OK != glewInitError)
    {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);

    //---------------------------------
    // Read our .obj files
    //---------------------------------

    std::map<int, std::string> materialIndices;
    const std::vector<Vertex> vertices = loadShape("../assets/models/museum/scene_full.obj", materialIndices);
    GLuint nvertices = vertices.size();

    std::map<int, std::string> materialIndices2;
    const std::vector<Vertex> vertices2 = loadShape("../assets/models/museum/wagon.obj", materialIndices2);
    GLuint nvertices2 = vertices2.size();

    std::map<int, std::string> materialIndices3;
    const std::vector<Vertex> vertices3 = loadShape("../assets/models/museum/rabbitPlayer.obj", materialIndices3);
    GLuint nvertices3 = vertices3.size();

    // Read la trajectoire
    std::vector<glm::vec3> verticesTraj;
    bool resTraj = loadVertices("../assets/models/museum/trajectoire.obj", verticesTraj);
    GLuint nverticesTraj = verticesTraj.size();
    // std::cout  << verticesTraj.size() << std::endl;

    std::cout << "obj1 loaded, nvertices = " << nvertices << std::endl;

    //---------------------------------
    // Textures
    //---------------------------------

    std::vector<std::string> texPaths = {
        "../assets/textures/baccante.jpg",
        "../assets/textures/DioSkuros.jpg",
        "../assets/textures/edmon.jpg",
        "../assets/textures/wood.jpg",
        "../assets/textures/brique.jpg",
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
        "../assets/textures/parquet.jpg",
        "../assets/textures/bois.jpg",
        "../assets/textures/gris.jpg",
        "../assets/textures/gris.jpg"};

    GLuint texSize = texPaths.size();
    GLuint textures[texSize];
    glGenTextures(texSize, textures);

    for (int i = 0; i < texSize; i++)
        loadAndBindTextures(texPaths[i], textures, i);

    // pour savoir quel texture pour quel point
    std::vector<int> material_positions;
    for (auto const &imap : materialIndices)
        material_positions.push_back(imap.first);

    std::cout << "texture loaded" << std::endl;

    //---------------------------------
    // Load les shaders
    //---------------------------------

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/resume.fs.glsl");
    program.use();

    std::cout << "shader loaded" << std::endl;
    //---------------------------------
    // Buffers et Vertices
    //---------------------------------

    int nshapes = 3;
    std::vector<GLuint> Lnvertices = {nvertices, nvertices2, nvertices3};
    std::vector<std::vector<Vertex>> Lvertices = {vertices, vertices2, vertices3};

    GLuint vbos[3]; // pour les vertices, uvs, and normals
    glGenBuffers(3, vbos);

    initVBOs(vbos, nshapes, Lnvertices, Lvertices);

    std::cout << "vbo ok" << std::endl;

    //---------------------------------
    // VAO
    //---------------------------------

    // Faire attention que le vao soit bien bindé et que que ce soit le bon vao

    GLuint vaos[3];
    glGenVertexArrays(3, vaos);

    initVAOs(vaos, vbos, nshapes);

    std::cout << "vao ok" << std::endl;

    //---------------------------------
    // Variables uniformes locations
    //---------------------------------

    GLuint locationMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLuint locationMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLuint locationNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    // Récupère la location des textures dans le shader
    GLuint locationTex = glGetUniformLocation(program.getGLId(), "uEarthTexture");
    glUniform1i(locationTex, 0);

    GLuint locationKd = glGetUniformLocation(program.getGLId(), "uKd");
    GLuint locationKs = glGetUniformLocation(program.getGLId(), "uKs");
    GLuint locationShininess = glGetUniformLocation(program.getGLId(), "uShininess");

    GLuint locationAmbient = glGetUniformLocation(program.getGLId(), "uAmbiantLight");

    GLuint locationLightDir_vs = glGetUniformLocation(program.getGLId(), "uLightDir_vs");
    GLuint locationLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");

    GLuint locationLightPos_vs1 = glGetUniformLocation(program.getGLId(), "uLightPos_vs1");
    GLuint locationLightPos_vs2 = glGetUniformLocation(program.getGLId(), "uLightPos_vs2");
    GLuint locationLightIntensity1 = glGetUniformLocation(program.getGLId(), "uLightIntensity1");
    GLuint locationLightIntensity2 = glGetUniformLocation(program.getGLId(), "uLightIntensity2");

    // valeurs à donner aux variables uniformes*
    glm::vec3 uAmbiantLight = 0.1f * glm::vec3(1, 1, 1);

    glm::vec3 uKd = 0.5f * glm::vec3(1, 1, 1); // coefficient de reflection diffuse de l'objet
    glm::vec3 uKs = 0.2f * glm::vec3(1, 1, 1); // coefficient de reflection glossy de l'objet
    GLfloat uShininess = 1;                    // exposant de brillance (taille de brillance glossy)

    glm::vec3 uLightPos_vs1 = glm::vec3(0, 2, 1);
    glm::vec3 uLightPos_vs2 = glm::vec3(2.5, 3.5, -7); // 1.428869 1.942768 -16.182192

    glm::vec3 uLightDir_vs = glm::vec3(1, 1, 1);    // direction incidente
    glm::vec3 uLightIntensity = glm::vec3(1, 1, 1); // intensite de la lumière incidente

    std::cout << "variables uniformes ok" << std::endl;

    //---------------------------------
    // Boucle des drawings
    //---------------------------------
    FreeflyCamera Freefly = FreeflyCamera();
    glm::ivec2 lastMousePos;
    bool dir_light = false;
    bool full_move = false;
    int x = 0;

    // Décalage du wagon et du lapinou
    glm::vec3 vecOffset = glm::vec3(0, 1.5, 0);
    glm::mat4 offset = glm::translate(glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0, 1, 0)), vecOffset);

    // Application loop:
    bool done = false;
    while (!done)
    {
        // Event loop:
        SDL_Event e;
        while (windowManager.pollEvent(e))
        {
            if (e.type == SDL_QUIT)
            {
                done = true; // Leave the loop after this iteration
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glBindVertexArray(*vaos);
        glBindVertexArray(vaos[0]);

        glm::mat4 MatView = Freefly.getViewMatrix();

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)largeur / (GLfloat)hauteur, 0.1f, 400.f);
        glm::mat4 MVMatrix = MatView * glm::mat4(1.f);
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(locationMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(locationNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glUniformMatrix4fv(locationMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));

        glUniform1f(locationShininess, uShininess);
        glUniform3fv(locationKd, 1, glm::value_ptr(uKd));
        glUniform3fv(locationKs, 1, glm::value_ptr(uKs));

        glm::vec3 lightDir_vs = glm::vec3(MatView * glm::vec4(uLightDir_vs, 0));
        glm::vec3 lightPos_vs2 = glm::vec3(MatView * glm::vec4(uLightPos_vs2, 1));

        lightDir_vs = static_cast<float>(dir_light) * lightDir_vs;

        glUniform3fv(locationAmbient, 1, glm::value_ptr(uAmbiantLight));

        glUniform3fv(locationLightDir_vs, 1, glm::value_ptr(lightDir_vs));
        glUniform3fv(locationLightPos_vs1, 1, glm::value_ptr(uLightPos_vs1));
        glUniform3fv(locationLightPos_vs2, 1, glm::value_ptr(lightPos_vs2));

        glUniform3fv(locationLightIntensity, 1, glm::value_ptr(uLightIntensity));
        glUniform3fv(locationLightIntensity1, 1, glm::value_ptr(10.f * uLightIntensity));
        glUniform3fv(locationLightIntensity2, 1, glm::value_ptr(10.f * uLightIntensity));

        for (int i = 0; i < material_positions.size() - 1; i++)
        {
            // bindez la texture sur la cible GL_TEXTURE_2D
            glUniform1i(locationTex, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[i % texSize]);

            // std::cout << "material_positions[i] = " << material_positions[i] << std::endl;

            glDrawArrays(GL_TRIANGLES, material_positions[i], material_positions[i + 1] - material_positions[i]);
        }
        // prendre en compte le dernier element des indices de matrice
        glUniform1i(locationTex, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[3]);

        glDrawArrays(GL_TRIANGLES, material_positions[material_positions.size() - 1], nvertices - material_positions[material_positions.size() - 1]);

        // vérifier les action du joueur
        lastMousePos = checkActions(windowManager, Freefly, lastMousePos, full_move);

        if (windowManager.isKeyPressed(SDLK_j))
        {
            dir_light = true;
        }
        if (windowManager.isKeyPressed(SDLK_n))
        {
            dir_light = false;
        }
        if (windowManager.isKeyPressed(SDLK_b))
        {
            full_move = false;
        }
        if (windowManager.isKeyPressed(SDLK_h))
        {
            full_move = true;
        }
        if (!full_move)
        {
            Freefly.setPosition(verticesTraj[x] + 3.5f * vecOffset);
        }

        for (int t = 0; t < 500; t++)
        {

            // pour dessiner ton 2e obj
            glBindVertexArray(vaos[1]);

            float angle = 0.f;
            if (x > 0)
                angle = glm::acos(glm::dot(glm::normalize(verticesTraj[x] - verticesTraj[x - 1]), glm::vec3(0, 0, 1)));
            // glm::mat4 Model = glm::translate(glm::mat4(1.f),(verticesTraj[x+1]-precVec))*offset;//*(float)(u/1000));
            glm::mat4 Model = glm::translate(glm::mat4(1.f), verticesTraj[x]) * glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * offset; //*(float)(u/1000));
            glm::mat4 MVMatrix2 = MatView * Model;

            glm::mat4 NormalMatrix2 = glm::transpose(glm::inverse(MVMatrix2));
            glUniformMatrix4fv(locationMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix2));
            glUniformMatrix4fv(locationNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix2));
            glUniformMatrix4fv(locationMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix2));

            glUniform1f(locationShininess, uShininess);
            glUniform3fv(locationKd, 1, glm::value_ptr(uKd));
            glUniform3fv(locationKs, 1, glm::value_ptr(uKs));

            // bindez la texture sur la cible GL_TEXTURE_2D
            glUniform1i(locationTex, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[2]);
            // dessiner ton 2e obj
            glDrawArrays(GL_TRIANGLES, 0, nvertices2);

            // pour dessiner ton 3e obj
            glBindVertexArray(vaos[2]);

            glUniformMatrix4fv(locationMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix2));
            glUniformMatrix4fv(locationNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix2));
            glUniformMatrix4fv(locationMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix2));

            // bindez la texture sur la cible GL_TEXTURE_2D
            glUniform1i(locationTex, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            // dessiner ton 2e obj
            glDrawArrays(GL_TRIANGLES, 0, nvertices3);

            // time = windowManager.getTime()-timeRef;
        }
        x = x + 1;
        x %= verticesTraj.size();

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteBuffers(1, vbos);
    glDeleteVertexArrays(1, vaos);

    return EXIT_SUCCESS;
}