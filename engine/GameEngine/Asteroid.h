//
//  Asteroid.hpp
//  GameEngine
#ifndef ASTEROID_H
#define ASTEROID_H
#include "Common.h"
#include "WorldEntity.h"
#include "Mesh.h"
#include "Matrix.h"

const float asteroidExplodingPeriod = 3;

class Asteroid : public WorldEntity
{
public:

	Asteroid()
	{
		velocity = Vector4(0.5f, 0.8f, 0.f, 0.f);
		Transform.Translation.Z = -20;
		float rotateX = 0.4f;
		float rotateY = 0.7f;
		float rotateZ = 0.3f;
		Transform.Rotation.X = rotateX;
		Transform.Rotation.Y = rotateY;
		Transform.Rotation.Z = rotateZ;
	}

	Vector4 velocity;

	inline void hide() { Transform.Translation = Vector3(0, 0, 10); invisible = 1; }
	bool OnInitialize() override;
	void OnUpdate(const GameTime& time) override;
	void OnRender(const GameTime& time) override;
	void explode(const GameTime& t);
	//if you have asteroids in different Z, it should be modified
	inline float getRadius() const { return radius; }
private:
	Mesh* m_mesh;
	Material* m_material;

	std::vector<float> vertices;
	std::vector<GLushort> indices;

	Vector4 color = Vector4(0, 0.5, 0.5, 0);

	float radius = 1;
	void explodingTiming(const GameTime& t);
	bool exploding = false;
	float beginningExplodingTime = 0;
	bool flagForSendBeginningTime = true;
};

#endif /* Asteroid_hpp */
