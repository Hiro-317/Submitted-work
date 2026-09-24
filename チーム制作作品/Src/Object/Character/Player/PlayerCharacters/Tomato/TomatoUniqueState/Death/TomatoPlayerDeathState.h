#pragma once

#include "../../../../../CharacterStateBase.h"

#include "../../../../../../../Common/Vector3.h"

class TomatoPlayerDeathState : public CharacterStateBase
{
public:
	TomatoPlayerDeathState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& pos, Vector3& angle,
		const std::function<void(void)> PlayerDeathSetting,
		const std::function<void(void)> PlayerIsDeathTrue,
		const std::function<void(void)> DefaultChangeState
	);

	~TomatoPlayerDeathState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

private:

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Vector3& angle;

	// 死亡後の設定
	const std::function<void(void)> PlayerDeathSetting;
	// 死亡フラグをtrueにする関数のポインタ
	const std::function<void(void)> PlayerIsDeathTrue;
	// ダメージ状態終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

	float animeCounter;
};

