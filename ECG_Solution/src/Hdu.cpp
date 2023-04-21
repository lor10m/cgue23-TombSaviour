#include "Hdu.h"
#include "Transform.h"


Hdu::Hdu() {}

Hdu::Hdu(GLFWwindow* window, Camera threeDCamera) {

	Camera camera(window, threeDCamera.fov, (double)800 / (double)800, threeDCamera.near, threeDCamera.far, true);
	hduCamera = camera;

	Shader shader("guiShader.vsh", "guiShader.fsh");
	simpleShader = shader;

	glfwGetWindowSize(window, &width, &height);

	Texture texture;
	startTexture = texture.getHDUTexture("tiles_diffuse.dds");		//hduTry

	glm::vec3 screenPosition = glm::vec3(120 - width / 2.0f, height / 2.0f - 90, 0.0f);
	glm::vec3 screenScale = glm::vec3(190.0f, 190.0f, 1.0f);
	Transform screenTransform;
	screenTransform.translate(screenPosition);
	screenTransform.scale(screenScale);

	Model model("assets/objects/screen.obj");
	screenObject = model;
	simpleShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, screenTransform.getMatrix());

	//initLevelGui();

}

void Hdu::drawHDU()
{
	simpleShader.activate();
	//simpleShader.changeShader(hduCamera);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	screenObject.draw();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	std::cout << "I'm here";

}

void Hdu::renderFirstHDU(GLFWwindow* window, Camera threeDCamera)
{
	glm::vec3 screenPosition = glm::vec3(120 - width / 2.0f, height / 2.0f - 90, 0.0f);
	glm::vec3 screenScale = glm::vec3(190.0f, 190.0f, 1.0f);
	Transform screenTransform;
	screenTransform.translate(screenPosition);
	screenTransform.scale(screenScale);



	//_centerObj->getMaterial()->setTexture(Texture::loadTexture("loading.dds"));
	//addObject(_centerObj);

	drawHDU();
}



