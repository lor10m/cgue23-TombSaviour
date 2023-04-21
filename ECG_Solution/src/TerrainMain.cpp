#include "Utils.h"
#include "stb_image.h"
#include "TerrainShader.h"
#include "TerrainCamera.h"
#include "Callbacks.h"
#include <iostream>
#include <vector>
#include "PxPhysicsAPI.h"
#include "Camera.h"
#include "Terrain.h"
#include "PhysxScene.h"
#include "PhysxCamera.h"
#include "Character.h"

#include "Drawables/Model.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Hdu.h"


using namespace physx;

double camera_fov =  60 * 3.141592 / 180.0;
double camera_near = 0.1;
double camera_far = 1000;

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

    Camera camera(window, camera_fov, (double)800 / (double)800, camera_near, camera_far, false);
    Hdu hdu(window, camera);

    // build and compile our shader program
    // ------------------------------------

    // TERRAIN
    TerrainShader tessHeightMapShader("assets/shaders/terrainVertex.vs", "assets/shaders/terrainFragment.fs", nullptr,
        "assets/shaders/terrain.tcs", "assets/shaders/terrain.tes");

    const char* terrain_path = "assets/heightmaps/hm4_dark.png";
    Terrain terrain(tessHeightMapShader, "assets/textures/sand.png", terrain_path);

    PhysxScene physxScene;
    physxScene.createTerrain(terrain_path);
    //physxScene.createPlayer();
    
    // create Player: 
    PxControllerManager* gManager = PxCreateControllerManager(*physxScene.scene);
    PxCapsuleControllerDesc cDesc;
    cDesc.position = PxExtendedVec3{ 0.0f, 30.0f, 0.0f };
    cDesc.contactOffset = 0.05f;
    cDesc.height = 6.0f;
    cDesc.radius = 1.0f;
    cDesc.stepOffset = 0.2f;
    cDesc.slopeLimit = 0.2f;
    cDesc.upDirection = PxVec3(0.0f, 1.0f, 0.0f);
    cDesc.material = physxScene.material;
    PxController* pxChar = gManager->createController(cDesc);
    pxChar->getActor()->setName("mummy");

    Character mummy(&camera, pxChar, window);

    // OBJECT
    PointLight pointLight1({ 0, 0, 0 }, { 1, 1, 1 }, { 1.0f, 0.4f, 0.1f });
    DirectionalLight directionalLight1({ 0, -1, -1 }, { 0.8f, 0.8f, 0.8f });

    ShaderManager shaderManager;
    shaderManager.addPointLight(pointLight1);
    shaderManager.addDirectionalLight(directionalLight1);

    // Palm tree:
    Shader* palmTreeShader = shaderManager.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", 0.1f, 0.7f, 0.1f, 2);
    glm::vec3 palmPosition = glm::vec3(30.0, 30.0, 30.0);
    glm::vec3 palmRotate = glm::vec3(glm::radians(-90.0f), glm::radians(0.0f), glm::radians(0.0f));
    glm::vec3 palmScale = glm::vec3(0.01, 0.01, 0.01);
    Transform palmTransform;
    palmTransform.translate(palmPosition);
    palmTransform.rotate(palmRotate);
    palmTransform.scale(palmScale);

    Model model_palmTree("assets/objects/palm_tree.obj");
    palmTreeShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, palmTransform.getMatrix());
    physxScene.createModel(model_palmTree.indices, model_palmTree.vertices, model_palmTree.normals, palmPosition,  palmScale, palmRotate);
    
    // Pyramid: 
    Shader* pyramidShader = shaderManager.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", 0.1f, 0.7f, 0.1f, 2);
    glm::vec3 pyramidPosition = glm::vec3(30.0, 30.0, 30.0);
    glm::vec3 pyramidRotate = glm::vec3(glm::radians(-90.0f), glm::radians(90.0f), glm::radians(0.0f));
    glm::vec3 pyramidScale = glm::vec3(1, 1, 1);
    Transform pyramidTransform;
    pyramidTransform.translate(pyramidPosition);
    pyramidTransform.rotate(pyramidRotate);
    pyramidTransform.scale(pyramidScale);
    Model model_pyramid("assets/objects/pyramid1.obj");
    pyramidShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, pyramidTransform.getMatrix());
    physxScene.createModel(model_pyramid.indices, model_pyramid.vertices, model_pyramid.normals, pyramidPosition, pyramidScale, pyramidRotate);

    // Spike: 
    Shader* spikeShader = shaderManager.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", 0.1f, 0.7f, 0.1f, 2);
    glm::vec3 spikePosition = glm::vec3(0.0, 30.0, 0.0);
    glm::vec3 spikeRotate = glm::vec3(glm::radians(-90.0f), glm::radians(90.0f), glm::radians(0.0f));
    glm::vec3 spikeScale = glm::vec3(1, 1, 1);
    Transform spikeTransform;
    spikeTransform.translate(spikePosition);
    spikeTransform.rotate(spikeRotate);
    spikeTransform.scale(spikeScale);
    Model model_spike("assets/objects/shoot_spike.obj");
    spikeShader->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, spikeTransform.getMatrix());
    physxScene.createModel(model_spike.indices, model_spike.vertices, model_spike.normals, spikePosition, spikeScale, spikeRotate);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glm::vec3 curCameraPos = camera.getCameraPosition();
    glm::vec3 prevCameraPos = glm::vec3(0,0,0);

    /* ------------------------------------------- */
    // Init HUD
    /* ------------------------------------------- */



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        physxScene.simulate(window, 1.0f / 60.0f);
        //camera.pollInput(window);
        camera.pollMousePosition(window);

        // Character: 
        float dt = 0.08;
        mummy.pollInput(window, dt);

        // render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // render the terrain
        terrain.render();
        glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 view = camera.getTransformMatrix();
       
        tessHeightMapShader.setMat4("projection", projection);
        tessHeightMapShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        tessHeightMapShader.setMat4("model", model);


        // render model
        shaderManager.updateCameraValues(camera);

        pyramidShader->activate();
        model_pyramid.draw();

        palmTreeShader->activate();
        model_palmTree.draw();

        shaderManager.updateCameraValues(hdu.hduCamera);
        hdu.drawHDU();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    terrain.deleteTerrain();

    glfwTerminate();
    return 0;
}