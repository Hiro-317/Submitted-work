#pragma once

#include "DebugObjectBase.h"

#include "../Collider/CapsuleCollider.h"

class CapsuleDebugObject : public DebugObjectBase
{
public:
	CapsuleDebugObject(
		const Vector3& startPos,
		const Vector3& endPos,
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
		startPos(startPos),
		endPos(endPos),
		radius(radius)
	{
	}
	~CapsuleDebugObject()override = default;

	void Load(void)override {
		ColliderCreate(new CapsuleCollider(COLLIDER_TAG::DEBUG_OBJECT, startPos, endPos, radius));
	}

private:
	Vector3 startPos, endPos;
	float radius;
};