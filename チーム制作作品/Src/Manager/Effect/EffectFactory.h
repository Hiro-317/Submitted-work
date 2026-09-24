#pragma once
#include "EffectBase.h"

#include "TackleMove/EffectTackleMove.h"
#include "StampLand/EffectStampLand.h"
#include "Heal/EffectHeal.h"
#include "Buff/EffectBuff.h"
#include "TackleCharge/TackleCharge.h"
#include "StampShockWave/EffectStampShockWave.h"
#include "Bomb/EffectBombSmall.h"
#include "Bomb/EffectBombBig.h"
#include "Fire/EffectFire.h"

class EffectFactory {

public:

	static EffectBase* CreateEffect(const ParameterLoad& parameter, EFFECT_NAME name, const Vector3& local, const Transform* trans,
	bool followRotX, bool followRotY, bool followRotZ) {

		EffectBase* res{};

		switch (name){

		case EFFECT_NAME::TACKLE_MOVE: { res = new EffectTackleMove(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		case EFFECT_NAME::STAMP_LAND: { res = new EffectStampLand(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		case EFFECT_NAME::HEAL: { res = new EffectHeal(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		case EFFECT_NAME::BUFF: { res = new EffectBuff(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		case EFFECT_NAME::TACKLE_CHARGE: { res = new TackleCharge(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		case EFFECT_NAME::STAMP_SHOCK_WAVE: { res = new EffectStampShockWave(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		case EFFECT_NAME::BOMB_SMALL: { res = new EffectBombSmall(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		case EFFECT_NAME::BOMB_BIG: { res = new EffectBombBig(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }
		case EFFECT_NAME::FIRE: { res = new EffectFire(parameter, name, local, trans, followRotX, followRotY, followRotZ); break; }

		default: { break; }
		}
		return res;
	}
};