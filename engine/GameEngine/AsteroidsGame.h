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

struct Bound;

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
	Asteroid& CreateOneAsteroid();
	void CreateNAsteroid(int n);

	void ProcessInput() override;
	void OnUpdate(const GameTime& time) override;
    
private:
	Ship *shipInstance;
	vector<Asteroid*> asteroidList; 

	static void window_size_callback(GLFWwindow* window, int width, int height);
	void updateBound();
	Vector4 deriveBound(float z);
};
#endif /* SimpleGame_hpp */
