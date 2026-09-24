#include "PlayerDamageState.h"

PlayerDamageState::PlayerDamageState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const std::function<void(void)> PlayDamageAnime,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void()> InviStart,
	const std::function<void()> DefaultChangeState
) :
	CharacterStateBase(ownChangeState,isOwnState),
	PlayDamageAnime(PlayDamageAnime),
	IsAnimeEnd(IsAnimeEnd),
	InviStart(InviStart),
	DefaultChangeState(DefaultChangeState)
{
}

// 状態遷移後1度行う初期化処理
void PlayerDamageState::Enter(void)
{
	// アニメーション再生
	PlayDamageAnime();

	// 無敵カウンターを設定
	InviStart();
}

// 更新処理
void PlayerDamageState::Update(void)
{
	// 元の状態に遷移
	if (IsAnimeEnd()) { DefaultChangeState(); }
}

// 状態遷移前1度行う終了処理
void PlayerDamageState::Exit(void)
{
}
