#pragma once

#include "../../../Character/CharacterStateBase.h"

struct Vector3;

class BossSelectSkyShipMoveState : public CharacterStateBase
{
public:

	BossSelectSkyShipMoveState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,

		float MOVE_SPEED,

		Vector3& accelSum,
		Vector3& angle
	);

	~BossSelectSkyShipMoveState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;

	// 更新処理
	void Update(void)override;

	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:
	// 移動速度
	const float MOVE_SPEED;

	// 移動量参照
	Vector3& accelSum;

	// 角度の参照
	Vector3& angle;
};