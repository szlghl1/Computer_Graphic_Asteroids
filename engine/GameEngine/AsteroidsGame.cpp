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
	CreateNAsteroid(numberOfAsteroid);
	CreateNBullet(numberOfBullet);

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
	static int nExist = 0;
	for (int i = 0; i < n; ++i)
	{
		auto &tempInstance = Create<Asteroid>("asteroid" + to_string(n + nExist));
		tempInstance.Transform.Translation.X = (rand() % 100) / 10.f;
		tempInstance.Transform.Translation.Y = (rand() % 100) / 20.f;
		tempInstance.velocity = Vector4((rand() % 10)/10.f, (rand() % 10)/10.f, 0.f, 0.f);
		tempInstance.Transform.Rotation.X = (rand() % 600) / 10.f;
		tempInstance.Transform.Rotation.Y = (rand() % 600) / 10.f;
		tempInstance.Transform.Rotation.Z = (rand() % 600) / 10.f;
		asteroidList.push_back(&tempInstance);
		nExist++;
	}
}

void AsteroidsGame::CreateNBullet(int n)
{
	static int nExist = 0;
	for (int i = 0; i < n; ++i)
	{
		auto &tempInstance = Create<Bullet>("bullet" + to_string(n + nExist));
		tempInstance.Transform.Translation.Z = 10.f;//make it invisible at beginning
		bulletList.push_back(&tempInstance);
		nExist++;
	}
}

Bullet& AsteroidsGame::showBullet(Vector3 coor, float radiusAngle)
{
	static int nextIndex = 0;
	Bullet *bullet = bulletList.at(nextIndex % bulletList.size());
	bullet->setBullet(shipInstance->Transform.Translation, shipInstance->Transform.Rotation.Z);
	nextIndex++;
	return *bullet;
}

void AsteroidsGame::ProcessInput(const GameTime& time)
{
	auto* window = Game::Instance().Window();

	shipInstance->ProcessInput(time);

	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		auto& camRotate = Game::Camera.Transform.Rotation;
		if (camRotate.X <= 2 * PI)
		{
			camRotate.X += 0.01f;
		}
		else
		{
			camRotate = 0;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		auto& camRotate = Game::Camera.Transform.Rotation;
		if (camRotate.X >= 0)
		{
			camRotate.X -= 0.01f;
		}
		else
		{
			camRotate = 2*PI;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		static float lastShotTime = 0;
		if (time.TotalSeconds() - lastShotTime >= 0.5)
		{
			shipShot();
			lastShotTime = time.TotalSeconds();
		}
	}
}

void AsteroidsGame::OnUpdate(const GameTime& time)
{
	updateBound();
}

void AsteroidsGame::window_size_callback(GLFWwindow* window, int width, int height)
{
	Log::Info << "Size changed: width = " << width << " height = " << height << std::endl;
	if (height)
	{
		Camera.Aspect = static_cast<float>(width / height);
		gl::Viewport(0, 0, width, height);
	}
}


void AsteroidsGame::updateBound()
{
	//update ship
	int tempZ = static_cast<int>(shipInstance->Transform.Translation.Z);
	shipInstance->setBound(deriveBound(tempZ));

	//update asteroids
	for (std::vector<Asteroid *>::iterator i = asteroidList.begin(); i < asteroidList.end(); ++i)
	{
		tempZ = static_cast<int>((*i)->Transform.Translation.Z);
		(*i)->setBound(deriveBound(tempZ));
	}

	//update bullets
	for (std::vector<Bullet *>::iterator i = bulletList.begin(); i < bulletList.end(); ++i)
	{
		tempZ = static_cast<int>((*i)->Transform.Translation.Z);
		(*i)->setBound(deriveBound(tempZ));
	}
}

Vector4 AsteroidsGame::deriveBound(int z)
{
	//it works if camera has not rotated
	//if camera translated along Z axis, just add camera.z to z
	float upBound = -z*tan(Game::Camera.FieldOfView/2);
	Game::Camera.GetProjectionMatrix();
	float rightBound = upBound * Game::Camera.Aspect;
	return Vector4(upBound,-upBound,-rightBound,rightBound);
}

void AsteroidsGame::shipShot()
{
	showBullet(shipInstance->Transform.Translation, shipInstance->Transform.Rotation.Z);
	Log::Info << "Ship shotted a bullet." << std::endl;
}