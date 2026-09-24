#pragma once

#include "../EffectBase.h"

class EffectStampLand : public EffectBase {
public:

	EffectStampLand(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
		bool followRotX, bool followRotY, bool followRotZ);

	~EffectStampLand() = default;
};