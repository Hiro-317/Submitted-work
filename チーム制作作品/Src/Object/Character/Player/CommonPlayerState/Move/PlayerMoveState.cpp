#include "PlayerMoveState.h"

#include "../../../../../Manager/Input/KeyManager.h"
#include "../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../Character/CharacterStatsDefine.h"

PlayerMoveState::PlayerMoveState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float DASH_SPEED_RATE, short DASH_STAMINA_MAX, float ATTENUATION,
	Vector3& accelSum, float& ACCEL_MAX, Vector3& angle, const CharacterStats& playerStats,
	const std::function<void(void)>& PlayIdleAnime,
	const std::function<void(void)>& PlayWalkAnime,
	const std::function<void(void)>& PlayRunAnime
) :
	CharacterStateBase(ownChangeState, isOwnState),
	DASH_SPEED_RATE(DASH_SPEED_RATE), DASH_STAMINA_MAX(DASH_STAMINA_MAX), ATTENUATION(ATTENUATION),
	accelSum(accelSum), ACCEL_MAX(ACCEL_MAX), angle(angle), playerStats(playerStats),
	PlayIdleAnime(PlayIdleAnime),
	PlayWalkAnime(PlayWalkAnime),
	PlayRunAnime(PlayRunAnime),

	isDash(false), dashStamina(DASH_STAMINA_MAX), isTired(false)
{
}

void PlayerMoveState::OwnStateConditionUpdate(void)
{
	// 移動にかかわるキーのダウントリガーを条件にする
	if (
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_RIGHT).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_LEFT).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_FRONT).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_BACK).down ||
		Key::GetIns().GetInfo(KEY_TYPE::PLAYER_DASH).down
		) {
		// 自分の状態に遷移
		OwnChangeState();
	}
}

void PlayerMoveState::Enter(void)
{
}

void PlayerMoveState::Update(void)
{
	// コントローラーの入力を取得
	Vector3 vec = Key::GetIns().GetLeftStickVec().ToVector3XZ();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_RIGHT).now) { vec.x++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_LEFT).now) { vec.x--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_FRONT).now) { vec.z++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_BACK).now) { vec.z--; }
	}

	// ダッシュフラグを立てる(移動中じゃなければ、ダッシュフラグは変えない)
	isDash = (isTired) ? false : Key::GetIns().GetInfo(KEY_TYPE::PLAYER_DASH).now && vec != 0.0f;

	// 移動量の最大値を更新する
	ACCEL_MAX = MOVE_SPEED_MAX(isDash);
	// もし加速度の合計が最大値を超えていたら、その値から1フレーム分減衰した値を最大値にする
	if (ACCEL_MAX < accelSum.Length()) { ACCEL_MAX = accelSum.Length() - ATTENUATION; }

	// 最終的に入力があれば加速度に加算する
	if (vec != 0.0f) { 

		// 正規化
		vec.Normalize();

		// 移動方向をカメラで回転させる
		vec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// 移動量を加算
		accelSum += vec * MOVE_SPEED(isDash);

		// ダッシュスタミナを更新 / アニメーションを更新
		if (isDash) {

			// ダッシュしているときはスタミナを減らす
			if (--dashStamina < 0) {
				dashStamina = 0;

				// 息切れ
				isTired = true;
			}

			// ダッシュしているときは走るアニメーションにする
			PlayRunAnime();
		}
		else {
			// ダッシュしていないときは歩くアニメーションにする
			PlayWalkAnime();
		}
	}
	else {
		// 待機アニメーションにする
		PlayIdleAnime();
	}

	// 角度を入力方向に向ける
	if (vec != 0.0f) { angle.y = atan2f(vec.x, vec.z); }
}

void PlayerMoveState::Exit(void)
{
	accelSum = 0.0f; // 加速度をリセット
}

void PlayerMoveState::AlwaysUpdate(void)
{
	// ダッシュしていないときはスタミナを回復させる
	if (!isDash) {
		if (++dashStamina > DASH_STAMINA_MAX) {
			dashStamina = DASH_STAMINA_MAX;

			// 息切れ回復
			isTired = false;
		}
	}
}