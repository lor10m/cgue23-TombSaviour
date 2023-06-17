#include "Hdu.h"
#include <glm/gtx/string_cast.hpp>

Hdu::Hdu() {}

void Hdu::createHdu(GLFWwindow* window, Camera* threeDCamera, int lifeStartNumber) {

	Camera camera(window, threeDCamera->fov, (double)800 / (double)800, threeDCamera->near, threeDCamera->far, true);
	hduCamera = camera;
	currentLifeNr = lifeStartNumber;
	maxLifeNr = lifeStartNumber;

	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	calculateScale();
	calculatePosition();
	generateStartHDU();
	initGameHDU(lifeStartNumber);

	//initLevelGui();
}

// TODO (?)
void Hdu::generateStartHDU() {
	// optional, this should be more like the start window like "Welcome to our game! It's a me - Ramesses II.!"
}


void Hdu::initGameHDU(int lifeStartNumber) {

	// cacti HDU
	hduDataMap["Cacti"].simpleShader.createHDUShader("assets/textures/hdu/spikes.dds");
	hduDataMap["Cacti"].transform.translate(glm::vec3(hduDataMap["Cacti"].ndcXposition, 0.8f, 0.0f)).scale(hduDataMap["Cacti"].getScale(widthScale, heightScale));
	hduDataMap["Cacti"].model.generateModel("assets/objects/screenPanel.obj");

	// tens HDU
	hduDataMap["Tens"].simpleShader.createHDUShader("assets/textures/hdu/0.dds");
	hduDataMap["Tens"].scaleMulti = 0.5f;
	hduDataMap["Tens"].transform.translate(glm::vec3(hduDataMap["Tens"].ndcXposition, 0.8f, 0.0f)).scale(hduDataMap["Tens"].getScale(widthScale, heightScale));
	hduDataMap["Tens"].model.generateModel("assets/objects/screenPanel.obj");

	// units HDU
	hduDataMap["Units"].simpleShader.createHDUShader("assets/textures/hdu/0.dds");
	hduDataMap["Units"].scaleMulti = 0.5f;
	hduDataMap["Units"].transform.translate(glm::vec3(hduDataMap["Units"].ndcXposition, 0.8f, 0.0f)).scale(hduDataMap["Units"].getScale(widthScale,heightScale));
	hduDataMap["Units"].model.generateModel("assets/objects/screenPanel.obj");

	// lifes HDU
	for (int i = 1; i <= maxLifeNr; i++) {
		string keyName = "Life" + std::to_string(i);
		hduDataMap[keyName].simpleShader.createHDUShader("assets/textures/hdu/scarab2.dds");
		//std::string texPath = "assets/textures/hdu/" + std::to_string(i) + ".dds";
		//hduDataMap[keyName].simpleShader.createHDUShader(texPath);

		hduDataMap[keyName].scaleMulti = 0.4f;
		hduDataMap[keyName].transform.translate(glm::vec3(hduDataMap[keyName].ndcXposition, -0.85f, 0.0f)).scale(hduDataMap[keyName].getScale(widthScale, heightScale));
		hduDataMap[keyName].model.generateModel("assets/objects/screenPanel.obj");
	}
}

void Hdu::calculateScale() {
	float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	float targetAspectRatio = 1.0f;
	float displayHeight = screenHeight * 0.5f;
	float displayWidth = displayHeight;

	if (aspectRatio > targetAspectRatio) {
		displayWidth = displayHeight * aspectRatio;
	}
	else {
		displayHeight = displayWidth / aspectRatio;
	}
	if (displayWidth > displayHeight) {
		displayWidth = displayHeight;
	}
	else {
		displayHeight = displayWidth;
	}

	widthScale = displayWidth / screenWidth;
	heightScale = displayHeight / screenHeight;
}

void Hdu::calculatePosition() {

	int objectWidth = screenWidth * widthScale*0.1; // Beispielbreite des Objekts

	// Normalized Device Coordinates (NDC) im Bereich [-1, 1]
	hduDataMap["Units"].ndcXposition = (2.0f * (screenWidth - objectWidth - 0)) / screenWidth - 1.0f;
	hduDataMap["Tens"].ndcXposition  = (2.0f * (screenWidth - objectWidth * 2.05)) / screenWidth - 1.0f;
	hduDataMap["Cacti"].ndcXposition = (2.0f * (screenWidth - objectWidth * 5.1)) / screenWidth - 1.0f;

	for (int i = 1; i <= maxLifeNr; i++) {
		string keyName = "Life" + std::to_string(i);
		hduDataMap[keyName].ndcXposition = (2.0f * (objectWidth * (i == 1 ? 1.2 : 1.2 + 1.5 * (i - 1)))) / screenWidth - 1.0f;
		std::cout << "\n xposition: " << hduDataMap[keyName].ndcXposition << "/n";
	}
}

