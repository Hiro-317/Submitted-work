#pragma once

#include "../../../../ActorBase.h"
#include "../../../../../Manager/Net/NetWorkManager.h"

#include "PlayerTripleAttackStDefine.h"

class PlayerTripleAttackCollOperator : public ActorBase
{
public:

	/// <summary>
	/// コンストラクト
	/// </summary>
	/// <param name="FIND_ATTACK_TARGET_RANGE">攻撃対象の探索範囲</param>
	/// <param name="COLL_TAG"></param>
	/// <param name="COLL_SIZE"></param>
	/// <param name="COLL_LOCAL_POS">攻撃判定の座標（プレイヤー座標からのローカル座標）</param>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="playerAngle">プレイヤーの角度</param>
	PlayerTripleAttackCollOperator(
		float FIND_ATTACK_TARGET_RANGE,
		COLLIDER_TAG COLL_TAG,
		const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_SIZE,
		const Vector3& COLL_LOCAL_POS,
		const short SKILL_1_ATTACK_RATE_PERCENT,

		const Vector3& playerPos, const Vector3& playerAngle,

		MSG_SENDER_ID operatorSenderId,
		const CharacterStats& playerStats
	);
	~PlayerTripleAttackCollOperator()override = default;

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

	// 指定の段の攻撃の判定を発生させる
	void CollOn(PLAYER_TRIPLE_ATTACK_STAGE stage) {
		if(!isHit)ColliderSerch(GetCollider(), COLL_TAG).at((int)stage)->SetJudgeFlg(true);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(true, (MsgDataPlayerCollOperator::COLLIDER_TYPE)stage));
		}
	}

	// 指定の段の攻撃の判定を消す（指定がない場合は全ての段の判定を消す）
	void CollOff(PLAYER_TRIPLE_ATTACK_STAGE stage = PLAYER_TRIPLE_ATTACK_STAGE::NON) {
		if (stage == PLAYER_TRIPLE_ATTACK_STAGE::NON) {
			// 指定がない場合は全ての段の判定を消す
			for (ColliderBase* coll : ColliderSerch(GetCollider(), COLL_TAG)) { coll->SetJudgeFlg(false); }
		}
		// 指定がある場合はその段の判定を消す
		else { ColliderSerch(GetCollider(), COLL_TAG).at((int)stage)->SetJudgeFlg(false); }

		// コライダー消去通知を送信
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_1));
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_2));
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::CommonPlayerTripleAttack_3));
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

	// 三段攻撃の攻撃倍率
	const short SKILL_1_ATTACK_RATE_PERCENT;

	// 攻撃の段階ごとのタグ
	COLLIDER_TAG COLL_TAG;

	// 攻撃の段階ごとの判定のサイズ（半径）
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> COLL_SIZE;

	// 攻撃の段階ごとの判定のローカル座標
	const Vector3 COLL_LOCAL_POS;

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

	// 攻撃対象の座標
	const Vector3* targetPos;


	// 更新処理
	void SubUpdate(void)override;
};