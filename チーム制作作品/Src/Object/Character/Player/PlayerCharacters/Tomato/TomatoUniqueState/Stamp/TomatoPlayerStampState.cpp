#include "TomatoPlayerStampState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"
#include "../../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../../../../Manager/Effect/EffectManager.h"

TomatoPlayerStampState::TomatoPlayerStampState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	TomatoPlayerStampCollOperator& collOperator,
	int COOL_TIME,
	float JUMP_POWER, float STAMP_FALL_SPEED,
	Vector3& pos, Vector3& angle, Vector3& accelSum,
	const std::function<void(void)> ChangeIsGravity,
	const std::function<bool(void)> IsGround,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	COOL_TIME(COOL_TIME),
	JUMP_POWER(JUMP_POWER), STAMP_FALL_SPEED(STAMP_FALL_SPEED),
	pos(pos), angle(angle), accelSum(accelSum),
	ChangeIsGravity(ChangeIsGravity),
	IsGround(IsGround),
	DefaultChangeState(DefaultChangeState),
	isEnd(false),
	trans()
{

}

void TomatoPlayerStampState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_3).down) {
		OwnChangeState();
	}
}

void TomatoPlayerStampState::Enter(void)
{
	ChangeIsGravity();

	// クールタイムをセット
	coolTimeCounter = COOL_TIME;

	// 角度を水平に
	angle.x = 0.0f;

	// ジャンプの初速を設定
	VelocityY = JUMP_POWER;

	// フラグの初期化
	isEnd = false;
	collOperator.ResetIsHit();
}

void TomatoPlayerStampState::Update(void)
{
	trans.pos = pos;
	trans.angle = angle;

	// 攻撃終了フラグがtrueの場合ステートを抜ける
	if (isEnd) {
		collOperator.CollOff();
		DefaultChangeState();
		return;
	}

	// ジャンプ開始
	// 徐々にジャンプの速度を落としていく
	pos.y += VelocityY;
	VelocityY -= 0.6f;

	// ジャンプ速度が0になったら
	if (VelocityY <= 0) {
		
		// 落下処理
		pos.y -= STAMP_FALL_SPEED;
		
		// ジャンプ速度変数を一応0に固定
		VelocityY = 0;

		if (IsGround()) {
			// 攻撃終了フラグをtrueに
			isEnd = true;
			// 地面に到着したら、当たり判定を発生
			collOperator.CollOn();

			EffectManager::GetIns()->CreateEffect(EFFECT_NAME::STAMP_SHOCK_WAVE, 0.0f, &trans);
			
			// 当たったら当たり判定を消す
			if (collOperator.GetIsHit()) {
				collOperator.CollOff();
			}
		}
	}
	else {
		// ジャンプ中は移動処理を行う
		JumpMove();
	}
}

void TomatoPlayerStampState::Exit(void)
{
	// すべて一応初期化
	collOperator.CollOff();
	collOperator.ResetIsHit();
	isEnd = false;
	ChangeIsGravity();
}

void TomatoPlayerStampState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}

void TomatoPlayerStampState::JumpMove(void)
{
	//コントローラーの入力を取得
	Vector3 vec = Key::GetIns().GetLeftStickVec().ToVector3XZ();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_RIGHT).now) { vec.x++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_LEFT).now) { vec.x--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_FRONT).now) { vec.z++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_BACK).now) { vec.z--; }
	}

	// 最終的に入力があれば加速度に加算する
	if (vec != 0.0f) {

		// 正規化
		vec.Normalize();

		// 移動方向をカメラで回転させる
		vec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// 移動量を加算
		accelSum += vec * JUMP_MOVE_SPEED;
	}
	// 角度を入力方向に向ける
	if (vec != 0.0f) { angle.y = atan2f(vec.x, vec.z); }
}