void Hdu::drawHDU(GLFWwindow* window)
{
	int newWidth, newHeight;
	glfwGetWindowSize(window, &newWidth, &newHeight);
	glm::mat4 projMatrix = hduCamera.getProjectionMatrixHDU();

	if (newWidth != screenWidth || newHeight != screenHeight) {
		screenWidth = newWidth;
		screenHeight = newHeight;

		calculateScale();		// set new scale
		calculatePosition();

		for (const auto& element : hduDataMap) {
			//element.second.transform = emptyTransform;	// not working
			const std::string& keyName = element.first;
			hduDataMap[keyName].transform = emptyTransform;

			// wenn life enthält => y Position anders
			if (keyName.find("Life") != std::string::npos) {
				hduDataMap[keyName].transform.translate(glm::vec3(element.second.ndcXposition, -0.85f, 0.0f)).scale(hduDataMap[keyName].getScale(widthScale, heightScale));
			}
			else {	
				hduDataMap[keyName].transform.translate(glm::vec3(element.second.ndcXposition, 0.8f, 0.0f)).scale(hduDataMap[keyName].getScale(widthScale, heightScale));
			}
		}
	}

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (const auto& element : hduDataMap) {
		const std::string& name = element.first;

		hduDataMap[name].simpleShader.activate();
		hduDataMap[name].simpleShader.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, element.second.transform.getMatrix());
		hduDataMap[name].simpleShader.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, projMatrix);
		hduDataMap[name].simpleShader.setUniform3f("eyePos", hduCamera.cameraPosition.x, hduCamera.cameraPosition.y, hduCamera.cameraPosition.z);
		hduDataMap[name].model.draw(&hduDataMap[name].simpleShader);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Hdu::updateSpikeCount(int newNumber) {

	int tennerplace = newNumber / 10;
	int unitplace = newNumber % 10;

	hduDataMap["Tens"].simpleShader.activate();
	std::string newTexturePath = "assets/textures/hdu/" + std::to_string(tennerplace) + ".dds";
	hduDataMap["Tens"].simpleShader.changeTexture(newTexturePath);

	hduDataMap["Units"].simpleShader.activate();
	newTexturePath = "assets/textures/hdu/" + std::to_string(unitplace) + ".dds";
	hduDataMap["Units"].simpleShader.changeTexture(newTexturePath);

}

void Hdu::updateLifeCount(int newLifeNumber) {

	if (newLifeNumber == 0) {
		showBigScreen("loseEndscreen");
	}
	else if (newLifeNumber > currentLifeNr) {			// life won (TODO: OPTIONAL)
		float i = 1;
		for (const auto& element : hduDataMap) {
			const std::string& keyName = element.first;
			if (keyName.find("Life") != std::string::npos && i == newLifeNumber) {
				hduDataMap[keyName].simpleShader.changeTexture("assets/textures/hdu/scarab2.dds");
				i++;
				std::cout << "Life won!";
			}
		}
	}
	else if (newLifeNumber < currentLifeNr) {		// life lost
		float i = 1;
		for (const auto& element : hduDataMap) {
			const std::string& keyName = element.first;
			std::string makeGreyKeyName = "Life" + std::to_string(newLifeNumber+1);
			if (keyName.find(makeGreyKeyName) != std::string::npos) {
				hduDataMap[keyName].simpleShader.changeTexture("assets/textures/hdu/scarab-grey-transparent.dds");
				std::cout << "Life lost!";
				break;
				i++;
			}
		}
	}
	newLifeNumber = currentLifeNr;

	std::cout << "\nchanging life count!\n";

}

void Hdu::showBigScreen(string screenKey) {
	// screenKey: loseEndscreen ODER winEndscreen

	hduDataMap.clear();
	hduDataMap[screenKey].simpleShader.createHDUShader("assets/textures/hdu/" + screenKey + ".dds");

	// Endbild erstreckt sich über ganzen Screen:
	hduDataMap[screenKey].transform.translate(glm::vec3(0.0f, 0.0f, 0.0f)).scale(glm::vec3(getFullscreenHDUScale() * 2.0f));

	// Alternative: schön rechteckig, aber bei Fullscreen immer noch Spiel auf der Seite sichtbar:
	//hduDataMap["GameOver"].scaleMulti = 4.0f;
	//hduDataMap["GameOver"].transform.translate(glm::vec3(0.0f, 0.0f, 0.0f)).scale(hduDataMap["GameOver"].getScale(widthScale, heightScale));

	hduDataMap[screenKey].model.generateModel("assets/objects/screenPanel.obj");
	std::cout << "\nMuhahah you lost! >:( \n";
}

float Hdu::getFullscreenHDUScale() {
	float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
	float targetAspectRatio = aspectRatio;
	float displayWidth = screenWidth;
	float displayHeight = screenHeight;
	if (aspectRatio > targetAspectRatio) {
		displayWidth = displayHeight * targetAspectRatio;
	}
	else {
		displayHeight = displayWidth / targetAspectRatio;
	}
	float scaleWidth = static_cast<float>(screenWidth) / displayWidth;
	float scaleHeight = static_cast<float>(screenHeight) / displayHeight;
	float scale = std::max(scaleWidth, scaleHeight);
	return scale;
}
