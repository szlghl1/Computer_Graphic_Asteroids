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
#include "Bullet.h"

const int numberOfBullet = 20;
const int numberOfAsteroid = 3;

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
	void CreateNBullet(int n);
	Bullet& showBullet(Vector3 coor, float radiusAngle);
	void CreateNAsteroid(int n);

	void ProcessInput(const GameTime& time) override;
	void OnUpdate(const GameTime& time) override;
    
private:
	Ship *shipInstance;
	vector<Asteroid*> asteroidList; 
	vector<Bullet*> bulletList;

	static void window_size_callback(GLFWwindow* window, int width, int height);
	void updateBound();
	Vector4 deriveBound(int z);
	void shipShot();
	void collisionDetect();
};
#endif /* SimpleGame_hpp */
