#include "PlayerSimpleAttackState.h"

#include "../../../../../Manager/Sound/SoundManager.h"

PlayerSimpleAttackState::PlayerSimpleAttackState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,

	KEY_TYPE ATTACK_KEY,
	int COOL_TIME, float COLL_START_TIME, float COLL_END_TIME,
	float ATTACK_MOVE_SPEED,

	PlayerSimpleAttackCollOperator& collOperator,
	Vector3& pos, Vector3& angle,

	const std::function<void(void)> PlayAttackAnime,
	const std::function<float(void)> GetAnimePlayRatio,
	const std::function<bool(void)> IsAnimeEnd,
	const std::function<void(void)> DefaultChangeState
) :
	CharacterStateBase(ownChangeState, isOwnState),

	ATTACK_KEY(ATTACK_KEY),
	COOL_TIME(COOL_TIME), COLL_START_TIME(COLL_START_TIME), COLL_END_TIME(COLL_END_TIME),
	ATTACK_MOVE_SPEED(ATTACK_MOVE_SPEED),

	collOperator(collOperator),

	pos(pos),angle(angle),

	PlayAttackAnime(PlayAttackAnime),
	GetAnimePlayRatio(GetAnimePlayRatio),
	IsAnimeEnd(IsAnimeEnd),
	DefaultChangeState(DefaultChangeState)
{
}

void PlayerSimpleAttackState::OwnStateConditionUpdate(void)
{
	// 攻撃のクールタイム中は状態遷移しない
	if (coolTimeCounter > 0) { return; }

	// 攻撃キーのダウントリガーで状態遷移
	if (Key::GetIns().GetInfo(ATTACK_KEY).down) { OwnChangeState(); }

}

void PlayerSimpleAttackState::Enter(void)
{
	// 攻撃アニメーションを再生する
	PlayAttackAnime();

	SoundManager::GetIns().Play("PlayerSimpleAttack");

	// 攻撃のクールタイムをセットする
	coolTimeCounter = COOL_TIME;

	// 攻撃対象の探索を開始する
	collOperator.TargetSerch();

	// 攻撃のヒット管理のフラグをリセット
	collOperator.ResetIsHit();
}

void PlayerSimpleAttackState::Update(void)
{
	// 一旦攻撃判定をオフにしておく
	collOperator.CollOff();


	// アニメーションの再生割合を取得する
	float animePlayRate = GetAnimePlayRatio();

	if (animePlayRate < COLL_START_TIME) {
		// 攻撃判定開始前

		// 移動ベクトル
		Vector3 moveVec = {};

		if (collOperator.GetIsFindAttackTarget()) {
			// 攻撃対象が見つかっていれば、攻撃対象に向かって移動する
			moveVec = collOperator.GetTargetPos() - pos;

			// 攻撃対象に向かうように角度を変える
			angle.y = atan2f(moveVec.x, moveVec.z);
		}
		else {
			// 攻撃対象が見つかっていなければ、角度の方向に移動する
			moveVec = Vector3::XZonly(sinf(angle.y), cosf(angle.y));
		}

		// 最終的な移動ベクトルで移動する
		pos += moveVec.Normalized() * ATTACK_MOVE_SPEED;
	}
	else if (animePlayRate < COLL_END_TIME) {
		// 攻撃判定中
		collOperator.CollOn();
	}
	else {
		// 攻撃判定終了後

		// 自身の遷移条件を呼び出し、キー入力で次段に繋がるようにしておく
		this->OwnStateConditionUpdate();

		// アニメーションが終了していたら入力関係なく指定の通常状態に遷移させる
		if (IsAnimeEnd()) { DefaultChangeState(); }
	}
}

void PlayerSimpleAttackState::Exit(void)
{
	// 当たり判定をオフにする
	collOperator.CollOff();

	// 探索情報をリセットする
	collOperator.ResetTarget();
}

void PlayerSimpleAttackState::AlwaysUpdate(void)
{
	// 自身の状態でないときは、攻撃のクールタイムを減らす
	if (!IsOwnState()) {
		// 攻撃のクールタイムを減らす
		if (coolTimeCounter > 0) { coolTimeCounter--; }
	}
}