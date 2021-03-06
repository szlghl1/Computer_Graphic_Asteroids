#include "WorldEntity.h"

void WorldEntity::setBound(float up, float bottom, float left, float right)
{
	upBound = up;
	bottomBound = bottom;
	leftBound = left;
	rightBound = right;
}

void WorldEntity::setBound(Vector4 bound)
{
	upBound = bound.X;
	bottomBound = bound.Y;
	leftBound = bound.Z;
	rightBound = bound.W;
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