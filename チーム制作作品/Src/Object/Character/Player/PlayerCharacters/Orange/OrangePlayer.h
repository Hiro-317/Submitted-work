#pragma once

#include "../../PlayerBase.h"

#include "../../../../../Common/Vector2.h"

class PlayerTripleAttackCollOperator;


class OrangePlayer : public PlayerBase
{
public:
	OrangePlayer(MSG_SENDER_ID operatorSenderId);	
	~OrangePlayer()override = default;

	void PlayerLoad(void)override;

	// “–‚½‚è”»’èˆ—
	void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other, const Vector3& collisionPoint)override;

	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;
private:

#pragma region ’è”’è‹`



	// ƒXƒLƒ‹1ó‘Ô```````````````````````````

	// UŒ‚‘ÎÛ‚Ì’Tõ”ÍˆÍ
	const float SKILL_1_TARGET_SERCH_RANGE = GetParameter("TripleAttack", "TargetSerchRange");

	// “–‚½‚è”»’è‚ÌƒTƒCƒYƒe[ƒuƒ‹i”¼Œaj
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> SKILL_1_COLL_SIZE_TABLE =
	{
		GetParameter("TripleAttack","CollSize1"),
		GetParameter("TripleAttack","CollSize2"),
		GetParameter("TripleAttack","CollSize3"),
	};

	// “–‚½‚è”»’è‚ÌƒIƒtƒZƒbƒg
	const Vector3 SKILL_1_COLL_LOCAL_POS = GetParameterToVector3("TripleAttack", "CollLocalPos");

	// UŒ‚‚»‚Ì‚à‚Ì‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SKILL_1_COOL_TIME = GetParameterToInt("TripleAttack", "CoolTime");

	// UŒ‚‚ªŸ’i‚ÉŒq‚ª‚é—P—\ŠÔ
	const int SKILL_1_ATTACK_NEXT_STAGE_CONTINUE_TIME = GetParameterToInt("TripleAttack", "AttackNextStageContinueTime");

	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éŠJnŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> SKILL_1_COLL_START_TIME =
	{
		GetParameter("TripleAttack","CollStartTime1"),
		GetParameter("TripleAttack","CollStartTime2"),
		GetParameter("TripleAttack","CollStartTime3"),
	};
	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éI—¹ŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const std::array<float, (size_t)PLAYER_TRIPLE_ATTACK_STAGE::MAX> SKILL_1_COLL_END_TIME =
	{
		GetParameter("TripleAttack","CollEndTime1"),
		GetParameter("TripleAttack","CollEndTime2"),
		GetParameter("TripleAttack","CollEndTime3"),
	};

	// UŒ‚’†‚ÌˆÚ“®‘¬“x
	const float SKILL_1_ATTACK_MOVE_SPEED = GetParameter("TripleAttack", "AttackMoveSpeed");

	// ƒXƒLƒ‹‚ÌUŒ‚”{—¦
	const short SKILL_1_ATTACK_RATE_PERCENT = (short)GetParameter("TripleAttack", "AttackRatePercent");

	// ```````````````````````````ƒXƒLƒ‹1ó‘Ô


	// ƒXƒLƒ‹2ó‘Ô```````````````````````````

	// UŒ‚‘ÎÛ‚Ì’Tõ”ÍˆÍ
	const float SKILL_2_TARGET_SERCH_RANGE = GetParameter("SimpleAttack", "TargetSerchRange");

	// “–‚½‚è”»’è‚Ìƒ^ƒO
	const COLLIDER_TAG SKILL_2_COLL_TAG = COLLIDER_TAG::PLAYER_ATTACK;

	// “–‚½‚è”»’è‚ÌƒTƒCƒYi”¼Œaj
	const float SKILL_2_COLL_SIZE_TABLE = GetParameter("SimpleAttack", "CollSize");

	// “–‚½‚è”»’è‚ÌƒIƒtƒZƒbƒg
	const Vector3 SKILL_2_COLL_LOCAL_POS = GetParameterToVector3("SimpleAttack", "CollLocalPos");

	// UŒ‚‚»‚Ì‚à‚Ì‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SKILL_2_COOL_TIME = GetParameterToInt("SimpleAttack", "CoolTime");

	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éŠJnŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_2_COLL_START_TIME = GetParameter("SimpleAttack", "CollStartTime");

	// UŒ‚‚Ì”»’è‚ğ”­¶‚³‚¹‚éI—¹ŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_2_COLL_END_TIME = GetParameter("SimpleAttack", "CollEndTime");

	// UŒ‚’†‚ÌˆÚ“®‘¬“x
	const float SKILL_2_ATTACK_MOVE_SPEED = GetParameter("SimpleAttack", "AttackMoveSpeed");

	// ƒXƒLƒ‹‚ÌUŒ‚”{—¦
	const short SKILL_2_ATTACK_RATE_PERCENT = GetParameterToInt("SimpleAttack", "AttackRatePercent");

	// ```````````````````````````ƒXƒLƒ‹2ó‘Ô


	// ƒXƒLƒ‹3ó‘Ô```````````````````````````

	// ‰ñ”ğ‚»‚Ì‚à‚Ì‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SKILL_3_COOL_TIME = GetParameterToInt("Dodge","CoolTime");

	// ‰ñ”ğ‚Ì–³“G”»’è‚ğ”­¶‚³‚¹‚éŠJnŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_3_INVI_START_TIME = GetParameter("Dodge","InviStartTime");
	// ‰ñ”ğ‚Ì–³“G”»’è‚ğ”­¶‚³‚¹‚éI—¹ŠÔiƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶Š„‡j
	const float SKILL_3_INVI_END_TIME = GetParameter("Dodge","InviEndTime");

	// ‰ñ”ğ’†‚ÌˆÚ“®‘¬“x
	const float SKILL_3_MOVE_SPEED = GetParameter("Dodge","DodgeMoveSpeed");

	// ```````````````````````````ƒXƒLƒ‹2ó‘Ô



	// ƒAƒjƒ[ƒVƒ‡ƒ“``````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìí—Ş
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,
		WALK,
		RUN,
		PUNCH1,
		PUNCH2,
		PUNCH3,
		KICK,
		DODGE,
		DAMAGE,
		DEATH,

		MAX
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌÄ¶‘¬“x
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::MAX] = 
	{
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "WalkAnimeSpeed"),	// WALK
		GetParameter("Anime", "RunAnimeSpeed"),		// RUN
		GetParameter("Anime", "Punch1AnimeSpeed"),	// PUNCH1
		GetParameter("Anime", "Punch2AnimeSpeed"),	// PUNCH2
		GetParameter("Anime", "Punch3AnimeSpeed"),	// PUNCH3
		GetParameter("Anime", "KickAnimeSpeed"),	// KICK
		GetParameter("Anime", "DodgeAnimeSpeed"),	// DODGE
		GetParameter("Anime", "DamageAnimeSpeed"),	// DAMAGE
		GetParameter("Anime", "DeathAnimeSpeed"),	// DEATH
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒtƒHƒ‹ƒ_‚Ü‚Å‚ÌƒpƒX
	const std::string ANIME_FOLDER_PATH = "Data/Model/Character/Orange/Anime/";

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌƒpƒX
	const std::string ANIME_PATH_TABLE[(int)ANIME_TYPE::MAX] =
	{
		ANIME_FOLDER_PATH + "Idle.mv1",		// IDLE
		ANIME_FOLDER_PATH + "Walk.mv1",		// WALK
		ANIME_FOLDER_PATH + "Run.mv1",		// RUN
		ANIME_FOLDER_PATH + "Punch1.mv1",	// PUNCH1
		ANIME_FOLDER_PATH + "Punch2.mv1",	// PUNCH2
		ANIME_FOLDER_PATH + "Punch3.mv1",	// PUNCH3
		ANIME_FOLDER_PATH + "Kick.mv1",		// KICK
		ANIME_FOLDER_PATH + "Dodge.mv1",	// DODGE
		ANIME_FOLDER_PATH + "Hit.mv1",		// DAMAGE
		ANIME_FOLDER_PATH + "Death.mv1"		// DEATH
	};

	// ``````````````````````````ƒAƒjƒ[ƒVƒ‡ƒ“

#pragma endregion ’è”’è‹`

};