#pragma once

#include "../../ActorBase.h"

#include "../../../Manager/Input/KeyManager.h"
#include "../../../Manager/Camera/CurrentCamera.h"

class DebugObjectBase : public ActorBase
{
public:
	DebugObjectBase(
		const Vector3& pos,

		bool dynamicFlg,
		bool isGravity,
		bool pushFlg,
		unsigned char pushWeight,

		bool isOperator
	) :
		ActorBase(),
		isOperator(isOperator)
	{
		trans.pos = pos;
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}
	DebugObjectBase(
		const std::string& parameterPath,

		const Vector3& pos,

		bool dynamicFlg,
		bool isGravity,
		bool pushFlg,
		unsigned char pushWeight,

		bool isOperator
	) :
		ActorBase(parameterPath),
		isOperator(isOperator)
	{
		trans.pos = pos;
		SetDynamicFlg(dynamicFlg);
		SetGravityFlg(isGravity);
		SetPushFlg(pushFlg);
		SetPushWeight(pushWeight);
	}
	~DebugObjectBase()override = default;

private:
	// ‘€ì‰Â”\‚©‚Ç‚¤‚©
	bool isOperator;

	void SubUpdate(void)override {
		if (!isOperator) { return; }

		// ˆÚ“®•ûŒü
		Vector3 vec = {};

		// ‘€ì
		if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_OBJECT_FRONT).now) { vec.z++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_OBJECT_BACK).now) { vec.z--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_OBJECT_RIGHT).now) { vec.x++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_OBJECT_LEFT).now) { vec.x--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_OBJECT_UP).now) { vec.y++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::DEBUG_OBJECT_DOWN).now) { vec.y--; }

		// ƒJƒƒ‰‚ÌŠp“x‚É‡‚í‚¹‚ÄˆÚ“®•ûŒü‚ğ‰ñ“]‚³‚¹‚é
		vec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// ˆÚ“®
		trans.pos += vec * 5.0f;
		trans.angle.y = atan2f(vec.x, vec.z);
	}
};