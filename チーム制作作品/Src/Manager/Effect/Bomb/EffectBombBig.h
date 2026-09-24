#pragma once

#include "../EffectBase.h"

class EffectBombBig : public EffectBase {
public:

	EffectBombBig(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* follow,
		bool followRotX, bool followRotY, bool followRotZ);

	~EffectBombBig() = default;
};