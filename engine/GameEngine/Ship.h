//
//  Ship.hpp
//  GameEngine
//
//  Created by David Lively on 2/22/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef SHIP_H
#define SHIP_H

#include "Common.h"
#include "WorldEntity.h"
#include "Mesh.h"
#include "Matrix.h"


class Ship : public WorldEntity
{
public:
    
    Ship()
    {
//		translation = Matrix::CreateTranslation(0, 0, 0);
		rotation = Matrix::CreateRotation(0, 0, 0);
		acceleration = Vector4(0, 0, 0, 0);
		velocity = Vector4(0, 0, 0, 0);
		center = Vector4(0, 0, 0, 0);
		angleRadian = 0;
    }
    
    bool OnInitialize() override;
    
    void OnUpdate(const GameTime& time) override;
    void OnRender(const GameTime& time) override;
    
    void ProcessInput();
private:
	const float pi = 3.14159265358979f;
	const float max_speed = 8.0f;
	const float upBound = 8;
	const float bottomBound = -8;
	const float leftBound = -15;
	const float rightBound = 15;
	const float deceleration = 0.01f;

    Mesh* m_mesh;
    Material* m_material;

	Matrix rotation;
	//Matrix translation;
	Vector4 acceleration;
	Vector4 velocity;
	float angleVelocity;
	float angleRadian;
	std::vector<float> vertices;
	std::vector<GLushort> indices;
	Vector4 center;
    
};


#endif /* Ship_hpp */
