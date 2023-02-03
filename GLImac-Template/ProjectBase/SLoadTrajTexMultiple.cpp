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
#include <objects/Objloader.hpp>
#include <glimac/FreeflyCamera.hpp>

#include <glimac/Model.h>
#include <glimac/Mesh.h>
#include <glimac/Material.h>
#include <glimac/Texture.h>


using namespace glimac;


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
    
    FilePath applicationPath(argv[0]);

    FilePath VShaderPath = applicationPath.dirPath() + "shaders/3D.vs.glsl";
    FilePath FShaderPath = applicationPath.dirPath() + "shaders/directionallight_tex.fs.glsl";

    Model scene = Model("../assets/models/museum/scene_full.obj", VShaderPath, FShaderPath);
    Model wagon = Model("../assets/models/museum/wagon.obj", VShaderPath, FShaderPath);
    Model rabbit = Model("../assets/models/museum/rabbitPlayer.obj", VShaderPath, FShaderPath);

        
    // Read la trajectoire
	std::vector<glm::vec3> verticesTraj;
	bool resTraj = loadVertices("../assets/models/museum/trajectoire.obj", verticesTraj);
    GLuint nverticesTraj = verticesTraj.size();

        // std::cout << "obj1 loaded, nvertices = " <<nvertices << std::endl;

    //---------------------------------
    // Textures
    //---------------------------------
    GLuint texSize = 13;

    GLuint textures[texSize];
    glGenTextures(texSize, textures);

    Texture texCaroline = Texture("../assets/textures/caroline.jpg", textures, 0, GL_TEXTURE_2D);
    Texture texFragonard_a = Texture("../assets/textures/fragonard_armide.jpg", textures, 1, GL_TEXTURE_2D);
    Texture texFragonard_b = Texture("../assets/textures/fragonard_balance.jpg", textures, 2, GL_TEXTURE_2D);
    Texture texCourbet_a = Texture("../assets/textures/courbet_autoportrait.jpg", textures, 3, GL_TEXTURE_2D);
    Texture texCourbet_m = Texture("../assets/textures/courbet_mer.jpg", textures, 4, GL_TEXTURE_2D);    
    Texture texCabanel_f = Texture("../assets/textures/cabanel_fallen_angel.jpg", textures, 5, GL_TEXTURE_2D);
    Texture texCabanel_p = Texture("../assets/textures/cabanel_phedre.jpg", textures, 6, GL_TEXTURE_2D);
    Texture texPrintemps = Texture("../assets/textures/printemps.jpg", textures, 7, GL_TEXTURE_2D);
    Texture texCheval = Texture("../assets/textures/cheval.jpg", textures, 8, GL_TEXTURE_2D);
    Texture texDioSkuros = Texture("../assets/textures/DioSkuros.jpg", textures, 9, GL_TEXTURE_2D);
    Texture texEdmon = Texture("../assets/textures/edmon.jpg", textures, 10, GL_TEXTURE_2D);
    Texture texJundi = Texture("../assets/textures/hujundi_crane.jpg", textures, 11, GL_TEXTURE_2D);
    Texture texJundi_r = Texture("../assets/textures/jundi_red.jpg", textures, 12, GL_TEXTURE_2D);

    std::vector<Texture> vecTextures;
    vecTextures.push_back(texCaroline);
    vecTextures.push_back(texFragonard_a);
    vecTextures.push_back(texFragonard_b);
    vecTextures.push_back(texCourbet_a);
    vecTextures.push_back(texCourbet_m);
    vecTextures.push_back(texCabanel_f);
    vecTextures.push_back(texCabanel_p);
    vecTextures.push_back(texCaroline);
    vecTextures.push_back(texCheval);
    vecTextures.push_back(texDioSkuros);
    vecTextures.push_back(texEdmon);
    vecTextures.push_back(texJundi);
    vecTextures.push_back(texJundi_r);
    

        // std::cout << "texture loaded" << std::endl;

    //---------------------------------
    // Variables uniformes
    //---------------------------------

    glEnable(GL_DEPTH_TEST);

    // glm::vec3 uLightDir_vs = glm::vec3(1, 1, -1);    // direction incidente
    // glm::vec3 uLightIntensity = glm::vec3(1, 1, 1); // intensite de la lumière incidente
    
    // GLuint locationLightDir_vs = glGetUniformLocation(program.getGLId(), "uLightDir_vs");
    // GLuint locationLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");

        // std::cout << "variables uniformes ok" << std::endl;

    //---------------------------------
    // Boucle des drawings
    //---------------------------------
    FreeflyCamera Freefly = FreeflyCamera();
    glm::ivec2 lastMousePos;
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


        // glUniform3fv(locationLightIntensity, 1, glm::value_ptr(uLightIntensity));
        // glm::vec3 lightDir_vs = glm::vec3(MatView * glm::vec4(uLightDir_vs, 0));
        // glUniform3fv(locationLightDir_vs, 1, glm::value_ptr(lightDir_vs));


        glm::mat4 MatView = Freefly.getViewMatrix();
       
        scene.render(MatView,vecTextures);
       

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

        lastMousePos = Souris;


        float angle = 0.f;
        if (x>0) angle = glm::acos(glm::dot(glm::normalize(verticesTraj[x]-verticesTraj[x-1]), glm::vec3(0,0,1)));
        glm::mat4 Model = glm::translate(glm::mat4(1.f),verticesTraj[x])
            *glm::rotate(glm::mat4(1.f),angle,glm::vec3(0,1,0))*offset;
        glm::mat4 MVMatrix2 = MatView*Model;

        wagon.setModel(Model);
        rabbit.setModel(Model);

        Freefly.setPosition(verticesTraj[x]+3.5f*vecOffset);


        wagon.render(MatView,vecTextures);
        rabbit.render(MatView,vecTextures);
        

        }
        x=x+1;
        x%=verticesTraj.size();

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}