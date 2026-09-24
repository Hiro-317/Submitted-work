#include "PlayerDodgeState.h"

PlayerDodgeState::PlayerDodgeState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	KEY_TYPE DODGE_KEY,
	int COOL_TIME, float DODGE_MOVE_SPEED, 
	float INVI_START_TIME, float INVI_END_TIME,
	Vector3& pos, Vector3& angle,
	const std::function<void(void)> PlayDodgeAnime,
	const std::function<float(void)> GetAnimePlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(unsigned char)> SetInviCounter,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	DODGE_KEY(DODGE_KEY),
	COOL_TIME(COOL_TIME), 
	DODGE_MOVE_SPEED(DODGE_MOVE_SPEED),
	INVI_START_TIME(INVI_START_TIME), INVI_END_TIME(INVI_END_TIME),
	pos(pos), angle(angle),
	PlayDodgeAnime(PlayDodgeAnime),
	GetAnimePlayRatio(GetAnimePlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState),
	SetInviCounter(SetInviCounter)
{
}

void PlayerDodgeState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Key::GetIns().GetInfo(DODGE_KEY).down) {
		OwnChangeState();
		Enter();
	}
}

void PlayerDodgeState::Enter(void)
{
	// アニメーションの再生
	PlayDodgeAnime();

	// 攻撃のクールタイムをセットする
	coolTimeCounter = COOL_TIME;
}

void PlayerDodgeState::Update(void)
{
	// 無敵判定終了前移動させる
	if (GetAnimePlayRatio() <= INVI_END_TIME) {
		// 回避の移動処理
		pos += Vector3::XZonly(sinf(angle.y), cosf(angle.y)).Normalized() * DODGE_MOVE_SPEED;
	}

	// アニメーションが終了していたら入力関係なく指定の通常状態に遷移させる
	if (IsAnimeEnd()) { DefaultChangeState(); }
}

void PlayerDodgeState::Exit(void)
{

}

void PlayerDodgeState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}
