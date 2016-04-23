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
#include <random>
#include <ctime>

#include "AsteroidsGame.h"
#include "Mesh.h"
#include "Material.h"
#include "Files.h"
#include "Sound.h"
#include <future>

using namespace std;

bool AsteroidsGame::OnCreateScene()
{
	auto& cam = Game::Camera;
	cam.Transform.Translation.Z = 20;

    CreateShip();
	CreateNAsteroid(numberOfAsteroid);
	CreateNBullet(numberOfBullet);

	levelUp();

	glfwSetWindowSizeCallback(m_window, window_size_callback);

    return true;
}

Ship& AsteroidsGame::CreateShip()
{
	auto &ship = Create<Ship>("ship");
	shipInstance = &ship;
    return *shipInstance;
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
	auto bound = deriveBound(0);
	static std::default_random_engine dre(static_cast<int>(time(0)));
	static std::uniform_real_distribution<float> urd4H(0, bound.X);
	static std::uniform_real_distribution<float> urd4W(0, bound.W);
	static std::uniform_real_distribution<float> urd4V(0, 2);

	for (int i = 0; i < n; ++i)
	{
		if (asteroidInActiveList.size() == 0)
		{
			Log::Error << "Inactive asteroids not enough to show." << std::endl;
			return;
		}

		auto& tempInstance = asteroidInActiveList.at(0);

		//ensure asteroids will not appear near ship
		auto& tempTrans = tempInstance->Transform.Translation;
		auto& shipTrans = shipInstance->Transform.Translation;
		//float dSquare;
		bool temp;
		do
		{
			tempTrans.X = urd4W(dre);
			tempTrans.Y = urd4H(dre) / 2;
			tempTrans.Z = 0.f;
			temp = checkCollision(*tempInstance, *shipInstance);
		} while (temp);

		tempInstance->velocity = Vector4(urd4V(dre), urd4V(dre), 0.f, 0.f);
		tempInstance->Transform.Rotation.X = urd4W(dre);
		tempInstance->Transform.Rotation.Y = urd4W(dre);
		tempInstance->Transform.Rotation.Z = urd4W(dre);

		tempInstance->invisible = 0;
		asteroidActiveList.push_back(tempInstance);
		asteroidInActiveList.erase(asteroidInActiveList.begin());
		updateBound();
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
}

void AsteroidsGame::ProcessInput(const GameTime& time)
{
	auto* window = Game::Instance().Window();

	if (status == GameStatus::running)
	{
		shipInstance->ProcessInput(time);
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

	static bool lastStatusForEnter = false;
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS)
	{
		lastStatusForEnter = true;
	}
	else
	{
		if (lastStatusForEnter == true)//means key was pressed down and released
		{
			if (status == GameStatus::pause)
			{
				status = GameStatus::running;
			}
			else if (status == GameStatus::running)
			{
				status = GameStatus::pause;
			}
			else if (status == GameStatus::over)
			{
				reset();
			}
			lastStatusForEnter = false;
		}
	}
	
}

void AsteroidsGame::OnUpdate(const GameTime& time)
{
	updateBound();
	collisionDetect(time); 
	auto title = "Time: " + to_string(time.TotalSeconds()) + " Score: " + to_string(score);
	title += " Life: " + to_string(life) + " Level: " + to_string(level);
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
		if ((*i)->invisible)
		{
			continue;
		}
		tempZ = static_cast<int>((*i)->Transform.Translation.Z);
		(*i)->setBound(deriveBound(tempZ));
	}
}

Vector4 AsteroidsGame::deriveBound(int z)
{
	//it works if camera has not rotated
	//if camera translated along Z axis, just add camera.z to z
	if (z != 0)
		DEBUG_BREAK;
	z -= static_cast<int>(Camera.Transform.Translation.Z);
	float upBound = -z*tan(Game::Camera.FieldOfView/2);
	Game::Camera.GetProjectionMatrix();
	float rightBound = upBound * Game::Camera.Aspect;
	return Vector4(upBound,-upBound,-rightBound,rightBound);
}

void AsteroidsGame::shipShot()
{
	sound.playFire();
	showBullet(shipInstance->Transform.Translation, shipInstance->Transform.Rotation.Z);
	Log::Info << "Ship shotted a bullet." << std::endl;
}

void AsteroidsGame::collisionDetect(const GameTime t)
{
	//checking collision between asteroids and ship
	while (1)
	{
		if (shipInstance->exploding)
		{
			break;
		}
		else
		{
			vector<Asteroid*>::iterator i = find_if(asteroidActiveList.begin(), asteroidActiveList.end(),
				[this](Asteroid* a) {return this->checkCollision(*a, *(this->shipInstance)); });
			if (i == asteroidActiveList.end())
			{
				break;
			}

			auto pAsteroid = *i;
			asteroidActiveList.erase(i);
			asteroidInActiveList.push_back(pAsteroid);
			pAsteroid->explode(t);
			sound.playExploding();
			if (life - 1 >= 0)
			{
				life--;
				shipInstance->explode(t);
				Log::Info << "Ship resurrected." << std::endl;
			}
			else
			{
				shipInstance->explode(t);
				status = GameStatus::over;
				Log::Error << "Game over." << std::endl;
				return;
			}
		}
	}

	//checking collision between asteroids and bullets
	for (auto i = bulletList.begin(); i != bulletList.end(); i++)
	{
		vector<Asteroid*>::iterator j = find_if(asteroidActiveList.begin(), asteroidActiveList.end(),
			[this,i](Asteroid* a) {return this->checkCollision(*a, **i); });
		if (j == asteroidActiveList.end())
		{
			continue;
		}

		sound.playExploding();

		auto pBullet = *i;
		auto pAsteroid = *j;
		pBullet->hide();
		asteroidActiveList.erase(j);
		asteroidInActiveList.push_back(pAsteroid);
		pAsteroid->explode(t);
	}

	if (asteroidActiveList.empty())
	{
		levelUp();
	}
}

void AsteroidsGame::reset()
{
	level = 0;
	for (auto i = asteroidActiveList.begin(); i != asteroidActiveList.end(); i++)
	{
		(*i)->hide();
		asteroidInActiveList.push_back(*i);
	}

	asteroidActiveList.erase(asteroidActiveList.begin(),asteroidActiveList.end());

	shipInstance->reborn();
	score = 0;
	life = initialLife;
	levelUp();
	status = GameStatus::running;
}

void AsteroidsGame::levelUp()
{
	level++;
	switch (level)
	{
	case 1:showNAsteroid(4); break;
	case 2:showNAsteroid(4); break;
	case 3:showNAsteroid(6); break;
	case 4:showNAsteroid(6); break;
	case 5:showNAsteroid(8); break;
	case 6:showNAsteroid(8); break;
	case 7:showNAsteroid(10); break;
	case 8:showNAsteroid(10); break;
	default:
		Log::Error << "You are doing so good that you are beyond the design of game" << std::endl;
		break;
	}
}

bool AsteroidsGame::checkCollision(const Asteroid& tempAsteroid, const WorldEntity& e) 
{
	auto& tempTLocate = e.Transform.Translation;
	auto& asteroidLocate = tempAsteroid.Transform.Translation;
	float distanceSquare = pow(tempTLocate.X - asteroidLocate.X, 2) + pow(tempTLocate.Y - asteroidLocate.Y, 2) + pow(tempTLocate.Z - asteroidLocate.Z, 2);
	if (distanceSquare < tempAsteroid.getRadius())
	{
		Log::Info << "Collision detected." << std::endl;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}