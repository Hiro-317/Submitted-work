#pragma once

#include "../BossSelectSpotBase.h"

class BossSelectBananaSpot : public BossSelectSpotBase
{
public:
	BossSelectBananaSpot() :
		BossSelectSpotBase(
			BOSS_TYPE::Banana,
			"BossSelect/BossSelectBanana/BossSelectBanana",
			7.0f, 8.0f,
			100.0f,
			Vector3::YZonly(100.0f, -10.0f),
			Vector3::XZonly(-2000.0f, 3000.0f),
			Vector3::Yonly(Deg2Rad(-30.0f))
		)
	{
	}
	~BossSelectBananaSpot()override = default;
};