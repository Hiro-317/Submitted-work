#pragma once

#include "../EffectBase.h"

class EffectBuff : public EffectBase {
public:

	EffectBuff(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
		bool followRotX, bool followRotY, bool followRotZ);

	~EffectBuff() = default;
};