#pragma once

#include "../../ActorBase.h"

class LobbyStage : public ActorBase
{
public:
	LobbyStage() : ActorBase() {}
	~LobbyStage() = default;

	void Load(void)override {
		// モデルのロード
		trans.Load("SkyShip/SkyShip");
		trans.scale = 50.0f;
		trans.centerDiff = Vector3(0, 30, 0) * trans.scale;
		trans.pos = Vector3(300.0f, 0.0f, 200.0f);
		trans.angle.y = Deg2Rad(-35.0f);
	}
	void Update(void)override {}

private:
	void SubInit(void)override {
		// 影を消す（消さなかったら、変な色合いになるので）
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	}
};