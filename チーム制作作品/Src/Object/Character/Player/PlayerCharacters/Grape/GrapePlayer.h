#pragma once
#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class GrapePlayer : public PlayerBase
{
public:

	// コンストラクタ
	GrapePlayer(MSG_SENDER_ID operatorSenderId);
	~GrapePlayer()override = default;

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
		DOWN,
		DEATH,
		KICK_DOWN,
		THROW,
		TOSS,
		JUMP_START,
		JUMP_LOOP,
		STAMP,

		MAX
	};

	// アニメーションの再生速度
	const float ANIME_SPEED[(int)ANIME_TYPE::MAX] = {
		1.0f,	// IDLE
		1.0f,	// WALK
		1.0f,	// RUN
		1.0f,	// DAMAGE
		1.0f,	// DOWN
		1.0f,	// DEATH
		1.0f,	// KICK_DOWN
		1.0f,	// THROW
		1.0f,	// TOSS
		1.0f,	// JUMP_START
		1.0f,	// JUMP_LOOP
		1.0f,	// STAMP
	};

#pragma region スキル1の定数定義

	const short SKILL1_ATTACK_RATE = GetParameter("GrapeShot", "AttackRate");

	const int SKILL1_COOL_TIME = GetParameter("GrapeShot", "CoolTime");

#pragma endregion 

#pragma region スキル2の定数定義

	const short SKILL2_ATTACK_RATE = GetParameter("GrapeThrowBomb", "AttackRate");

	const int SKILL2_COOL_TIME = GetParameter("GrapeThrowBomb", "CoolTime");

#pragma endregion 

#pragma region スキル3の定数定義

	const short SKILL3_ATTACK_RATE = GetParameter("GrapeBomb", "AttackRate");
	const int SKILL3_COOL_TIME = GetParameter("GrapeBomb", "CoolTime");

	const int ATTACK_COUNT_TIME = GetParameter("GrapeBomb", "CountTime");
	const int ATTACK_START_TIME = GetParameter("GrapeBomb", "StartTime");

#pragma endregion

};