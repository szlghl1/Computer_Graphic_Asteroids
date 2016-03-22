#include "Asteroid.h"
//
//  Ship.cpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright ? 2016 David Lively. All rights reserved.
//

#include "Common.h"
#include "Asteroid.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

bool Asteroid::OnInitialize()
{
	auto& mesh = Create<Mesh>("Asteroid-mesh");

	float X = 0.525731112119133606f;
	float Z = 0.850650808352039932f;

	vertices =
	{
		-X, 0.f, Z,
		X, 0.f, Z,
		-X, 0.f, -Z,
		X, 0.f, -Z,
		0.f, Z, X,
		0.f, Z, -X,
		0.f, -Z, X,
		0.f, -Z, -X,
		Z, X, 0.f,
		-Z, X, 0.f,
		Z, -X, 0.f,
		-Z, -X, 0.f
	};
	indices =
	{
		0, 4, 1,
		0, 9, 4,
		9, 5, 4,
		4, 5, 8,
		4, 8, 1,
		8, 10, 1,
		8, 3, 10,
		5, 3, 8,
		5, 2, 3,
		2, 7, 3,
		7, 10, 3,
		7, 6, 10,
		7, 11, 6,
		11, 0, 6,
		0, 1, 6,
		6, 1, 10,
		9, 0, 11,
		9, 11, 2,
		9, 2, 5,
		7, 2, 11
	};

	/*
	vertices =
	{
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,0.5f,

	};

	indices = 
	{ 
		0,1,2,2,3,0,
		4,5,6,6,7,4,
		1,5,6,6,2,1,
		0,4,7,7,3,0,
		0,1,5,5,4,0,
		3,2,6,6,7,3
	};
	*/
	center = Vector4(0, 0, 0, 0);

	mesh.Initialize(vertices, indices);

	m_mesh = &mesh;

	auto& material = Create<class Material>("Asteroid-material");
	m_material = &material;

	mesh.Material = &material;
	material.FillType = PolygonMode::Line;

	return material.Build("Shaders/primitive");

}

void Asteroid::OnUpdate(const GameTime& time)
{
	Vector4 dX = velocity * time.ElapsedSeconds();
	center += dX;
	auto& translation = Transform.Translation;
	translation.X += dX.X;
	translation.Y += dX.Y;
	translation.Z += dX.Z;

	//Transform.Rotation.X += rotateX;
	//Transform.Rotation.Y += rotateY;
	//Transform.Rotation.Z += rotateZ;

	//wrapping figure is implemented in AsteroidsGame::OnUpdate()

}


void Asteroid::OnRender(const GameTime& time)
{
	auto& cam = Game::Camera;
	m_material->Bind();
	m_material->SetUniform("World", Transform.GetMatrix());
	m_material->SetUniform("View", cam.GetViewMatrix());
	m_material->SetUniform("Projection", cam.GetProjectionMatrix());
}
