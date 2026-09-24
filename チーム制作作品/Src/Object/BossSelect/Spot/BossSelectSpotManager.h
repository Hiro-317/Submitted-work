#pragma once

#include "../../ActorBase.h"

class BossSelectSpotBase;

class BossSelectSpotManager : public ActorBase
{
public:
	BossSelectSpotManager();
	~BossSelectSpotManager()override = default;

	void Load(void)override;

	std::vector<ColliderBase*> GetCollider(void)const override;

private:

	// 各スポットの実体
	std::vector<BossSelectSpotBase*> spot;

	// 飛行船がスポットに近づいたときに表示する共通UI
	int selectedImage[2];


	void SubInit(void)override;
	void SubUpdate(void)override;
	void SubDraw(void)override;
	void SubAlphaDraw(void)override;
	void SubRelease(void)override;

};