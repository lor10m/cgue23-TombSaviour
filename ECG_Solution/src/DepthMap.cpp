//#include "DepthMap.h"
//
//DepthMap::DepthMap()
//{
//}
//
//void DepthMap::createTerrainDepthMap()
//{
//    // create depth texture
//    glGenTextures(1, &depthMap);
//    glBindTexture(GL_TEXTURE_2D, depthMap);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // attach depth texture as FBO's depth buffer
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void DepthMap::render(Shader* simpleDepthShader, Shader* terrainShader, Terrain* terrain, glm::mat4 viewMatrix, glm::mat4 projection)
//{
//
//    // 1. render depth of scene to texture (from light's perspective)
//   // --------------------------------------------------------------
//    glm::mat4 lightProjection, lightView;
//    glm::mat4 lightSpaceMatrix;
//    float near_plane = 1.0f, far_plane = 7.5f;
//    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
//    lightSpaceMatrix = lightProjection * lightView;
//
//    // render scene from light's point of view
//    simpleDepthShader->setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
//    simpleDepthShader->setUniformMatrix4fv("projection", 1, GL_FALSE, projection);
//    simpleDepthShader->setUniformMatrix4fv("view", 1, GL_FALSE, viewMatrix);
//    simpleDepthShader->setUniformMatrix4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
//    
//    //depth to texture
//    glViewport(0, 0, 800,800);
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//     //RENDERN
//    terrain->render(simpleDepthShader, -1);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    // reset viewport
//    //glViewport(0, 0, 800, 800);
//    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // 2. render scene as normal using the generated depth/shadow map  
//    // --------------------------------------------------------------
//    // set light uniforms
//    //shader.setVec3("viewPos", camera.Position);
//    //shader.setVec3("lightPos", lightPos);
//    //shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
//    terrainShader->setUniformMatrix4fv("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);
//    terrainShader->setVec3("lightPos", lightPos);
//    terrainShader->setUniformMatrix4fv("projection", 1, GL_FALSE, projection);
//    terrainShader->setUniformMatrix4fv("view", 1, GL_FALSE, viewMatrix);
//    terrainShader->setUniformMatrix4fv("model", 1, GL_FALSE, glm::mat4(1.0f));
//
//    terrain->render(terrainShader, depthMap);
//
//}