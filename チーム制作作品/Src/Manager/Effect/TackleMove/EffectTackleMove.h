#pragma once

#include "../EffectBase.h"

class EffectTackleMove : public EffectBase {
public:

	EffectTackleMove(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
	bool followRotX, bool followRotY, bool followRotZ);

	~EffectTackleMove() = default;
};