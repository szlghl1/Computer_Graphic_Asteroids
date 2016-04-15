#include "Common.h"
#include "Ship.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"

#include <vector>
#include <cmath>
#include <iostream>

bool Ship::OnInitialize()
{	
    auto& mesh = Create<Mesh>("ship-mesh");
    
    vertices =
    {
        0,0.5f,0.5
		,
		0.5f, -0.8f, 0
		,
		0,-0.5f,0.5
		,
        -0.5f, -0.8f, 0
    };
    
    indices = {0,1,2,2,3,0};
   
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
	auto& translation = this->Transform.Translation;
	translation.X += dX.X;
	translation.Y += dX.Y;
	translation.Z += dX.Z;

	this->Transform.Rotation.Z = angleRadian;

	checkBound();
}


void Ship::OnRender(const GameTime& time)
{
    auto& cam = Game::Camera;
	m_material->Bind();
	explodingTiming(time);
	m_material->SetUniform("GameTimeTotalSeconds", time.TotalSeconds());
	m_material->SetUniform("World", Transform.GetMatrix());
    m_material->SetUniform("View",cam.GetViewMatrix());
    m_material->SetUniform("Projection",cam.GetProjectionMatrix());
	m_material->SetUniform("objectColor", color);
}

void Ship::ProcessInput(const GameTime& time)
{
	if(exploding)
	{
		return;
	}

	angleVelocity = atan(velocity.X/velocity.Y);

    auto* window = Game::Instance().Window();
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
		if (angleRadian <= 2 * pi)
		{
			angleRadian += 0.01f;
		}
		else
		{
			angleRadian = 0;
		}
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
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
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
		velocity.X = 0;
		velocity.Y = 0;
		acceleration.X = 0;
		acceleration.Y = 0;
    }
}

void Ship::explode(const GameTime & t)
{
	acceleration = Vector4(0, 0, 0, 0);
	velocity = Vector4(0, 0, 0, 0);
	exploding = true;
	beginningExplodingTime = t.TotalSeconds();
	Log::Info << "Ship exploded." << std::endl;
}

void Ship::reborn()
{
	angleRadian = 0;
	velocity = Vector4(0, 0, 0, 1);
	acceleration = Vector4(0, 0, 0, 1);
	Transform.Translation.X = 0; 
	Transform.Translation.Y = 0;
}

void Ship::explodingTiming(const GameTime & t)
{
	static bool flagForSendBeginningTime = true;
	if (exploding)
	{
		if (t.TotalSeconds() - beginningExplodingTime > shipExplodingPeriod)
		{
			exploding = false;
			m_material->SetUniform("Exploding", 0);
			reborn();
			flagForSendBeginningTime = true;
		}
		else
		{
			if (flagForSendBeginningTime == true)
			{
				m_material->SetUniform("TimeBeginToExplode", t.TotalSeconds());
				flagForSendBeginningTime = false;
			}
			m_material->SetUniform("Exploding", 1);
		}
	}
}

