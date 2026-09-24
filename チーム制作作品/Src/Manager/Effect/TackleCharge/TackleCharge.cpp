#include "TackleCharge.h"


TackleCharge::TackleCharge(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
	bool followRotX, bool followRotY, bool followRotZ)
	: EffectBase(parameter)
{
	info.follow = follow;
	info.name = name;
	info.trans.pos = local;

	info.followRotX = followRotX;
	info.followRotY = followRotY;
	info.followRotZ = followRotZ;

	info.trans.LoadEffect("TackleCharge");

	info.trans.angle = Vector3();
	info.trans.scale = Vector3(1.0f);
}