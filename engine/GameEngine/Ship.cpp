//
//  Ship.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#include "Common.h"
#include "Ship.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

bool Ship::OnInitialize()
{	
    auto& mesh = Create<Mesh>("ship-mesh");
    
    vertices =
    {
        0,0.5f,0
		,
		0.5f, -0.8f, 0
		,
		0,-0.5f,0
		,
        -0.5f, -0.8f, 0
    };
    
    indices = {0,1,2,2,3,0};

	center = Vector4(0, 0, 0, 0);
   
    mesh.Initialize(vertices, indices);
    
    m_mesh = &mesh;
    
    auto& material = Create<class Material>("ship-material");
    m_material = &material;
    
    mesh.Material = &material;
    material.FillType = PolygonMode::Line;
    
    return material.Build("Shaders/primitive");

}

void Ship::OnUpdate(const GameTime& time)
{
	Vector4 dV = acceleration * time.ElapsedSeconds();
	velocity += dV;
	if (abs(velocity.X) >= max_speed)
	{
		if(velocity.X >= 0)
		{
			velocity.X = max_speed;
		}
		else
		{
			velocity.X = -max_speed;
		}
	}
	if (abs(velocity.Y) >= max_speed)
	{
		if (velocity.Y >= 0)
		{
			velocity.Y = max_speed;
		}
		else
		{
			velocity.Y = -max_speed;
		}
	}

	Vector4 dX = velocity * time.ElapsedSeconds();
	center += dX;
	//translation.m03 += dX.X;
	//translation.m13 += dX.Y;
	//translation.m23 += dX.Z;
	translation.m30 -= dX.X;
	translation.m31 += dX.Y;
	translation.m32 += dX.Z;
	rotation = Matrix::CreateRotationZ(angleRadian);

	if (translation.m30 < leftBound)
	{
		translation.m30 = rightBound;
	}
	if (translation.m30 > rightBound)
	{
		translation.m30 = leftBound;
	}
	if (translation.m31 > upBound)
	{
		translation.m31 = bottomBound;
	}
	if (translation.m31 < bottomBound)
	{
		translation.m31 = upBound;
	}
	
	
	/*
	Vector4 aVec = Vector4(0, 0.5, 0, 1);
	Vector4 bVec = Vector4(1 / 3.f, -0.5f, 0.f,1.0f);
	Vector4 cVec = Vector4(-1.3f, -0.5f, 0.f, 1.0f);

	aVec = rotation * aVec;
	aVec = translation * aVec;
	bVec = rotation * bVec;
	bVec = translation * bVec;
	cVec = rotation * cVec;
	cVec = translation * cVec;
	center = rotation * center;
	center = translation * center;
	
	vertices =
	{aVec.X,aVec.Y,aVec.Z,bVec.X,bVec.Y,bVec.Z,cVec.X,cVec.Y,cVec.Z};
	
	indices = { 0,1,2 };

	m_mesh -> Initialize(vertices, indices);
	*/

}


void Ship::OnRender(const GameTime& time)
{
    
    auto& cam = Game::Camera;
	m_material->Bind();
	m_material->SetUniform("World", Transform.GetMatrix());
    m_material->SetUniform("View",cam.GetViewMatrix());
    m_material->SetUniform("Projection",cam.GetProjectionMatrix());
	m_material->SetUniform("translation", translation);
	m_material->SetUniform("rotation", rotation);
}

void Ship::ProcessInput()
{
	angleVelocity = atan(velocity.X/velocity.Y);

    auto* window = Game::Instance().Window();
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
		if (angleRadian >= 0)
		{
			angleRadian -= 0.01f;
		}
		else
		{
			angleRadian = 2 * pi;
		}
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {

		if (angleRadian <= 2 * pi)
		{
			angleRadian += 0.01f;
		}
		else
		{
			angleRadian = 0.0f;
		}
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
		acceleration.X = static_cast<float>(-4.0f * sin(angleRadian));
		acceleration.Y = static_cast<float>(4.0f * cos(angleRadian));
    }
	else
	{
		float speedSquare = pow(velocity.X, 2) + pow(velocity.Y, 2);
		if (speedSquare > 5)
			velocity *= static_cast<float>(1 - deceleration);
		else if (speedSquare > 3)
			velocity *= static_cast<float>(1 - deceleration * 1.5);
		else if (speedSquare > 1)
			velocity *= static_cast<float>(1 - deceleration * 3);
		else if (speedSquare > 0.2)
			velocity *= static_cast<float>(1 - deceleration * 8);
		else
		{
			velocity.X = 0;
			velocity.Y = 0;
		}
	}
	/*
	else if(abs(velocity.X) + abs(velocity.Y) + abs(velocity.Z) < 0.01)
	{
		velocity.X = 0;
		velocity.Y = 0;
		velocity.Z = 0;
	}
	
	else
	{
		acceleration.X = 0.001 * sin(angleVelocity);
		acceleration.Y = -0.001 * cos(angleVelocity);
	}*/
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
		velocity.X = 0;
		velocity.Y = 0;
		acceleration.X = 0;
		acceleration.Y = 0;
    }
}

