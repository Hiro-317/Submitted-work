#include "TomatoPlayerTackleState.h"

#include "../../../../../../../Manager/Input/KeyManager.h"
#include "../../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../../../../Manager/Effect/EffectManager.h"

TomatoPlayerTackleState::TomatoPlayerTackleState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	TomatoPlayerTackleCollOperator& collOperator,
	int COOL_TIME,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle,
	const std::function<void(void)> ChargeAttackBuff,
	const std::function<void(void)> ChargeAttackBuffDelete,
	const std::function<void(void)> DefaultChangeState
	) : 
	CharacterStateBase(ownChangeState, isOwnState),
	collOperator(collOperator),
	COOL_TIME(COOL_TIME),
	MOVE_SPEED(MOVE_SPEED),
	ROTATION_POW(ROTATION_POW),
	pos(pos), angle(angle),
	ChargeAttackBuff(ChargeAttackBuff),
	ChargeAttackBuffDelete(ChargeAttackBuffDelete),
	DefaultChangeState(DefaultChangeState),
	timeCounter(0),
	chargeCounter(0),
	trans()
{

}

void TomatoPlayerTackleState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// ステート遷移処理
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_2).down) {
		OwnChangeState();
	}
}

void TomatoPlayerTackleState::Enter(void)
{
	//	当たったかどうかのフラグをリセット
	collOperator.ResetIsHit();

	// 突進方向の取得
	moveDir.x = sinf(angle.y);
	moveDir.z = cosf(angle.y);
	moveDir.Normalize();

	// X軸の角度をゼロに初期化(回転量で突進の開始を判断しているから、初期化しておく)
	angle.x = 0.0f;

	// カウンターの初期化
	timeCounter = COUNT_MAX;
	coolTimeCounter = chargeCounter;

	EffectManager::GetIns()->CreateEffect(EFFECT_NAME::TACKLE_CHARGE, 0.0f, &trans);
}

void TomatoPlayerTackleState::Update(void)
{
	coolTimeCounter = chargeCounter;
	trans.pos = pos;
	trans.angle.y = angle.y;

	// カウンターがゼロになったら終了
	if (timeCounter <= 0) {
		DefaultChangeState();
		return;
	}

	// チャージ終了したら突進を開始
	if (chargeCounter >= COOL_TIME) {
		Tackle();
	}

	// ボタンを押し続けている場合攻撃をチャージする
	if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_SKILL_2).now) {
		// これはスキルのUIをチャージ用に動かすための特殊処理処理
		Charge(chargeCounter, COOL_TIME, CHARGE_TIME);

		// 回転スタート
		angle.x += ROTATION_POW;

		// チャージの効果音を再生
		SoundManager::GetIns().Play("TackleCharge");

		// コントローラーの入力を取得
		moveDir = Key::GetIns().GetLeftStickVec().ToVector3XZ();

		// 入力がなければ次にキーボードの入力を取得
		if (moveDir == 0.0f) {
			if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_RIGHT).now) { moveDir.x++; }
			if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_LEFT).now) { moveDir.x--; }
			if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_FRONT).now) { moveDir.z++; }
			if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_BACK).now) { moveDir.z--; }
		}

		// 正規化
		moveDir.Normalize();

		// 移動方向をカメラで回転させる
		moveDir.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// 角度を入力方向に向ける
		if (moveDir != 0.0f) { angle.y = atan2f(moveDir.x, moveDir.z); }

		moveDir.x = sinf(angle.y);
		moveDir.z = cosf(angle.y);
		moveDir.Normalize();
	}
	else if(chargeCounter < COOL_TIME) {
		Tackle();
	}
}

void TomatoPlayerTackleState::Exit(void)
{
	// 念のためすべて初期化
	timeCounter = 0;
	collOperator.CollOff();
	collOperator.ResetIsHit();

	// スキルのクールタイムをセット
	coolTimeCounter = COOL_TIME;
	chargeCounter = 0;

	// チャージ攻撃のバフを削除
	ChargeAttackBuffDelete();

	EffectManager::GetIns()->StopEffect(EFFECT_NAME::TACKLE_CHARGE);
}

void TomatoPlayerTackleState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}

void TomatoPlayerTackleState::Tackle(void)
{
	// チャージの効果音を消して、タックルの効果音を再生
	SoundManager::GetIns().Stop("TackleCharge");
	SoundManager::GetIns().Play("TackleMove");

	EffectManager::GetIns()->StopEffect(EFFECT_NAME::TACKLE_CHARGE);

	// 突進中も回転を続ける
	angle.x += ROTATION_POW;
	// カウントを開始
	timeCounter--;
	// 前方に向かって突進
	pos += moveDir * MOVE_SPEED;

	// チャージできていたら、バフをかけて攻撃力を増やす
	if (chargeCounter >= COOL_TIME) {
		ChargeAttackBuff();
	}

	// 当たり判定をオンにする
	collOperator.CollOn();

	// 当たったらタイマーを強制的に終了し、次の処理へ
	if (collOperator.GetIsHit()) {
		timeCounter = 0;
		collOperator.CollOff();
	}
}
