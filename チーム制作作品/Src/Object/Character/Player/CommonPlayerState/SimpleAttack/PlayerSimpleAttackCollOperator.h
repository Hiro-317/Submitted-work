#pragma once

#include "../../../../ActorBase.h"
#include "../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../Common/Collider/ColliderBase.h"

struct CharacterStats;

class PlayerSimpleAttackCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="FIND_ATTACK_TARGET_RANGE">攻撃対象を見つける距離</param>
	/// <param name="COLL_TAG">攻撃のタグ</param>
	/// <param name="COLL_SIZE">攻撃判定のサイズ（半径）</param>
	/// <param name="COLL_LOCAL_POS">攻撃判定のローカル座標</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerAngle">プレイヤーの向き</param>
	PlayerSimpleAttackCollOperator(
		float FIND_ATTACK_TARGET_RANGE,
		COLLIDER_TAG COLL_TAG, float COLL_SIZE,
		const Vector3& COLL_LOCAL_POS,

		const Vector3& playerPos,
		const Vector3& playerAngle,

		const short ATTACK_RATE_PERCENT,

		MSG_SENDER_ID operatorSenderId,
		const CharacterStats& playerStats
	);
	~PlayerSimpleAttackCollOperator()override = default;

	void Load(void)override;

	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	// 攻撃対象を探索
	void TargetSerch(void) {
		ColliderSerch(GetCollider(), COLLIDER_TAG::PLAYER_COMMON).back()->SetJudgeFlg(true);
	}

	// 攻撃対象が見つかったかどうかを取得
	bool GetIsFindAttackTarget(void)const { return isFindAttackTarget; }
	// 攻撃対象の座標を取得
	const Vector3& GetTargetPos(void)const { return *targetPos; }

	// 攻撃の判定を発生させる
	void CollOn(void) { 
		if (!isHit)ColliderSerch(GetCollider(), COLL_TAG).back()->SetJudgeFlg(true);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(true, MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack));
		}
	}

	// 攻撃の判定を消す
	void CollOff(void) { 
		ColliderSerch(GetCollider(), COLL_TAG).back()->SetJudgeFlg(false);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerSimpleAttack));
		}
	}

	// 攻撃対象をリセットする
	void ResetTarget(void) { isFindAttackTarget = false; targetPos = nullptr; }

	// 攻撃のヒット管理のフラグをリセットする
	void ResetIsHit(void) { isHit = false; }
private:

#pragma region 定数

	// 攻撃対象を見つける距離
	const float FIND_ATTACK_TARGET_RANGE;

	// 攻撃のタグ
	const COLLIDER_TAG COLL_TAG;

	// 攻撃判定のサイズ（半径）
	const float COLL_SIZE;

	// 攻撃判定のローカル座標
	const Vector3 COLL_LOCAL_POS;

	const short ATTACK_RATE_PERCENT;

#pragma endregion

#pragma region 受け取る参照変数

	// プレイヤーの座標
	const Vector3& playerPos;
	// プレイヤーの向き
	const Vector3& playerAngle;

	const MSG_SENDER_ID operatorSenderId;

	const CharacterStats& playerStats;

#pragma endregion

	// 攻撃対象が見つかったかどうか
	bool isFindAttackTarget;

	// 攻撃対象の座標
	const Vector3* targetPos;

	// 攻撃のヒット管理のフラグ
	bool isHit;

	// 更新処理
	void SubUpdate(void)override;
};