#pragma once

#include "../../../../CharacterStateBase.h"

#include "../../../../../../Common/Vector3.h"

class GrapeBossMoveState : public CharacterStateBase
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
	GrapeBossMoveState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		float MOVE_SPEED,
		Vector3& pos, Vector3& angle, 
		const std::vector<const Vector3*> playerPos,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> PlayRunAnime,
		const std::function<void(void)> KickDownChangeState
	);
	~GrapeBossMoveState()override = default;

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

#pragma region 定数

	// 移動量
	const float MOVE_SPEED;

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
	const std::function<void(void)> PlayRunAnime;

	// キックに移行するポインタ
	const std::function<void(void)> KickDownChangeState;

#pragma endregion

	Vector3 moveDir;

	// プレイヤーターゲット
	int target;
};