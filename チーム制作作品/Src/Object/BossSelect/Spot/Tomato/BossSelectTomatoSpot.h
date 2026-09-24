#pragma once

#include "../BossSelectSpotBase.h"

class BossSelectTomatoSpot : public BossSelectSpotBase
{
public:

	BossSelectTomatoSpot() :
		BossSelectSpotBase(
			BOSS_TYPE::Tomato,
			"BossSelect/BossSelectTomato/BossSelectTomato",
			7.0f, 8.0f,
			100.0f,
			Vector3::Yonly(100.0f),
			Vector3::XZonly(-2500.0f, -4000.0f),
			Vector3::Yonly(Deg2Rad(-40.0f))
		)
	{
	}
	~BossSelectTomatoSpot()override = default;
};