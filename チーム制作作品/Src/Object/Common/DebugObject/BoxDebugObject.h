#pragma once

#include "DebugObjectBase.h"

#include "../Collider/BoxCollider.h"

class BoxDebugObject : public DebugObjectBase
{
public:
	BoxDebugObject(
		const Vector3& size,

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
		size(size)
	{
	}
	~BoxDebugObject()override = default;

	void Load(void)override {
		ColliderCreate(new BoxCollider(COLLIDER_TAG::DEBUG_OBJECT, size));
	}

private:
	Vector3 size;
};