#pragma once

#include "../../../../CharacterStateBase.h"


class BananaBossDeathState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="ROTATION_POW">回転量</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	BananaBossDeathState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		const std::function<void(void)>& PlayDeath,
		const std::function<void(void)> IsDeath
	);
	~BananaBossDeathState()override = default;

	//// 自分の状態に遷移する条件関数
	//void OwnStateConditionUpdate(void);

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:
#pragma region 参照関数

	// 死亡判定移行
	const std::function<void(void)> PlayDeath;
	const std::function<void(void)> IsDeath;

#pragma endregion 参照関数

#pragma region 定数定義

	static constexpr int CHANGE_COUNT = 120;

#pragma endregion 定数定義

	int cnt;
};