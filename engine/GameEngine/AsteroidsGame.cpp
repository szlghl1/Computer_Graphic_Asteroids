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
	//in OpenGL the direction of camera is inverse, because OpenGL can only recieve positive ZNear ZFar
	Vector3 a = Vector3(-halfW, halfH, -Camera.ZNear);
	Vector3 b = Vector3(halfW, halfH, -Camera.ZNear);
	Vector3 c = Vector3(-halfW, -halfH, -Camera.ZNear);
	Vector3 d = Vector3(halfW, -halfH, -Camera.ZNear);

	//get the normal vector by cross product
	//(a2b3-a3b2),(a3b1-a1b3),(a1b2-a2b1)
	//note: cross product is not commutative

	// a * b
	Vector3 nab = Vector3(a.Y*b.Z - a.Z*b.Y, a.Z*b.X - a.X*b.Z, a.X*b.Y - a.Y*b.X);
	// d * c
	Vector3 ncd = Vector3(d.Y*c.Z - d.Z*c.Y, d.Z*c.X - d.X*c.Z, d.X*c.Y - d.Y*c.X);
	// c * a
	Vector3 nac = Vector3(c.Y*a.Z - c.Z*a.Y, c.Z*a.X - c.X*a.Z, c.X*a.Y - c.Y*a.X);
	// b * d
	Vector3 nbd = Vector3(b.Y*d.Z - b.Z*d.Y, b.Z*d.X - b.X*d.Z, b.X*d.Y - b.Y*d.X);
	//normalize them
	//since normalization is seldom used, I did not seal it in a function
	float tempLength = sqrt(nab.X*nab.X + nab.Y*nab.Y + nab.Z*nab.Z);
	//all these four normal vectors have identical length
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
	float shipLengthToAb = nab.X * shipPos.X + nab.Y * shipPos.Y + nab.Z * shipPos.Z;
	float shipLengthToCd = ncd.X * shipPos.X + ncd.Y * shipPos.Y + ncd.Z * shipPos.Z;
	float shipLengthToAc = nac.X * shipPos.X + nac.Y * shipPos.Y + nac.Z * shipPos.Z;
	float shipLengthToBd = nbd.X * shipPos.X + nbd.Y * shipPos.Y + nbd.Z * shipPos.Z;

	if (shipLengthToAb < 0.1)
	{
		shipPos.Y = -shipPos.Y + 1;
		//Log::Info << "upper bound" << std::endl;
	}
	if (shipLengthToCd < 0.1)
	{
		shipPos.Y = -shipPos.Y - 1;
		//Log::Info << "bottom bound" << std::endl;
	}
	if (shipLengthToAc < 0.1)
	{
		shipPos.X = -shipPos.X - 1;
		//Log::Info << "left bound" << std::endl;
	}
	if (shipLengthToBd < 0.1)
	{
		shipPos.X = -shipPos.X + 1;
		//Log::Info << "right bound" << std::endl;
	}


	auto& asteroidPos = asteroidInstance->Transform.Translation;
	float asteroidLengthToAb = nab.X * asteroidPos.X + nab.Y * asteroidPos.Y + nab.Z * asteroidPos.Z;
	float asteroidLengthToCd = ncd.X * asteroidPos.X + ncd.Y * asteroidPos.Y + ncd.Z * asteroidPos.Z;
	float asteroidLengthToAc = nac.X * asteroidPos.X + nac.Y * asteroidPos.Y + nac.Z * asteroidPos.Z;
	float asteroidLengthToBd = nbd.X * asteroidPos.X + nbd.Y * asteroidPos.Y + nbd.Z * asteroidPos.Z;

	if (asteroidLengthToAb < 0.1)
	{
		asteroidPos.Y = -asteroidPos.Y - 1;
		//Log::Info << "upper bound" << std::endl;
	}
	if (asteroidLengthToCd < 0.1)
	{
		asteroidPos.Y = -asteroidPos.Y + 1;
		//Log::Info << "bottom bound" << std::endl;
	}
	if (asteroidLengthToAc < 0.1)
	{
		asteroidPos.X = -asteroidPos.X + 1;
		//Log::Info << "left bound" << std::endl;
	}
	if (asteroidLengthToBd < 0.1)
	{
		asteroidPos.X = -asteroidPos.X - 1;
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