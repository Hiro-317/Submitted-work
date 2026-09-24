#include "GrapePlayerBombState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"

GrapePlayerBombState::GrapePlayerBombState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	GrapePlayerBombCollOperator& collOperator,
	const int COOL_TIME,
	Vector3& pos,
	const std::function<void(void)> PlayAnime,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	COOL_TIME(COOL_TIME),
	pos(pos),
	PlayAnime(PlayAnime),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState)
{
}

void GrapePlayerBombState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_3).down) {
		OwnChangeState();
	}
}

void GrapePlayerBombState::Enter(void)
{
	// クールタイムをセット
	coolTimeCounter = COOL_TIME;

	// アニメーションを再生
	PlayAnime();

	// 爆弾設置処理
	collOperator.LocalBombSetStart(pos);
}

void GrapePlayerBombState::Update(void)
{
	// アニメーションが終了したら、爆弾設置ステート終了
	if (IsAnimeEnd()) {
		DefaultChangeState();
	}
}

void GrapePlayerBombState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}
