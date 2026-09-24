#pragma once

#include "DebugObjectBase.h"

#include "../Collider/SphereCollider.h"

class SphereDebugObject : public DebugObjectBase
{
public:
	SphereDebugObject(
		float radius,

		const Vector3& pos = Vector3(),

		bool dynamicFlg = true,
		bool isGravity = true,
		bool pushFlg = true,
		unsigned char pushWeight = 50,
		bool isOperator = false
	) :
		DebugObjectBase(
			pos,
			dynamicFlg,
			isGravity,
			pushFlg,
			pushWeight,
			isOperator
		),
		radius(radius)
	{
	}
	~SphereDebugObject()override = default;

	void Load(void)override {
		ColliderCreate(new SphereCollider(COLLIDER_TAG::DEBUG_OBJECT, radius));
	}

private:
	float radius;
};