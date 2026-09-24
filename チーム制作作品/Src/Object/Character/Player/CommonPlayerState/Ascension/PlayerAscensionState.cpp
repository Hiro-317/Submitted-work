#include "PlayerAscensionState.h"

PlayerAscensionState::PlayerAscensionState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const std::function<void()> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	DefaultChangeState(DefaultChangeState)
{
}

// 状態遷移後1度行う初期化処理
void PlayerAscensionState::Enter(void)
{
}

// 更新処理
void PlayerAscensionState::Update(void)
{
}

// 状態遷移前1度行う終了処理
void PlayerAscensionState::Exit(void)
{
}