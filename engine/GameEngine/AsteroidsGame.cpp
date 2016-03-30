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

bool AsteroidsGame::OnCreateScene()
{
    CreateShip();
	CreateNAsteroid(numberOfAsteroid);
	CreateNBullet(numberOfBullet);

	showNAsteroid(5);

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
	asteroidActiveList.push_back(&asteroid);
	return asteroid;
}

void AsteroidsGame::CreateNAsteroid(int n)
{
	static int nExist = 0;
	for (int i = 0; i < n; ++i)
	{
		auto &tempInstance = Create<Asteroid>("asteroid" + to_string(n + nExist));
		tempInstance.hide();
		asteroidInActiveList.push_back(&tempInstance);
		nExist++;
	}
}

void AsteroidsGame::showNAsteroid(int n)
{
	for (int i = 0; i < n; ++i)
	{
		if (asteroidInActiveList.size() == 0)
		{
			Log::Error << "Inactive asteroids not enough to show." << std::endl;
			return;
		}

		auto tempInstance = asteroidInActiveList.at(0);

		//ensure asteroids will not appear at the same spot as ship
		do
		{
			tempInstance->Transform.Translation.X = (rand() % 100) / 10.f;
			tempInstance->Transform.Translation.Y = (rand() % 100) / 20.f;
			tempInstance->Transform.Translation.Z = -20.f;
		} while (checkCollision(*shipInstance, *tempInstance));

		tempInstance->velocity = Vector4((rand() % 10) / 10.f, (rand() % 10) / 10.f, 0.f, 0.f);
		tempInstance->Transform.Rotation.X = (rand() % 600) / 10.f;
		tempInstance->Transform.Rotation.Y = (rand() % 600) / 10.f;
		tempInstance->Transform.Rotation.Z = (rand() % 600) / 10.f;

		tempInstance->invisible = 0;
		asteroidActiveList.push_back(tempInstance);
		asteroidInActiveList.erase(asteroidInActiveList.begin());
	}
}

void AsteroidsGame::CreateNBullet(int n)
{
	static int nExist = 0;
	for (int i = 0; i < n; ++i)
	{
		auto &tempInstance = Create<Bullet>("bullet" + to_string(n + nExist));
		tempInstance.hide();//make it invisible at beginning
		bulletList.push_back(&tempInstance);
		nExist++;
	}
}

Bullet& AsteroidsGame::showBullet(Vector3 coor, float radiusAngle)
{
	static int nextIndex = 0;
	Bullet *bullet = bulletList.at(nextIndex % bulletList.size());
	bullet->setBullet(shipInstance->Transform.Translation, shipInstance->Transform.Rotation.Z);
	bullet->invisible = 0;
	nextIndex++;
	return *bullet;
	/*
	Bullet *bullet = nullptr;
	if (bulletInActiveList.size() != 0)
	{
		bullet = bulletInActiveList.at(0);
	}
	else
	{
		Log::Error << "Inactive bullet is not enough to shot." << std::endl;
	}
	bullet->invisible = 0;
	bullet->setBullet(shipInstance->Transform.Translation, shipInstance->Transform.Rotation.Z);
	bulletActiveList.push_back(bullet);
	return *bullet;
	*/
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
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
	{
		static float lastAddTime = 0;
		if (time.TotalSeconds() - lastAddTime >= 0.5)
		{
			showNAsteroid(1);
			lastAddTime = time.TotalSeconds();
		}
	}
}

void AsteroidsGame::OnUpdate(const GameTime& time)
{
	updateBound();
	collisionDetect(); 
	auto title = "Time: " + to_string(time.TotalSeconds()) + " Score: " + to_string(score) + " Life: " + to_string(life);
	glfwSetWindowTitle(m_window, title.c_str());
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
	for (std::vector<Asteroid *>::iterator i = asteroidActiveList.begin(); i < asteroidActiveList.end(); ++i)
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

void AsteroidsGame::collisionDetect()
{
	for (vector<Asteroid*>::size_type i = 0; i < asteroidActiveList.size(); ++i)
	{
		auto& asteroidLocate = asteroidActiveList.at(i)->Transform.Translation;
		
		//checking collision between asteroids and ship
		if(checkCollision(*shipInstance,*asteroidActiveList.at(i)))
		{
			life -= 1;

			shipInstance->reborn();
			Log::Info << "Ship resurrected." << std::endl;

			asteroidActiveList.at(i)->explode();
			asteroidInActiveList.push_back(asteroidActiveList.at(i));
			asteroidActiveList.erase(asteroidActiveList.begin() + i);
			break;
		}


		//checking collision between asteroids and bullets
		for (vector<Bullet*>::size_type j = 0; j < bulletList.size(); ++j)
		{
			if (checkCollision(*asteroidActiveList.at(i), (*bulletList.at(j))))
			{
				score += 10;

				asteroidActiveList.at(i)->explode();
				asteroidInActiveList.push_back(asteroidActiveList.at(i));
				asteroidActiveList.erase(asteroidActiveList.begin() + i);

				bulletList.at(j)->hide();
				break;
			}
		}
	}
}

bool AsteroidsGame::checkCollision(const Ship& tempShip, const Asteroid& tempAsteroid)
{
	auto& shipLocate = tempShip.Transform.Translation;
	auto& asteroidLocate = tempAsteroid.Transform.Translation;
	float distanceSquare = pow(shipLocate.X - asteroidLocate.X, 2) + pow(shipLocate.Y - asteroidLocate.Y, 2) + pow(shipLocate.Z - asteroidLocate.Z, 2);
	if (distanceSquare < tempAsteroid.getRadius())
	{
		Log::Info << "Collision between ship and asteroid detected." << std::endl;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool AsteroidsGame::checkCollision(const Asteroid& tempAsteroid, const Bullet& tempbullet)
{
	auto& bulletLocate = tempbullet.Transform.Translation;
	auto& asteroidLocate = tempAsteroid.Transform.Translation;
	float distanceSquare = pow(bulletLocate.X - asteroidLocate.X, 2) + pow(bulletLocate.Y - asteroidLocate.Y, 2) + pow(bulletLocate.Z - asteroidLocate.Z, 2);
	if (distanceSquare < tempAsteroid.getRadius())
	{
		Log::Info << "Collision between bullet and asteroid detected." << std::endl;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}