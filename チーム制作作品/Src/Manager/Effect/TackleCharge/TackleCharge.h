#pragma once

#include "../EffectBase.h"

class TackleCharge : public EffectBase {
public:

	TackleCharge(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
		bool followRotX, bool followRotY, bool followRotZ);

	~TackleCharge() = default;
};