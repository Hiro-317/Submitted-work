#include "PlayerSingleModifierState.h"

#include "../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../Manager/Net/NetWorkManager.h"

#include "PlayerSingleModifierCollOperator.h"


PlayerSingleModifierState::PlayerSingleModifierState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,

	PlayerSingleModifierCollOperator& collOperator,

	float COLL_START_TIME, KEY_TYPE HEAL_KEY,
	int COOL_TIME,
	const std::function<void(void)> PlayHealAnime,
	const std::function<float(void)> GetAnimePlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),

	collOperator(collOperator),
	COLL_START_TIME(COLL_START_TIME),
	HEAL_KEY(HEAL_KEY),
	COOL_TIME(COOL_TIME),
	PlayHealAnime(PlayHealAnime),
	GetAnimePlayRatio(GetAnimePlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	isModifierEnd(false)
{
}

void PlayerSingleModifierState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Key::GetIns().GetInfo(HEAL_KEY).down) { OwnChangeState(); }
}

void PlayerSingleModifierState::Enter(void)
{
	coolTimeCounter = COOL_TIME;
	collOperator.CollOff();
	collOperator.ResetIsHit();
	isModifierEnd = false;
	PlayHealAnime();
}

void PlayerSingleModifierState::Update(void)
{
	if (!isModifierEnd && GetAnimePlayRatio() > COLL_START_TIME) {
		collOperator.CollOn();
		isModifierEnd = true;
	}

	if (IsAnimeEnd()) { DefaultChangeState(); }
}

void PlayerSingleModifierState::Exit(void)
{
	collOperator.CollOff();
	collOperator.ResetIsHit();
	isModifierEnd = false;
}

void PlayerSingleModifierState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}