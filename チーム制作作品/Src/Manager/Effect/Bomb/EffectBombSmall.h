#pragma once

#include "../EffectBase.h"

class EffectBombSmall : public EffectBase {
public:

	EffectBombSmall(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
		bool followRotX, bool followRotY, bool followRotZ);

	~EffectBombSmall() = default;
};