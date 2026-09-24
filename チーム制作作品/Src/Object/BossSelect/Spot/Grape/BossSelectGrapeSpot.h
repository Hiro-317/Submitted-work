#pragma once

#include "../BossSelectSpotBase.h"

class BossSelectGrapeSpot : public BossSelectSpotBase
{
public:
	BossSelectGrapeSpot() :
		BossSelectSpotBase(
			BOSS_TYPE::Grape,
			"BossSelect/BossSelectGrape/BossSelectGrape",
			7.0f, 8.0f,
			100.0f,
			Vector3::Yonly(100.0f),
			Vector3::XZonly(3000.0f, -1500.0f),
			Vector3::Yonly(Deg2Rad(30.0f))
		)
	{
	}
	~BossSelectGrapeSpot()override = default;
};