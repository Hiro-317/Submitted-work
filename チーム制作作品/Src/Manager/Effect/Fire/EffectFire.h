#pragma once

#include "../EffectBase.h"

class EffectFire : public EffectBase {
public:

	EffectFire(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
		bool followRotX, bool followRotY, bool followRotZ);

	~EffectFire() = default;
};