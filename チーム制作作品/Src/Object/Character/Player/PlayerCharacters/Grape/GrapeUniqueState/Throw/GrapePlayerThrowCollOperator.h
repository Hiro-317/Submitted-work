#pragma once
#include "../../../../../../ActorBase.h"
#include "../../../../../../../Manager/Net/NetWorkManager.h"

struct CharacterStats;

class GrapePlayerThrowCollOperator : public ActorBase
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
	GrapePlayerThrowCollOperator(
		COLLIDER_TAG COLL_TAG,
		const short ATTACK_RATE_PERCENT,
		const Vector3& playerPos, const Vector3& playerAngle,
		MSG_SENDER_ID operatorSenderId,
		const CharacterStats& playerStats
	);
	~GrapePlayerThrowCollOperator()override = default;

	// ロード
	void Load(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	// 攻撃の判定を発生させる
	void CollOn(void) {
		SetJudge(true);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(true, MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerThrow));
		}
	}
	// 攻撃の判定を消す
	void CollOff(void) {
		SetJudge(false);
		if (!Net::GetIns().IsHost()) {
			Net::GetIns().Send(MsgDataPlayerCollOperator(false, MsgDataPlayerCollOperator::COLLIDER_TYPE::GrapePlayerThrow));
		}
	}

	// プレイヤーの位置に爆弾を設置する
	void SetTargetVec(const Vector3& vec) {
	}


	void RemoteThrowBombStart(const Vector3& pos, const Vector3& vec);
	void RemoteThrowBombEnd(void);

	void LocalThrowBombStart(const Vector3& pos, const Vector3& vec);
	void LocalThrowBombEnd(void);

private:

	// 更新処理
	void SubUpdate(void)override;

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

	Vector3 targetVec;

	float gravity;

	float bouncePower;

	bool isBlast;
};