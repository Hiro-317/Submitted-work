#include "BossSelectSkyShipMoveState.h"

#include "../../../../Common/Vector3.h"

#include "../../../../Manager/Input/KeyManager.h"

#include "../../../../Manager/Camera/CurrentCamera.h"

BossSelectSkyShipMoveState::BossSelectSkyShipMoveState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,

	float MOVE_SPEED,

	Vector3& accelSum,
	Vector3& angle) :
	CharacterStateBase(ownChangeState, isOwnState),

	MOVE_SPEED(MOVE_SPEED),

	accelSum(accelSum),
	angle(angle)
{
}

void BossSelectSkyShipMoveState::Enter(void)
{

}

void BossSelectSkyShipMoveState::Update(void)
{
	// コントローラーの入力を取得
	Vector3 vec = Key::GetIns().GetLeftStickVec(false).ToVector3XZ();

	// 入力がなければ次にキーボードの入力を取得
	if (vec == 0.0f) {
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_RIGHT).now) { vec.x++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_LEFT).now) { vec.x--; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_FRONT).now) { vec.z++; }
		if (Key::GetIns().GetInfo(KEY_TYPE::PLAYER_MOVE_BACK).now) { vec.z--; }
		vec.Normalize();
	}

	// 最終的に入力があれば加速度に加算する
	if (vec != 0.0f) {

		// 移動方向をカメラで回転させる
		vec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetAngle().y));

		// 移動速度をかける
		vec *= MOVE_SPEED;

		// 移動量を加算
		accelSum += vec;
	}

	// 角度を入力方向に向ける
	if (vec != 0.0f) { angle.y = atan2f(vec.x, vec.z); }
}

void BossSelectSkyShipMoveState::Exit(void)
{

}