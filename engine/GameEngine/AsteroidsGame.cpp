//
//  SimpleGame.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//
//  modified almost everything by Ling He :)

#include <vector>
#include <iostream>

using namespace std;

#include "AsteroidsGame.h"
#include "Mesh.h"
#include "Material.h"
#include "Files.h"

#include <cmath>
//#include <string>


bool AsteroidsGame::OnCreateScene()
{
    CreateShip();
	CreateNAsteroid(3);

    auto& cam = Game::Camera;
    
    //cam.Transform.Translation.Z = 1;//20

	glfwSetWindowSizeCallback(m_window, window_size_callback);

    return true;
    
}

Ship& AsteroidsGame::CreateShip()
{
	auto &ship = Create<Ship>("ship");
	shipInstance = &ship;
    return *shipInstance;
}

Asteroid& AsteroidsGame::CreateOneAsteroid()
{
	auto &asteroid = Create<Asteroid>("asteroid");
	asteroidList.push_back(&asteroid);
	return asteroid;
}

void AsteroidsGame::CreateNAsteroid(int n)
{
	for (int i = 0; i < n; ++i)
	{
		auto &tempInstance = Create<Asteroid>("asteroid" + to_string(n));
		tempInstance.Transform.Translation.X = (rand() % 100) / 10.f;
		tempInstance.Transform.Translation.Y = (rand() % 100) / 20.f;
		tempInstance.velocity = Vector4((rand() % 10)/10.f, (rand() % 10)/10.f, 0, 0);
		tempInstance.Transform.Rotation.X = (rand() % 60) / 10.f;
		tempInstance.Transform.Rotation.Y = (rand() % 60) / 10.f;
		tempInstance.Transform.Rotation.Z = (rand() % 60) / 10.f;
		asteroidList.push_back(&tempInstance);
	}
}

void AsteroidsGame::ProcessInput()
{
	shipInstance->ProcessInput();
}

void AsteroidsGame::OnUpdate(const GameTime& time)
{
	updateBound();
}

void AsteroidsGame::window_size_callback(GLFWwindow* window, int width, int height)
{
	Log::Info << "Size changed: width = " << width << " height = " << height << std::endl;
	Camera.Aspect = static_cast<int>(width / height);
	gl::Viewport(0, 0, width, height);
}


void AsteroidsGame::updateBound()
{
	//if bound of at this Z surface has already derived, retrive it from map
	//if not, calculate it
	//Because compare float will cause some problem, please use integer as Z (this game is 2D)
	//Or use statement like x - toCompare < 0.01 to compare

	//update ship
	int tempZ = static_cast<int>(shipInstance->Transform.Translation.Z);
	map<int, Vector4>::iterator i = boundMap.find(tempZ);
	if (i == boundMap.end())
	{
		//calculate it
		boundMap.insert({ tempZ,deriveBound(tempZ) });
	}
	else
	{
		//retrieve it 
		auto& tempBound = boundMap[tempZ];
		shipInstance->setBound(tempBound.X, tempBound.Y, tempBound.Z, tempBound.W);
	}

	//update asteroids
	for (std::vector<Asteroid *>::iterator asteroidIterator = asteroidList.begin()
		;
		asteroidIterator < asteroidList.end()
		;
		++asteroidIterator)
	{
		tempZ = static_cast<int>((*asteroidIterator)->Transform.Translation.Z);
		i = boundMap.find(tempZ);
		if (i == boundMap.end())
		{
			//calculate it
			boundMap.insert({ tempZ,deriveBound(tempZ) });
		}
		else
		{
			//retrieve it 
			auto& tempBound = boundMap[tempZ];
			(*asteroidIterator)->setBound(tempBound.X, tempBound.Y, tempBound.Z, tempBound.W);
		}
	}
}

Vector4 AsteroidsGame::deriveBound(int z)
{
	float upBound = -z*tan(Game::Camera.FieldOfView/2);
	Game::Camera.GetProjectionMatrix();
	float rightBound = upBound * Game::Camera.Aspect;
	return Vector4(upBound,-upBound,-rightBound,rightBound);
}