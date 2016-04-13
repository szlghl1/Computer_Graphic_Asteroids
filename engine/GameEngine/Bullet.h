#ifndef BULLET_H
#define BULLET_H
#include "Common.h"
#include "WorldEntity.h"
#include "Mesh.h"
#include "Matrix.h"

class Bullet : public WorldEntity
{
public:
	Bullet()
	{
		Transform.Scale = 0.5f;
		Transform.Rotation = Vector3(1.f, 0.5f, 1.f);
	}

	void setBullet(const Vector3& coor, float radiusAngle)
	{
		Transform.Translation = coor;
		velocity = Vector4(-5*sin(radiusAngle), 5*cos(radiusAngle), 0.f, 0.f);
	}

	inline void hide()
	{
		setBullet(Vector3(0, 0, 10), 0);
		invisible = 1;
	}

	Vector4 velocity;

	bool OnInitialize() override;
	void OnUpdate(const GameTime& time) override;
	void OnRender(const GameTime& time) override;
private:
	Mesh* m_mesh;
	Material* m_material;

	Vector4 color = Vector4(0.2f, 0.2f, 0.2f, 0);

	std::vector<float> vertices;
	std::vector<GLushort> indices;

	void checkBound() override;
};

#endif
