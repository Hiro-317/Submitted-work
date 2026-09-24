#pragma once

#include "../../../../CharacterStateBase.h"

#include "../../../../../../Common/Vector3.h"

class TomatoBossMoveState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="MOVE_SPEED">移動速度</param>
	/// <param name="ROTATION_POW">回転量</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="GetTarget">ターゲット番号の取得</param>
	/// <param name="ResetAngle">角度のリセット</param>
	/// <param name="HeadbuttChangeState">頭突き攻撃へのステート</param>
	TomatoBossMoveState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		float MOVE_SPEED, float ROTATION_POW,
		Vector3& pos, Vector3& angle, 
		const std::vector<const Vector3*> playerPos,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> ResetAngle,
		const std::function<void(void)> HeadbuttChangeState
	);
	~TomatoBossMoveState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 定数

	// 移動量
	const float MOVE_SPEED;

	// 回転量
	const float ROTATION_POW;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;

	// プレイヤーの座標の読み取り
	const std::vector<const Vector3*> playerPos;

	const std::function<int(void)> GetTarget;
		// 角度を戻すポインタ
	const std::function<void(void)> ResetAngle;

	// 頭突きに移行するポインタ
	const std::function<void(void)> HeadbuttChangeState;

#pragma endregion

	Vector3 moveDir;

	// プレイヤーターゲット
	int target;
};