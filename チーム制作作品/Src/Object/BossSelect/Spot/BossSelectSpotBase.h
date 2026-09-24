#pragma once

#include "../../ActorBase.h"
#include "../../CharaTypeDefine.h"
#include <map>

class BossSelectSpotBase : public ActorBase
{
public:
	BossSelectSpotBase(
		BOSS_TYPE bossType,
		std::string MODEL_PATH,
		float defaultModelScale, float selectedModelScale,
		float DEFAULT_COLLIDER_RADIUS,
		Vector3 DEFAULT_SELECTED_IMAGE_OFFSET,
		const Vector3& pos, const Vector3& angle
	);
	~BossSelectSpotBase()override = default;

	void Load(void)override;

	void SelectedDraw(const int& selectedImage);

	// 当たり判定の通知
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

private:
	
	// 飛行船が使用するコライダーのタグ
	static constexpr COLLIDER_TAG SKY_SHIP_COLL_TAG = COLLIDER_TAG::PLAYER;

	// このクラスのボスタイプ
	const BOSS_TYPE bossType;

	// モデルパス
	const std::string MODEL_PATH;

	// モデルの表示倍率
	const std::map<bool, float> MODEL_SCALE;

	// モデルの表示倍率の変化速度
	static constexpr float MODEL_SCALE_EASING_RATE = 1.0f / 10.0f;

	// コライダーの半径
	const float COLLIDER_RADIUS;

	// 押し出しを行うコライダーの半径に対して、選択判定を行う半径の差分
	static constexpr float COLLIDER_RADIUS_EXTRA = 50.0f;

	// 選択時に表示するイメージの相対座標
	const Vector3 SELECTED_IMAGE_OFFSET;

	// 現在飛行船が近くにいるかどうか（現フレーム）
	bool selected;

	// 選択時表示イメージのイージングカウンター
	float selectedEasingCounter;
	// 選択時表示イメージのイージングレート
	float selectedEasingRate;

	void SubUpdate(void)override;
};