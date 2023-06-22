
#include "Music.h"

void playBackgroundMusic(std::string musicKey) {
	std::string path = "";

	if (musicKey == "instructionScreen") {
		path = "assets/music/Dance-in-the-Desert.wav";		// Dance-in-the-Desert.wav
	}
	else if (musicKey == "loseEndscreen") {
		path = "assets/music/kl-music-box-game-over-i-152199.wav";
	}
	else if (musicKey == "winEndscreen") {
		path = "assets/music/Victory.wav";
	}

	backgroundmusic.stop();

	if (!backgroundmusic.openFromFile(path))
	{
		std::cout << "no music file found!";
	}

	backgroundmusic.setLoop(true);
	backgroundmusic.setVolume(80);
	//backgroundmusic.play();

	std::cout << "play music" << std::endl;
}

void playCollectSound() {
	if (!soundEffectCollect.openFromFile("assets/music/collectcoin-6075.wav"))
	{
		std::cout << "no music file found!";
	}
	soundEffectCollect.play();
}

void playShootSound() {
	if (!soundEffectShoot.openFromFile("assets/music/mixkit-fast-blow-2144.wav"))
	{
		std::cout << "no music file found!";
	}
	soundEffectShoot.play();
}

