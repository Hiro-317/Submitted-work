#pragma once

#include "../../../CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

class PlayerDeathState : public CharacterStateBase
{
public:
	PlayerDeathState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& pos, Vector3& angle,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> PlayDeathAnime,
		const std::function<void(void)> PlayerDeathSetting,
		const std::function<void(void)> DefaultChangeState
	);

	~PlayerDeathState()override = default;

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

	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;
	// 死亡アニメーション
	const std::function<void(void)> PlayDeathAnime;
	// 死亡後の設定
	const std::function<void(void)> PlayerDeathSetting;
	// ダメージ状態終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;
};

