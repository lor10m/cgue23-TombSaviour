#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

extern sf::Music backgroundmusic;
extern sf::Music soundEffectCollect;
extern sf::Music soundEffectShoot;

void playBackgroundMusic(std::string musicKey);

void playCollectSound();
void playShootSound();
