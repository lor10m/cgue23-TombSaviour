#include "Utils.h"
#include "stb_image.h"
#include "TerrainShader.h"
#include "TerrainCamera.h"
#include "Callbacks.h"
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "PhysxCamera.h"


double camera_fov =  60 * 3.141592 / 180.0;
double camera_near = 0.1;
double camera_far = 100;

bool polygonMode = false;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        EXIT_WITH_ERROR("Failed to init GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #if _DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    INIReader reader("assets/settings.ini");
    GLFWwindow* window = glfwCreateWindow(reader.GetInteger("window", "width", 800), ("window", "height", 800), "LearnOpenGL: Terrain GPU", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    glewExperimental = true;
    if (GLEW_OK != glewInit()) {
        EXIT_WITH_ERROR("Failed to init GLEW");
    }

    if (!initFramework()) {
        EXIT_WITH_ERROR("Failed to init framework");
    }
   
    glEnable(GL_DEPTH_TEST);

    if (polygonMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    Camera camera(window, camera_fov, (double)800 / (double)800, camera_near, camera_far);
    //PhysxCamera camera(45.0f, 1.33f, 0.1f, 1000.0f);

    // build and compile our shader program
    // ------------------------------------
    TerrainShader tessHeightMapShader("assets/shaders/terrainVertex.vs", "assets/shaders/terrainFragment.fs", nullptr,            // if wishing to render as is
        "assets/shaders/terrain.tcs", "assets/shaders/terrain.tes");

    Terrain terrain(tessHeightMapShader, "assets/textures/sand.png", "assets/heightmaps/hm3.png");
   
    PhysxScene physxScene;
    physxScene.createTerrain("assets/heightmaps/hm3.png");
    //physxScene.createPlayer(camera);


    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        physxScene.simulate(window, 1.0f / 60.0f);
        camera.pollInput(window);
        camera.pollMousePosition(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the terrain
        terrain.render();
        // view/projection transformations
        glm::mat4 projection = glm::mat4(1.0f);
        //glm::mat4 view = camera.getViewMatrix();
        glm::mat4 view = camera.getTransformMatrix();
       
        tessHeightMapShader.setMat4("projection", projection);
        tessHeightMapShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        tessHeightMapShader.setMat4("model", model);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    terrain.deleteTerrain();

    glfwTerminate();
    return 0;
}