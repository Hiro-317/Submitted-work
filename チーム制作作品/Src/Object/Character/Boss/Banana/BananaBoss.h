#pragma once
#include "../BossBase.h"


class BananaBoss : public BossBase
{
public:
	BananaBoss(const std::vector<const Vector3*> playerPos, const std::vector<const bool*> playerLive);
	~BananaBoss()override = default;

	// ó‘Ô‘JˆÚŒã1“xs‚¤‰Šú‰»ˆ—
	std::vector<ColliderBase*> GetCollider(void)const override {
		std::vector<ColliderBase*> ret = {};
		// ©g‚ÌƒRƒ‰ƒCƒ_[‚ğ•Ô‹p—pˆê•Ï”‚ÉŠi”[
		for (ColliderBase*& coll : ActorBase::GetCollider()) { ret.emplace_back(coll); }

		// •ø‚¦‚é‰ºˆÊƒNƒ‰ƒX‚Ì•Ô‹p—pˆê•Ï”‚ÉŠi”[
		for (ActorBase* const& subObj : subObjArray) {
			for (ColliderBase*& coll : subObj->GetCollider()) { ret.emplace_back(coll); }
		}
		// ÅI“I‚È•Ô‹p—pˆê•Ï”‚ğ•Ô‹p
		return ret;
	}

	virtual void ReceptionUpdate(void)override;

protected:

	// ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
	void PlayDamage(void) override;

private:

	void CharacterInit(void)override;

#pragma region ’è”’è‹`

	// UŒ‚``````````````````````````````````````````````

	// ‚©‚©‚Æ—‚Æ‚µ‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	const int SCRATCH_COOLTIME = GetParameterToInt("Attack", "ScratchCoolTime");
	//// ’¼ü“Š±‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	//const int STRAIGHT_COOLTIME = GetParameterToInt("Attack", "StraightCoolTime");
	//// ƒXƒ^ƒ“ƒvUŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	//const int STAMP_COOLTIME = GetParameterToInt("Attack", "StampCoolTime");
	//// ˆê‚Â‚ÌUŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	//const int SINGLE_COOLTIME = GetParameterToInt("Attack", "SingleCoolTime");
	//// ’Ç]UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	//const int STALKER_COOLTIME = GetParameterToInt("Attack", "StalkerCoolTime");
	//// ƒ‰ƒ“ƒ_ƒ€UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	//const int RANDOM_COOLTIME = GetParameterToInt("Attack", "RandomCoolTime");

	// ``````````````````````````````````````````````UŒ‚


	// ˆÚ“®ó‘Ô`````````````````````````````

	// ˆÚ“®—Ê
	const float MOVE_SPEED = GetParameter("Move", "Speed");

	// `````````````````````````````ˆÚ“®ó‘Ô


	// ‰æ‘œ‚Ì–¼‘O
	const std::string HP_FRAME_IMAGE_NAME = "BossHpFrame";
	const std::string HP_IMAGE_NAME = "BossHp";
	const std::string HP_LOST_IMAGE_NAME = "BossHpLost";

	// HPƒo[‚Ì‰æ‘œƒTƒCƒY
	const Vector2I HP_IMAGE_SIZE = Vector2I(629, 50);
	const Vector2I HP_GAUGE_OFFSET = Vector2I(18, 0);
	const Vector2I HP_UI_POS = Vector2I(App::SCREEN_SIZE_X_HALF - 314, 0);

	// ƒQ[ƒ€ƒNƒŠƒA‚Ì•ÏXŠÔ
	static constexpr int GAMECLEAR_CHANGE_TIME = 120;

	// ƒAƒjƒ[ƒVƒ‡ƒ“``````````````````````````

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìí—Ş
	enum class ANIME_TYPE
	{
		NONE = -1,

		IDLE,

		SCRATCH,
		SCRATCH_START,
		SCRATCH_LOOP,
		SCRATCH_END,

		BREATH,
		BREATH_READY,
		BREATH_READY_LOOP,
		BREATH_ATTACK,
		BREATH_ATTACK_LOOP,
		
		JUMP,
		JUMP_START,
		JUMP_LOOP,

		DAMAGE,
		DEATH,
		DEATH_FALL,

		MAX
	};

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìƒ^ƒCƒv‚²‚Æ‚ÌÄ¶‘¬“x
	const float ANIME_SPEED_TABLE[(int)ANIME_TYPE::MAX] =
	{
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE

		GetParameter("Anime", "ScratchAnimeSpeed"),	// SCRATCH
		GetParameter("Anime", "ScratchAnimeSpeed"),	// SCRATCH
		GetParameter("Anime", "ScratchAnimeSpeed"),	// SCRATCH
		GetParameter("Anime", "ScratchAnimeSpeed"),	// SCRATCH

		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE

		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE

		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "IdleAnimeSpeed"),	// IDLE
		GetParameter("Anime", "FireAnimeSpeed"),	// FIRE


		//GetParameter("Anime", "ScratchStartAnimeSpeed"),	// SCRATCH_START
		//GetParameter("Anime", "ScratchLoopAnimeSpeed"),	// SCRATCH_LOOP
		//GetParameter("Anime", "ScratchEndAnimeSpeed"),	// SCRATCH_END

		//GetParameter("Anime", "BreathAnimeSpeed"),	// BREATH
		//GetParameter("Anime", "BreathReadyAnimeSpeed"),	// BREATH_READY
		//GetParameter("Anime", "BreathReadyLoopAnimeSpeed"),	// BREATH_READY_LOOP
		//GetParameter("Anime", "BreathAttackAnimeSpeed"),	// BREATH_ATTACK
		//GetParameter("Anime", "BreathAttackLoopAnimeSpeed"),	// BREATH_ATTACK_LOOP

		//GetParameter("Anime", "JumpAnimeSpeed"),	// JUMP
		//GetParameter("Anime", "JumpStartAnimeSpeed"),	// JUMP_START
		//GetParameter("Anime", "JumpLoopAnimeSpeed"),	// JUMP_LOOP

		//GetParameter("Anime", "DamageAnimeSpeed"),		// DAMAGE
		//GetParameter("Anime", "DeathAnimeSpeed"),		// DEATH
		//GetParameter("Anime", "DeathFallAnimeSpeed"),		// DEATH_FALL
	};

	// ``````````````````````````ƒAƒjƒ[ƒVƒ‡ƒ“

#pragma endregion ’è”’è‹`

	// ƒƒCƒ“ˆ—``````````````
	void PlayerLoad(void)override;
	// ``````````````ƒƒCƒ“ˆ—

	// UŒ‚‚ÌƒN[ƒ‹ƒ^ƒCƒ€
	int coolTime;

	// ‚Ğ‚Á‚©‚«UŒ‚‚ÌƒtƒŒ[ƒ€À•W
	Vector3 StartFrame;
	Vector3 EndFrame;
};