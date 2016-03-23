#include "WorldEntity.h"

void WorldEntity::setBound(float left, float right, float up, float bottom)
{
	leftBound = left;
	rightBound = right;
	upBound = up;
	bottom = bottom;
}

void WorldEntity::checkBound()
{
	auto &trans = Transform.Translation;

	if (trans.X < leftBound)
	{
		trans.X = rightBound;
	}
	else if (trans.X > rightBound)
	{
		trans.X = leftBound;
	}

	if (trans.Y > upBound)
	{
		trans.Y = bottomBound;
	}
	else if (trans.Y < bottomBound)
	{
		trans.Y = upBound;
	}
}