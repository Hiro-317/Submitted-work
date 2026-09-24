#pragma once

#include "../EffectBase.h"

class EffectHeal : public EffectBase {
public:

	EffectHeal(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
		bool followRotX, bool followRotY, bool followRotZ);

	~EffectHeal() = default;
};