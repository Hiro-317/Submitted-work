#pragma once

#include "../ActorBase.h"

#include "../../Application/Application.h"

#include "../Common/Collider/BoxCollider.h"
#include "../Common/Collider/XZCircleCollider.h"

class BossSelectStage : public ActorBase
{
public:

	BossSelectStage() : ActorBase() {}
	~BossSelectStage()override = default;

	void Load(void)override {

		SetDynamicFlg(false);
		SetPushFlg(true);

		// モデルをロード
		trans.Load("BossSelect/BossSelectStage/BossSelectStage");

		// スケール
		trans.scale = 10.0f;

		// モデルのズレ
		trans.centerDiff = Vector3(0.0f, -416.71, 0.0f) * trans.scale;

		// コライダー
		ColliderCreate(new BoxCollider(COLLIDER_TAG::STAGE, Vector3(20000, 10000, 2000), Vector3::Zonly(-8500.0f)));
		ColliderCreate(new BoxCollider(COLLIDER_TAG::STAGE, Vector3(20000, 10000, 2000), Vector3::Zonly(5000.0f)));
		ColliderCreate(new BoxCollider(COLLIDER_TAG::STAGE, Vector3(2000, 10000, 20000), Vector3::Xonly(-6000.0f)));
		ColliderCreate(new BoxCollider(COLLIDER_TAG::STAGE, Vector3(2000, 10000, 20000), Vector3::Xonly(6000.0f)));

		ColliderCreate(new BoxCollider(COLLIDER_TAG::STAGE, Vector3(4000, 10000, 1000), Vector3::XZonly(3000.0f, -7000.0f)));
	}
};