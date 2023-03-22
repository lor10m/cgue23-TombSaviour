///*
//* Copyright 2021 Vienna University of Technology.
//* Institute of Computer Graphics and Algorithms.
//* This file is part of the ECG Lab Framework and must not be redistributed.
//*/
//#pragma once
//
//#define _USE_MATH_DEFINES
//#include <cmath>
//#include <fstream>
//#include <sstream>
//#include <glm/gtc/type_ptr.hpp>
//#include "Utils.h"
//#include "Callbacks.h"
//#include "Renderer.h"
//#include "Shader.h"
//#include "Transform.h"
//#include "Camera.h"
//#include "Drawables/Sphere.h"
//#include "Drawables/Cylinder.h"
//#include "Drawables/Cube.h"
//#include "Drawables/Torus.h"
//#include "Vertex.h"
//#include "Lights/PointLight.h"
//#include "Lights/DirectionalLight.h"
//#include "ShaderManager.h"
//#include "Lights/SpotLight.h"
//#include "Drawables/PhysxObject.h"
//#include "GlobalVariables.h"
//
///* --------------------------------------------- */
//// Prototypes
///* --------------------------------------------- */
//
///* --------------------------------------------- */
//// Global variables
///* --------------------------------------------- */
//
///* --------------------------------------------- */
//// Main
///* --------------------------------------------- */
//
//int main(int argc, char** argv) {
//    /* --------------------------------------------- */
//    // Load settings.ini
//    /* --------------------------------------------- */
//
//    
//    // init reader for ini files
//    INIReader reader("assets/settings.ini");
//
//    // load values from ini file
//    // first param: section [window], second param: property name, third param: default value
//    int window_width = reader.GetInteger("window", "width", 800);
//    int window_height = reader.GetInteger("window", "height", 800);
//    int refresh_rate = reader.GetInteger("window", "refresh_rate", 60);
//    std::string window_title = reader.Get("window", "title", "Tomb Saviour");
//
//    double camera_fov = reader.GetReal("camera", "fov", 60) * M_PI / 180.0;
//    double camera_near = reader.GetReal("camera", "near", 0.1);
//    double camera_far = reader.GetReal("camera", "far", 100);
//
//    int brightnessIdx = reader.GetReal("global", "brightnessIdx", 10);
//    setIllumination(brightnessIdx);
//    std::cout << "Illumination: " << getIlluminationMultiplier() << "\n";
//
//    /* --------------------------------------------- */
//    // Init framework
//    /* --------------------------------------------- */
//
//    if (!glfwInit()) {
//        EXIT_WITH_ERROR("Failed to init GLFW");
//    }
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    // set refresh rate:
//    glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
//
//#if _DEBUG
//    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
//#endif
//
//    GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title.c_str(), nullptr, nullptr);
//    if (!window) {
//        EXIT_WITH_ERROR("Failed to init window");
//    }
//    glfwMakeContextCurrent(window);
//    glfwSwapInterval(1);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//
//    glewExperimental = true;
//    if (GLEW_OK != glewInit()) {
//        EXIT_WITH_ERROR("Failed to init GLEW");
//    }
//
//    glfwSetKeyCallback(window, keyCallback);
//#if _DEBUG
//    // Register your callback function.
//    glDebugMessageCallback(debugCallback, nullptr);
//    // Enable synchronous callback. This ensures that your callback function is called
//    // right after an error has occurred.
//    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//#endif
//
//    if (!initFramework()) {
//        EXIT_WITH_ERROR("Failed to init framework");
//    }
//
//    /* --------------------------------------------- */
//    // Initialize scene and draw loop
//    /* --------------------------------------------- */
//    {
//        glClearColor(1, 1, 1, 1);
//
//        double lastTime = glfwGetTime();
//        int nbFrames = 0;
//        double AvgTimeBetweenFrames, oneUnit, velocity = 0.0;
//
//        Camera camera(window, camera_fov, (double)window_width / (double)window_height, camera_near, camera_far);
//
//        Renderer renderer;
//        //glm::mat4 viewMatrix{};
//
//        //Cube cube(1.5f, 1.5f, 1.5f);
//        PhysxObject box = PhysxObject(glm::vec3(0.0, 0.0, 0.0));
//        box.getPosition();
//        Sphere sphere(64, 32, 1.0f);
//        //Cylinder cylinder(31, 1.3f, 1.0f);
//
//        PointLight pointLight1({ 0, 0, 0 }, { 1, 1, 1 }, { 1.0f, 0.4f, 0.1f });
//        DirectionalLight directionalLight1({ 0, -1, -1 }, { 0.8f, 0.8f, 0.8f });
//
//        ShaderManager shaderManager;
//        shaderManager.addPointLight(pointLight1);
//        shaderManager.addDirectionalLight(directionalLight1);
//
//        Shader* cubeShader = shaderManager.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", 0.1f, 0.7f, 0.1f, 2);
//        //Shader* cylinderShader = shaderManager.createPhongShader("assets/textures/tiles_diffuse.dds", "assets/textures/tiles_specular.dds", 0.1f, 0.7f, 0.3f, 8);
//        Shader* sphereShader = shaderManager.createPhongShader("assets/textures/tiles_diffuse.dds", "assets/textures/tiles_specular.dds", 0.1f, 0.7f, 0.3f, 8);
//
//        Transform cubeTransform;
//        //cubeTransform.translate(0, 0, 0);
//        glm::vec3 pos = box.getPosition();
//        std::cout << "Box position: X " << pos.x << "Y: " << pos.y << "Z: " << pos.z;
//        cubeTransform.translate(pos.x, pos.y, pos.z);
//        Transform sphereTransform;
//        sphereTransform.translate(0, 1.4f, 0);
//        Transform cylinderTransform;
//        cylinderTransform.translate(1.4f, -1.0f, 0);
//
//        cubeShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, cubeTransform.getMatrix());
//        sphereShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, sphereTransform.getMatrix());
//        //cylinderShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, cylinderTransform.getMatrix());
//
//        // adjust wndow size callbacks
//        glfwSetWindowUserPointer(window, &camera);
//        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
//
//        while (!glfwWindowShouldClose(window)) {
//            camera.pollInput(window);
//            camera.pollMousePosition(window);
//            renderer.clear();
//            glfwPollEvents();
//
//            camera.cameraSpeed = velocity;   // not sure if this makes sense because it's always 1
//
//            shaderManager.updateCameraValues(camera);
//
//            //std::cout << camera.cameraFront.x << camera.cameraFront.y << camera.cameraFront.z;
//            //renderer.renderDrawable(*cubeShader, cube);
//            cubeShader->activate();
//            box.draw();
//
//            renderer.renderDrawable(*sphereShader, sphere);
//            //renderer.renderDrawable(*cylinderShader, cylinder);
//
//            glfwSwapBuffers(window);
//            /* Gitlab CI automatic testing */
//            if (argc > 1 && std::string(argv[1]) == "--run_headless") {
//                saveScreenshot("screenshot", window_width, window_height);
//                break;
//            }
//        }
//    }
//
//    /* --------------------------------------------- */
//    // Destroy framework
//    /* --------------------------------------------- */
//
//    destroyFramework();
//
//    /* --------------------------------------------- */
//    // Destroy context and exit
//    /* --------------------------------------------- */
//
//    glfwTerminate();
//
//    return EXIT_SUCCESS;
//}