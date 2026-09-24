#pragma once

#include "../../../CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

class PlayerMoveState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="DASH_SPEED_RATE">ダッシュの移動量倍率</param>
	/// <param name="DASH_STAMINA_MAX">ダッシュのスタミナの最大量（1フレームずつデクリメント）</param>
	/// <param name="accelSum">移動量の参照</param>
	/// <param name="ACCEL_MAX">横軸加速度の最大値の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="PlayIdleAnime">待機アニメーションの再生関数のポインタ</param>
	/// <param name="PlayWalkAnime">歩きアニメーションの再生関数のポインタ</param>
	/// <param name="PlayRunAnime">走りアニメーションの再生関数のポインタ</param>
	PlayerMoveState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		float DASH_SPEED_RATE, short DASH_STAMINA_MAX, float ATTENUATION,
		Vector3& accelSum, float& ACCEL_MAX, Vector3& angle, const CharacterStats& playerStats,
		const std::function<void(void)>& PlayIdleAnime,
		const std::function<void(void)>& PlayWalkAnime,
		const std::function<void(void)>& PlayRunAnime
	);

	~PlayerMoveState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

	// スタミナを返す関数
	const short& GetDashStamina(void)const { return dashStamina; }
	// 息切れかどうかを返す関数
	bool IsTired(void)const { return isTired; }

private:

#pragma region 定数

	const float MOVE_SPEED(bool isDash)const {
		return (playerStats.speedPower.Value() * 0.5f) * (isDash ? DASH_SPEED_RATE : 1.0f);
	}

	// 最大移動量
	const float MOVE_SPEED_MAX(bool isDash) const {
		return playerStats.speedPower.Value() * (isDash ? DASH_SPEED_RATE : 1.0f);
	}

	// ダッシュの移動量倍率
	const float DASH_SPEED_RATE;

	// ダッシュのスタミナの最大量（1フレームずつデクリメント）
	const short DASH_STAMINA_MAX;

	// 加速度の減衰量
	const float ATTENUATION;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 移動量参照
	Vector3& accelSum;

	// 横軸加速度の最大値の参照
	float& ACCEL_MAX;

	// 角度の参照
	Vector3& angle;

	const CharacterStats& playerStats;

	// 待機アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayIdleAnime;
	// 歩きアニメーションの再生関数のポインタ
	const std::function<void(void)> PlayWalkAnime;
	// 走りアニメーションの再生関数のポインタ
	const std::function<void(void)> PlayRunAnime;

#pragma endregion

	// ダッシュしているかどうか
	bool isDash;

	// ダッシュスタミナ
	short dashStamina;

	// 息切れ(1度スタミナが0になったかどうか)
	bool isTired;
};