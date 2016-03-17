//
//  SimpleGame.cpp
//  GameEngine
//
//  Created by David Lively on 2/3/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

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
	CreateAsteroid();

    auto& cam = Game::Camera;
    
    cam.Transform.Translation.Z = 1;//20

	glfwSetWindowSizeCallback(m_window, &(this->window_size_callback));

    return true;
    
}

Ship& AsteroidsGame::CreateShip()
{
	auto &ship = Create<Ship>("ship");
	shipInstance = &ship;
    return *shipInstance;
}
Asteroid& AsteroidsGame::CreateAsteroid()
{
	auto &asteroid = Create<Asteroid>("asteroid");
	asteroidInstance = &asteroid;
	return *asteroidInstance;
}

void AsteroidsGame::ProcessInput()
{
	shipInstance->ProcessInput();
}

void AsteroidsGame::OnUpdate(const GameTime& time)
{
	/********************************************************
	//
	//     A--------------------------------B
	//     |                                |
	//     |                                |
	//     |             Frustum            |
	//     |                                |
	//     |                                |
	//	   C--------------------------------D
	//
	********************************************************/
	float halfH = Camera.ZNear * tan(Camera.FieldOfView / 2);
	float halfW = halfH * Camera.Aspect;
	Vector3 a = Vector3(-halfW, halfH, Camera.ZNear);
	Vector3 b = Vector3(halfW, halfH, Camera.ZNear);
	Vector3 c = Vector3(-halfW, -halfH, Camera.ZNear);
	Vector3 d = Vector3(halfW, -halfH, Camera.ZNear);

	//get the normal vector by cross product
	//(a2b3-a3b2),(a3b1-a1b3),(a1b2-a2b1)
	Vector3 nab = Vector3(a.Y*b.Z - a.Z*b.Y, a.Z*b.X - a.X*b.Z, a.X*b.Y - a.Y*b.X);
	Vector3 ncd = Vector3(c.Y*d.Z - c.Z*d.Y, c.Z*d.X - c.X*d.Z, c.X*d.Y - c.Y*d.X);
	Vector3 nac = Vector3(a.Y*c.Z - a.Z*c.Y, a.Z*c.X - a.X*c.Z, a.X*c.Y - a.Y*c.X);
	Vector3 nbd = Vector3(b.Y*d.Z - b.Z*d.Y, b.Z*d.X - b.X*d.Z, b.X*d.Y - b.Y*d.X);
	//normalize them
	//since normalization is seldom used, I did not seal it in a function
	float tempLength = sqrt(nab.X*nab.X + nab.Y*nab.Y + nab.Z*nab.Z);
	nab.X /= tempLength;
	nab.Y /= tempLength;
	nab.Z /= tempLength;
	//tempLength = sqrt(ncd.X*ncd.X + ncd.Y*ncd.Y + ncd.Z*ncd.Z);
	ncd.X /= tempLength;
	ncd.Y /= tempLength;
	ncd.Z /= tempLength;
	//tempLength = sqrt(nac.X*nac.X + nac.Y*nac.Y + nac.Z*nac.Z);
	nac.X /= tempLength;
	nac.Y /= tempLength;
	nac.Z /= tempLength;
	//tempLength = sqrt(nbd.X*nbd.X + nbd.Y*nbd.Y + nbd.Z*nbd.Z);
	nbd.X /= tempLength;
	nbd.Y /= tempLength;
	nbd.Z /= tempLength;
	//dot product of n and ship is the distance

	auto& shipPos = shipInstance->Transform.Translation;
	//if we use abs as length, then if ship may "disapear"
	//it is because the ship may run too quick that the distance in next frame is beyond the condition
	float lengthToAb = nab.X * shipPos.X + nab.Y * shipPos.Y + nab.Z * shipPos.Z;
	float lengthToCd = ncd.X * shipPos.X + ncd.Y * shipPos.Y + ncd.Z * shipPos.Z;
	float lengthToAc = nac.X * shipPos.X + nac.Y * shipPos.Y + nac.Z * shipPos.Z;
	float lengthToBd = nbd.X * shipPos.X + nbd.Y * shipPos.Y + nbd.Z * shipPos.Z;

	if (lengthToAb < 0.1)
	{
		shipPos.Y = -shipPos.Y - 1;
		//Log::Info << "upper bound" << std::endl;
	}
	if (lengthToCd > -0.1)
	{
		shipPos.Y = -shipPos.Y + 1;
		//Log::Info << "bottom bound" << std::endl;
	}
	if (lengthToAc > -0.1)
	{
		shipPos.X = -shipPos.X + 1;
		//Log::Info << "left bound" << std::endl;
	}
	if (lengthToBd < 0.1)
	{
		shipPos.X = -shipPos.X - 1;
		//Log::Info << "right bound" << std::endl;
	}
	//Log::Info << "ab " << lengthToAb << " cd " << lengthToCd
	/*
	if (shipPos.X < leftBound)
	{
		shipPos.X = rightBound;
	}
	if (shipPos.X > rightBound)
	{
		shipPos.X = leftBound;
	}
	if (shipPos.Y > upBound)
	{
		shipPos.Y = bottomBound;
	}
	if (shipPos.Y < bottomBound)
	{
		shipPos.Y = upBound;
	}*/
}

void AsteroidsGame::window_size_callback(GLFWwindow* window, int width, int height)
{
	Log::Info << "Size changed: width = " << width << " height = " << height << std::endl;
}