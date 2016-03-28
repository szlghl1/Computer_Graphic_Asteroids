#include "Common.h"
#include "Bullet.h"
#include "Mesh.h"
#include "Game.h"
#include "Camera.h"

#include <vector>
#include <cmath>

bool Bullet::OnInitialize()
{
	auto& mesh = Create<Mesh>(Name + "-mesh");

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

	mesh.Initialize(vertices, indices);

	m_mesh = &mesh;

	//auto& material = Create<class Material>("Asteroid-material");
	auto& material = Create<class Material>(Name + "-material");

	m_material = &material;

	mesh.Material = &material;
	material.FillType = PolygonMode::Line;

	return material.Build("Shaders/primitive");

}

void Bullet::OnUpdate(const GameTime& time)
{
	Vector4 dX = velocity * time.ElapsedSeconds();
	auto& translation = Transform.Translation;
	translation.X += dX.X;
	translation.Y += dX.Y;
	translation.Z += dX.Z;
	checkBound();
}


void Bullet::OnRender(const GameTime& time)
{
	auto& cam = Game::Camera;
	m_material->Bind();
	m_material->SetUniform("World", Transform.GetMatrix());
	m_material->SetUniform("View", cam.GetViewMatrix());
	m_material->SetUniform("Projection", cam.GetProjectionMatrix());
	m_material->SetUniform("objectColor", color);
}

void Bullet::checkBound()
{
	auto &trans = Transform.Translation;

	if (trans.X < leftBound)
	{
		Transform.Translation = Vector3(0, 0, 10.f);
		velocity = Vector4(0, 0, 0, 0);
	}
	else if (trans.X > rightBound)
	{
		Transform.Translation = Vector3(0, 0, 10.f);
		velocity = Vector4(0, 0, 0, 0);
	}
	else if (trans.Y > upBound)
	{
		Transform.Translation = Vector3(0, 0, 10.f);
		velocity = Vector4(0, 0, 0, 0);
	}
	else if (trans.Y < bottomBound)
	{
		Transform.Translation = Vector3(0, 0, 10.f);
		velocity = Vector4(0, 0, 0, 0);
	}
}