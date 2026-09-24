#include "EffectTackleMove.h"


EffectTackleMove::EffectTackleMove(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
	bool followRotX, bool followRotY, bool followRotZ)
	: EffectBase(parameter)
{
	info.follow = follow;
	info.name = name;
	info.trans.pos = local;

	info.followRotX = followRotX;
	info.followRotY = followRotY;
	info.followRotZ = followRotZ;

	info.trans.LoadEffect("TackleMove");

	info.trans.angle = GetParameterToVector3("TackleMove", "Angle");
	info.trans.scale = GetParameterToVector3("TackleMove", "Scale");
}