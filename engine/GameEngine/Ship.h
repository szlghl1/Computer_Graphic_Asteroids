#ifndef SHIP_H
#define SHIP_H

#include "Common.h"
#include "WorldEntity.h"
#include "Mesh.h"
#include "Matrix.h"

const float shipExplodingPeriod = 2;

class Ship : public WorldEntity
{
public:
    
    Ship()
    {
		acceleration = Vector4(0, 0, 0, 0);
		velocity = Vector4(0, 0, 0, 0);
		angleRadian = 0;
		Transform.Translation.Z = 0.f;
    }
    
    bool OnInitialize() override;
    void OnUpdate(const GameTime& time) override;
    void OnRender(const GameTime& time) override;
    void ProcessInput(const GameTime& time);
	void explode(const GameTime& t);
	void reborn();
private:
	const float pi = 3.14159265358979f;
	const float max_speed = 8.0f;
	const float deceleration = 0.02f;

    Mesh* m_mesh;
	Material* m_material;
	Vector4 acceleration;
	Vector4 velocity;
	Vector4 color = Vector4(0.5, 0.5, 0, 0);
	float angleVelocity;
	float angleRadian;
	std::vector<float> vertices;
	std::vector<GLushort> indices;
	float upBound, leftBound, rightBound, bottomBound;
	void explodingTiming(const GameTime & t);
	bool exploding = false;
	float beginningExplodingTime = 0;
};


#endif /* Ship_hpp */
