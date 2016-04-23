#pragma once
#ifndef _gamesound_
#define _gamesound_
#include <string>
#include <SFML/Audio.hpp>

class GameSound
{
public:
	void playFire();
	void playExploding();
	GameSound(std::string p);
private:
	std::string path;
	void _play(std::string wavFile);
};

extern GameSound sound;
#endif // !_gamesound_
