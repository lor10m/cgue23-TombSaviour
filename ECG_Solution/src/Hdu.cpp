#include "Hdu.h"
#include "Utils/Transform.h"


Hdu::Hdu() {}

void Hdu::createHdu(GLFWwindow* window, Camera* threeDCamera) {

	Camera camera(window, threeDCamera->fov, (double)800 / (double)800, threeDCamera->near, threeDCamera->far, true);
	hduCamera = camera;

	simpleShader.createHDUShader("assets/textures/hdu/scorpions.dds");

	glfwGetWindowSize(window, &width, &height);

	/*glm::vec3 screenPosition = glm::vec3(120 - width / 2.0f, height / 2.0f - 90, 0.0f);
	glm::vec3 screenScale = glm::vec3(190.0f, 190.0f, 1.0f);
	Transform screenTransform;
	screenTransform.translate(screenPosition);
	screenTransform.scale(screenScale);*/

	glm::vec3 screenPosition = glm::vec3(0.5f, 0.75f, 0.0f);
	glm::vec3 screenScale = glm::vec3(1.0f, 1.0f, 1.0f);
	Transform screenTransform;
	screenTransform.translate(screenPosition);
	screenTransform.scale(screenScale);

	screenObject.generateModel("assets/objects/screen.obj");

	simpleShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, screenTransform.getMatrix());

	//initLevelGui();

}

void Hdu::drawHDU()
{
	simpleShader.activate();
	//simpleShader.changeShader(hduCamera);

	glm::mat4 proj = hduCamera.getProjectionMatrixHDU();
	simpleShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, hduCamera.getProjectionMatrixHDU());

	simpleShader.setUniform3f("eyePos", hduCamera.cameraPosition.x, hduCamera.cameraPosition.y, hduCamera.cameraPosition.z);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	screenObject.draw(&simpleShader);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	//std::cout << "I'm here";
}
//
//void Hdu::renderFirstHDU(GLFWwindow* window, Camera threeDCamera)
//{
//	glm::vec3 screenPosition = glm::vec3(120 - width / 2.0f, height / 2.0f - 90, 0.0f);
//	glm::vec3 screenScale = glm::vec3(190.0f, 190.0f, 1.0f);
//	Transform screenTransform;
//	screenTransform.translate(screenPosition);
//	screenTransform.scale(screenScale);
//
//
//
//	//_centerObj->getMaterial()->setTexture(Texture::loadTexture("loading.dds"));
//	//addObject(_centerObj);
//
//	drawHDU();
//}



