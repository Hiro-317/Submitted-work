#pragma once

#include "../../Character/CharacterBase.h"

class BossSelectSkyShip : public CharacterBase
{
public:

	// ó‘Ô
	enum class STATE {

		None = -1,

		Start,

		Move,

		Max
	};

	BossSelectSkyShip();

	~BossSelectSkyShip()override = default;

	// “Ç‚İ‚İ
	void Load(void)override;

	// óMˆ—
	void ReceptionUpdate(void)override;

	// ‘—Mˆ—
	void SendUpdate(void)override;

	// “–‚½‚è”»’è‚Ì’Ê’m
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

private:

	void CharacterInit(void)override;
};