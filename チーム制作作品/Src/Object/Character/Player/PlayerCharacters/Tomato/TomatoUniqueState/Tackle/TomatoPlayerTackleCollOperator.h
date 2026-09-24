#pragma once
#include "../../../../../../ActorBase.h"
#include "../../../../../../../Manager/Net/NetWorkManager.h"

struct CharacterStats;

class TomatoPlayerTackleCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="COLL_TAG">コライダーに着けるタグ</param>
	/// <param name="SKILL_2_ATTACK_RATE_PERCENT">攻撃倍率</param>
	/// <param name="playerPos">プレイヤー座標</param>
	/// <param name="playerAngle">プレイヤーアングルram>
	/// <param name="operatorSenderId"></param>
	/// <param name="playerStats">プレイヤーのステータス情報</param>
	TomatoPlayerTackleCollOperator(
		COLLIDER_TAG COLL_TAG,
		const short ATTACK_RATE_PERCENT,
		const Vector3& playerPos, const Vector3& playerAngle,
		MSG_SENDER_ID operatorSenderId,
		const CharacterStats& playerStats
	);
	~TomatoPlayerTackleCollOperator()override = default;

	// ロード
	void Load(void)override;
	// 更新処理
	void Update(void)override;
	// 描画処理
	void SubAlphaDraw(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	// 攻撃の判定を発生させる
	void CollOn() { 
		if (isHit) { return; }
		SetJudge(true);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(true, MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerTackle));
		}
	}

	// 攻撃の判定を消す
	void CollOff() {
		SetJudge(false);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::TomatoPlayerTackle));
		}
	}

	// 攻撃のヒット管理のフラグをリセットする
	void ResetIsHit(void) { isHit = false; }
	const bool GetIsHit(void) { return isHit; }
private:

#pragma region 定数

	// 三段攻撃の攻撃倍率
	const short ATTACK_RATE_PERCENT;

	// 攻撃の段階ごとのタグ
	COLLIDER_TAG COLL_TAG;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーの座標
	const Vector3& playerPos;
	// プレイヤーの向き
	const Vector3& playerAngle;

	const MSG_SENDER_ID operatorSenderId;

	// プレイヤーのステータス
	const CharacterStats& playerStats;
#pragma endregion

	// 攻撃対象が見つかったかどうか
	bool isFindAttackTarget;

	// 攻撃のヒット管理のフラグ
	bool isHit;

	// 攻撃範囲描画フラグ
	bool isDrawArea;
};