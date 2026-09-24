#include "TomatoPlayerDamageState.h"

TomatoPlayerDamageState::TomatoPlayerDamageState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	Vector3& angle,
	const std::function<void(void)> InviStart,
	const std::function<void(void)> DefaultChangeState
):
	CharacterStateBase(ownChangeState, isOwnState),
	InviStart(InviStart),
	DefaultChangeState(DefaultChangeState),
	angle(angle)
{
}

// 状態遷移後1度行う初期化処理
void TomatoPlayerDamageState::Enter(void)
{
	// 無敵カウンターを設定
	InviStart();

	angle = Vector3(0.0f);
	angle.x = Deg2Rad(180.0f);
}

// 更新処理
void TomatoPlayerDamageState::Update(void)
{
	angle.y += 0.5f;

	// 元の状態に遷移
	if (angle.y > 20.0f) { DefaultChangeState(); }
}

// 状態遷移前1度行う終了処理
void TomatoPlayerDamageState::Exit(void)
{
	angle = Vector3(0.0f);
}
