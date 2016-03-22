//
//  Asteroid.hpp
//  GameEngine
#ifndef ASTEROID_H
#define ASTEROID_H
#include "Common.h"
#include "WorldEntity.h"
#include "Mesh.h"
#include "Matrix.h"

class Asteroid : public WorldEntity
{
public:

	Asteroid()
	{
		velocity = Vector4(0.5f, 0.8f, 0.f, 0.f);
		center = Vector4(0, 0, 0, 0);
		Transform.Translation.Z = -20;
		rotateX = 0.4f;
		rotateY = 0.7f;
		rotateZ = 0.3f;
		Transform.Rotation.X = rotateX;
		Transform.Rotation.Y = rotateY;
		Transform.Rotation.Z = rotateZ;
	}

	bool OnInitialize() override;

	void OnUpdate(const GameTime& time) override;
	void OnRender(const GameTime& time) override;
private:
	//const float pi = 3.14159265358979f;
	float rotateX, rotateY, rotateZ;

	Mesh* m_mesh;
	Material* m_material;

	Vector4 velocity;
	std::vector<float> vertices;
	std::vector<GLushort> indices;
	Vector4 center;

};

#endif /* Asteroid_hpp */
