#include <thread>
#include <iostream>
#include <SFML/Audio.hpp>
#include "Sound.h"

void GameSound::playFire()
{
	auto wav = path + "fire.wav";
	std::thread t([=] {_play(wav);  });
	t.detach();
}

void GameSound::playExploding()
{
	auto wav = path + "exploding.wav";
	std::thread t([=] {_play(wav); });
	t.detach();
}

GameSound::GameSound(std::string p)
{
	path = p;
}

void GameSound::_play(std::string wavFile)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(wavFile.c_str()))
		return;

	sf::Sound sound(buffer);
	sound.play();
	while (sound.getStatus() == sf::Sound::Playing)
		std::this_thread::sleep_for(std::chrono::seconds(1));
}

GameSound sound("sound/");