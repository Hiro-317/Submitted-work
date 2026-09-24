#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class PeachPlayer : public PlayerBase
{
public:

	// コンストラクタ
	PeachPlayer(MSG_SENDER_ID operatorSenderId);
	~PeachPlayer()override = default;

	// ロード
	void PlayerLoad(void)override;

	// 当たり判定処理
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	// 同期処理
	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;

private:

	// アニメーションの種類
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,
		WALK,
		RUN,
		DAMAGE,
		DEATH,
		PUNCH,
		HEAL,

		MAX
	};

	// アニメーションの再生速度
	const float ANIME_SPEED[(int)ANIME_TYPE::MAX] = {
		1.0f,	// IDLE,
		1.0f,	// WALK,
		1.0f,	// RUN,
		1.0f,	// DAMAGE,
		1.0f,	// DEATH,
		1.0f,	// PUNCH,
		2.5f	// HEAL,
	};

#pragma region スキル1の定数定義

	// スキル2状態～～～～～～～～～～～～～～～～～～～～～～～～～～～

	// 攻撃対象の探索範囲
	const float SKILL_1_TARGET_SERCH_RANGE = GetParameter("SimpleAttack", "TargetSerchRange");

	// 当たり判定のタグ
	const COLLIDER_TAG SKILL_1_COLL_TAG = COLLIDER_TAG::PLAYER_ATTACK;

	// 当たり判定のサイズ（半径）
	const float SKILL_1_COLL_SIZE_TABLE = GetParameter("SimpleAttack", "CollSize");

	// 当たり判定のオフセット
	const Vector3 SKILL_1_COLL_LOCAL_POS = GetParameterToVector3("SimpleAttack", "CollLocalPos");

	// 攻撃そのもののクールタイム
	const int SKILL_1_COOL_TIME = GetParameterToInt("SimpleAttack", "CoolTime");

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	const float SKILL_1_COLL_START_TIME = GetParameter("SimpleAttack", "CollStartTime");

	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	const float SKILL_1_COLL_END_TIME = GetParameter("SimpleAttack", "CollEndTime");

	// 攻撃中の移動速度
	const float SKILL_1_ATTACK_MOVE_SPEED = GetParameter("SimpleAttack", "AttackMoveSpeed");

	// スキルの攻撃倍率
	const short SKILL_1_ATTACK_RATE_PERCENT = GetParameterToInt("SimpleAttack", "AttackRatePercent");

	// ～～～～～～～～～～～～～～～～～～～～～～～～～～～スキル2状態
#pragma endregion

#pragma region スキル2の定数定義

	//// 当たり判定のタグ
	//const COLLIDER_TAG SKILL_2_COLL_TAG = COLLIDER_TAG::PLAYER_ATTACK;

	//// 攻撃そのもののクールタイム
	//const int SKILL_2_COOL_TIME = GetParameterToInt("SingleHeal", "CoolTime");

#pragma endregion 

#pragma region スキル3の定数定義


#pragma endregion 
};