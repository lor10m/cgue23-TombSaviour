//#include "Utils.h"
//#include "stb_image.h"
//#include "TerrainShader.h"
//#include "TerrainCamera.h"
//#include "Callbacks.h"
//#include <iostream>
//#include <vector>
//#include "Camera.h"
//#include "Terrain.h"
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 800;
//const unsigned int NUM_PATCH_PTS = 4;
//
//// camera - give pretty starting point
//TerrainCamera terrainCamera(glm::vec3(0.0f, 0.0f, 10.0f),
//    glm::vec3(0.0f, 1.0f, 0.0f),
//    -90.0f, 0.0f);
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//double camera_fov =  60 * 3.141592 / 180.0;
//double camera_near = 0.1;
//double camera_far = 100;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    if (!glfwInit()) {
//        EXIT_WITH_ERROR("Failed to init GLFW");
//    }
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    #if _DEBUG
//        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
//    #endif
//
//    // glfw window creation
//    // --------------------
//    INIReader reader("assets/settings.ini");
//    GLFWwindow* window = glfwCreateWindow(reader.GetInteger("window", "width", 800), ("window", "height", 800), "LearnOpenGL: Terrain GPU", nullptr, nullptr);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetKeyCallback(window, keyCallback);
//
//    glewExperimental = true;
//    if (GLEW_OK != glewInit()) {
//        EXIT_WITH_ERROR("Failed to init GLEW");
//    }
//
//    if (!initFramework()) {
//        EXIT_WITH_ERROR("Failed to init framework");
//    }
//    
//
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDisable(GL_CULL_FACE);
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//
//    Camera camera(window, camera_fov, (double)800 / (double)800, camera_near, camera_far);
//
//    // build and compile our shader program
//    // ------------------------------------
//    TerrainShader tessHeightMapShader("assets/shaders/terrainVertex.vs", "assets/shaders/terrainFragment.fs", nullptr,            // if wishing to render as is
//        "assets/shaders/terrain.tcs", "assets/shaders/terrain.tes");
//
//    Terrain terrain(tessHeightMapShader, "assets/textures/tiles_diffuse.dds");
//
//    bool print = true;
//
//    glfwSetWindowUserPointer(window, &camera);
//    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        camera.pollInput(window);
//        camera.pollMousePosition(window);
//        // per-frame time logic
//        // --------------------
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // render
//        // ------
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // be sure to activate shader when setting uniforms/drawing objects
//        tessHeightMapShader.use();
//
//        // view/projection transformations
//        glm::mat4 projection = glm::mat4(1.0f);
//        glm::mat4 view = camera.getTransformMatrix();
//       
//        tessHeightMapShader.setMat4("projection", projection);
//        tessHeightMapShader.setMat4("view", view);
//
//        // world transformation
//        glm::mat4 model = glm::mat4(1.0f);
//        tessHeightMapShader.setMat4("model", model);
//
//        // render the terrain
//        terrain.render();
//        /*glBindVertexArray(terrainVAO);
//        glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * rez * rez);*/
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    terrain.deleteTerrain();
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}