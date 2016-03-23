#ifndef WORLDENTITY_H
#define WORLDENTITY_H

#include "GameObject.h"
#include "Transform.h"

class WorldEntity : public GameObject
{
public:
    Transform Transform;
	void setBound(float left, float right, float up, float bottom);
protected:
	float upBound, leftBound, rightBound, bottomBound;
	void checkBound();
};

#endif /* WorldEntity_hpp */
