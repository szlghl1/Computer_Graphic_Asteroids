#ifndef WORLDENTITY_H
#define WORLDENTITY_H

#include "GameObject.h"
#include "Transform.h"

class WorldEntity : public GameObject
{
public:
    Transform Transform;
	void setBound(float left, float right, float up, float bottom);
	void setBound(Vector4 bound);
protected:
	float upBound, leftBound, rightBound, bottomBound;
	virtual void checkBound();
	bool exploding;
	float beginningExplodingTime = 0;
	virtual void explode(const GameTime& t) { ; }
	virtual void explodingTiming(const GameTime& t) { ; }
};

#endif /* WorldEntity_hpp */
