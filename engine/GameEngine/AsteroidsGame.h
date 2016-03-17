//
//  SimpleGame.hpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "Game.h"
#include "Ship.h"
#include "Asteroid.h"

class AsteroidsGame : public Game
{
public:
	AsteroidsGame() : ShaderFolder("Shaders/")
    {
    }
    
    bool OnCreateScene() override;
    

    /// location of shaders in the file system.
	std::string ShaderFolder;
    
    Ship& CreateShip();
	Asteroid& CreateAsteroid();

	void ProcessInput() override;
	void OnUpdate(const GameTime& time) override;
    
private:
	Ship *shipInstance;
	Asteroid *asteroidInstance;

	const float upBound = 8;
	const float bottomBound = -8;
	const float leftBound = -15;
	const float rightBound = 15;

	static void window_size_callback(GLFWwindow* window, int width, int height);
};


#endif /* SimpleGame_hpp */
