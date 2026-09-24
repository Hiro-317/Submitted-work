#pragma once

#include "../EffectBase.h"

class EffectStampShockWave : public EffectBase {
public:

	EffectStampShockWave(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
		bool followRotX, bool followRotY, bool followRotZ);

	~EffectStampShockWave() = default;
};