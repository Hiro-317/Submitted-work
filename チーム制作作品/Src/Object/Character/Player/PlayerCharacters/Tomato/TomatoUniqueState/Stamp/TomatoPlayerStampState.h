#pragma once
#include "../../../../../CharacterStateBase.h"

#include "TomatoPlayerStampCollOperator.h"

class TomatoPlayerStampState : public CharacterStateBase
{
public:

	// コンストラクタ
	TomatoPlayerStampState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		TomatoPlayerStampCollOperator& collOperator,
		int COOL_TIME,
		float JUMP_POWER, float STAMP_FALL_SPEED, 
		Vector3& pos, Vector3& angle, Vector3& accelSum,
		const std::function<void(void)> ChangeIsGravity,
		const std::function<bool(void)> IsGround,
		const std::function<void(void)> DefaultChangeState
	);

	// デストラクタ
	~TomatoPlayerStampState()override = default;

	// 自分の状態に遷移する条件関数
	void OwnStateConditionUpdate(void)override;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

	// チャージ攻撃のUIのためのクールタイム計算関数（チャージ攻撃はここで計算して、UIのチャージ演出をここでしている）
	const int Charge(int& coolTimeCounter, const int COOL_TIME, const int CHARGE_TIME) {
		return  coolTimeCounter += COOL_TIME / CHARGE_TIME;
	}

private:

	// ジャンプ中の移動処理
	void JumpMove(void);

#pragma region 受け取る定数定義

	// ジャンプ力
	const float JUMP_POWER;

	// スタンプ攻撃の落下速度
	const float STAMP_FALL_SPEED;

	// クールタイム
	const int COOL_TIME;

	// ジャンプ中の移動速度
	const float JUMP_MOVE_SPEED = 4.0f;

	// ジャンプ速度の減衰量（重力）
	const float VELOCITY_DEC = 0.6f;

#pragma endregion 

#pragma region 受け取る用

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Vector3& angle;
	// 加速度の参照
	Vector3& accelSum;

	// 当たり判定管理クラス
	TomatoPlayerStampCollOperator& collOperator;

	// プレイヤーの重力のオンオフ切り替え関数
	const std::function<void(void)> ChangeIsGravity;

	// プレイヤーが地面に接着しているかどうか
	const std::function<bool(void)> IsGround;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;
#pragma endregion 


	// ジャンプ速度
	float VelocityY;

	// 攻撃終了判定用
	bool isEnd;


	Transform trans;
};