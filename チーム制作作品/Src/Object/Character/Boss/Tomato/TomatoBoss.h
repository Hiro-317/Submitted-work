#pragma once
#include "../BossBase.h"


class TomatoBoss : public BossBase
{
public:
	TomatoBoss(const std::vector<const Vector3*> playerPos, const std::vector<const bool*> playerDeath);
	~TomatoBoss()override = default;

	// 状態遷移後1度行う初期化処理
	std::vector<ColliderBase*> GetCollider(void)const override {
		std::vector<ColliderBase*> ret = {};
		// 自身のコライダーを返却用一時変数に格納
		for (ColliderBase*& coll : ActorBase::GetCollider()) { ret.emplace_back(coll); }

		// 抱える下位クラスの返却用一時変数に格納
		for (ActorBase* const& subObj : subObjArray) {
			for (ColliderBase*& coll : subObj->GetCollider()) { ret.emplace_back(coll); }
		}
		// 最終的な返却用一時変数を返却
		return ret;
	}

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;
	virtual void ReceptionUpdate(void)override;

private:

#pragma region 定数定義

	// 当たり判定情報～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// カプセルコライダーの半径
	const float CAPSULE_COLLIDER_RADIUS = (MODEL_SIZE.y * 0.5f) * GetParameter("Collider","ModelToColliderRate");

	// カプセルコライダーのローカルX始点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_START_POS_X =
		Vector3::Xonly(
			(MODEL_SIZE.x * 0.5f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// カプセルコライダーのローカルX終点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_END_POS_X =
		-Vector3::Xonly(
			(MODEL_SIZE.x * 0.5f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// カプセルコライダーのローカルXZ始点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_START_POS_XZ =
		Vector3::XZonly(
			(MODEL_SIZE.x * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS,
			(MODEL_SIZE.z * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// カプセルコライダーのローカルXZ終点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_END_POS_XZ =
		-Vector3::XZonly(
			(MODEL_SIZE.x * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS,
			(MODEL_SIZE.z * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// カプセルコライダーのローカルZ始点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_START_POS_Z =
		Vector3::Zonly(
			(MODEL_SIZE.z * 0.5f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// カプセルコライダーのローカルZ終点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_END_POS_Z =
		-Vector3::Zonly(
			(MODEL_SIZE.z * 0.5f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// カプセルコライダーのローカルZX始点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_START_POS_ZX =
		Vector3::XZonly(
			-((MODEL_SIZE.x * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS),
			(MODEL_SIZE.z * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);
	// カプセルコライダーのローカルZX終点座標（モデルの中心点からのオフセット）
	const Vector3 CAPSULE_COLLIDER_END_POS_ZX =
		-Vector3::XZonly(
			-((MODEL_SIZE.x * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS),
			(MODEL_SIZE.z * 0.45f) * GetParameter("Collider","ModelToColliderRate")
			- CAPSULE_COLLIDER_RADIUS
		);

	// カプセルコライダーの絶対に当たらないおおよその距離
	const float CAPSULE_COLLIDER_ENOUGH_DISTANCE =
		(CAPSULE_COLLIDER_START_POS_X - CAPSULE_COLLIDER_END_POS_X).Length()
		+ CAPSULE_COLLIDER_RADIUS;

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～


	// 移動状態～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// 移動量
	const float MOVE_SPEED = GetParameter("Move", "Speed");

	// 回転量
	const float ROTATION_POW = GetParameter("Move", "RotationPower");

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～移動状態


	// 攻撃～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// スタンプ範囲
	const float STAMP_RADIUS = GetParameter("Attack", "StampRad");

	// 頭突き攻撃のクールタイム
	const int HEADBUTT_COOLTIME = GetParameterToInt("Attack", "HeadbuttCoolTime");
	// スタンプ攻撃のクールタイム
	const int STAMP_COOLTIME = GetParameterToInt("Attack", "StampCoolTime");
	// タックル攻撃のクールタイム
	const int TACKLE_COOLTIME = GetParameterToInt("Attack", "TackleCoolTime");

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～


	// 画像の名前
	const std::string HP_FRAME_IMAGE_NAME = "BossHpFrame";
	const std::string HP_IMAGE_NAME = "BossHp";
	const std::string HP_LOST_IMAGE_NAME = "BossHpLost";

	// HPバーの画像サイズ
	const Vector2I HP_IMAGE_SIZE = Vector2I(629, 50);
	const Vector2I HP_GAUGE_OFFSET = Vector2I(18, 0);
	const Vector2I HP_UI_POS = Vector2I(App::SCREEN_SIZE_X_HALF - 314, 0);

	// デフォルトのクールタイム
	static constexpr int DEFAULT_COOLTIME = 120;

#pragma endregion 定数定義

	// メイン処理～～～～～～～～～～～～～～
	void PlayerLoad(void)override;
	// ～～～～～～～～～～～～～～メイン処理

	// 岩に当たったか
	bool rockHit;

	// 攻撃のクールタイム
	int coolTime;

};