#include "Objects.h"



Objects::Objects(GLFWwindow* window, Camera* camera, PhysxScene* physxScene)
{
    this->camera = camera;
    this->physxScene = physxScene;
    createTerrain();
    createMummy();
    createEnemy();
    createPyramid();
    createPalmTree();
    createHduObject(window);

}

void Objects::createTerrain()
{

    terrainShader.createTerrainShader();

    const char* heightmap_path = "assets/heightmaps/hm4_dark.png";
    terrain = Terrain(&terrainShader, "assets/textures/sand.png", heightmap_path);

    physxScene->createTerrain(heightmap_path);

}

void Objects::createMummy()
{
    mummy = Character(camera, physxScene, glm::vec3(0.0f, 30.0f, 0.0f), camera->window);
}

void Objects::createEnemy()
{
    glm::vec3 modelRotate = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 modelScale = glm::vec3(0.03, 0.03, 0.03);
    glm::vec3 modelTranslate = glm::vec3(0.0, 2.0f, 0.0);
    Transform modelTransform;
    modelTransform.translate(modelTranslate);
    modelTransform.rotate(modelRotate);
    modelTransform.scale(modelScale);

    enemyModel.generateModel("assets/models/eve.dae");

    enemyShader.createPhongShader(0.1f, 0.7f, 0.1f, 2);
    enemyShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, modelTransform.getMatrix());
    enemyShader.setUniform1i("isAnimated", 1);

    enemy = Enemy(&enemyModel, physxScene, modelScale, glm::vec3(0.0f, 25.64f, 0.0f));
}

void Objects::createPalmTree()
{
    glm::vec3 palmRotate = glm::vec3(glm::radians(-90.0f), glm::radians(0.0f), glm::radians(0.0f));
    glm::vec3 palmScale = glm::vec3(0.01, 0.01, 0.01);
    glm::vec3 palmTranslate = glm::vec3(0.0, 25.64, 0.0);
    Transform palmTransform;
    palmTransform.translate(palmTranslate);
    palmTransform.rotate(palmRotate);
    palmTransform.scale(palmScale);


    palmTreeShader.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", palmTransform.getMatrix(), 0.1f, 0.7f, 0.1f, 2);
    palmTreeShader.setUniform1i("isAnimated", 0);

    palmTree.generateModel("assets/objects/palm_tree.obj");

    physxScene->createModel(palmTree.indices, palmTree.vertices, palmTree.normals, palmScale, palmTranslate, palmRotate);
}

void Objects::createPyramid()
{
    glm::vec3 pyramidRotate = glm::vec3(glm::radians(-90.0f), glm::radians(90.0f), glm::radians(0.0f));
    glm::vec3 pyramidScale = glm::vec3(2, 2, 2);
    glm::vec3 pyramidTranslate = glm::vec3(5.0, 25.64, 0.0);
    Transform pyramidTransform;
    pyramidTransform.translate(pyramidTranslate);
    pyramidTransform.rotate(pyramidRotate);
    pyramidTransform.scale(pyramidScale);
    pyramid.generateModel("assets/objects/pyramid1.obj");

    pyramidShader.createPhongShader("assets/textures/wood_texture.dds", "assets/textures/wood_texture_specular.dds", pyramidTransform.getMatrix(), 0.1f, 0.7f, 0.1f, 2);
    pyramidShader.setUniform1i("isAnimated", 0);

    physxScene->createModel(pyramid.indices, pyramid.vertices, pyramid.normals, pyramidScale, pyramidTranslate, pyramidRotate);
}

void Objects::createHduObject(GLFWwindow* window)
{
    hduObject.createHdu(window, camera);

}

void Objects::render(GLFWwindow* window, float dt)//, Shader* enemyModelShader)
{
    // Character: 
    mummy.pollInput(window, dt);        // ca.: 
    std::cout << dt << "\n";

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 viewMatrix = camera->getTransformMatrix();


    // render the terrain
    terrainShader.setUniformMatrix4fv("projection", 1, GL_FALSE, projection);
    terrainShader.setUniformMatrix4fv("view", 1, GL_FALSE, viewMatrix);
    terrainShader.setUniformMatrix4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
    terrain.render();


    // render pyramid
    pyramidShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
    pyramidShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
    pyramid.draw(&pyramidShader);


    // render palm tree
    palmTreeShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
    palmTreeShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);
    palmTree.draw(&palmTreeShader);

    //render enemy
    enemyShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, camera->getTransformMatrix());
    enemyShader.setUniform3f("eyePos", camera->cameraPosition.x, camera->cameraPosition.y, camera->cameraPosition.z);

    std::vector<glm::mat4> transformationMatrices;
    enemyModel.getBoneTransforms(dt, glm::mat4(1.0f), transformationMatrices);
    for (unsigned int i = 0; i < transformationMatrices.size(); i++) {
        glm::mat4 mat = transformationMatrices[i];
        enemyShader.setUniformMatrix4fv("bones[" + std::to_string(i) + "]", 1, GL_FALSE, mat);
    }
    enemyModel.draw(&enemyShader);

    enemy.move(mummy.getPosition(), 0.2, dt);


    //hduObject->simpleShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, hduObject->hduCamera.getProjectionMatrixHDU());
    //hduObject->simpleShader.setUniform3f("eyePos", hduObject->hduCamera.cameraPosition.x, hduObject->hduCamera.cameraPosition.y, hduObject->hduCamera.cameraPosition.z);

    hduObject.drawHDU();

    // simulate physx
    //physxScene->simulate(window, 1.0f / 60.0f);     // min. 60 FPS and Framerate Independence
}

void Objects::deleteObjects()
{
    terrain.deleteTerrain();

}