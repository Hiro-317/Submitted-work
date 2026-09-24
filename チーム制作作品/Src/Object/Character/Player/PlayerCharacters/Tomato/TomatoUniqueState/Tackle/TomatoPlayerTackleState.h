#pragma once
#include "../../../../../CharacterStateBase.h"

#include "TomatoPlayerTackleCollOperator.h"

class TomatoPlayerTackleState : public CharacterStateBase
{
public:

	// コンストラクタ
	TomatoPlayerTackleState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		TomatoPlayerTackleCollOperator& collOperator,
		int COOL_TIME,
		float MOVE_SPEED, float ROTATION_POW,
		Vector3& pos, Vector3& angle,
		const std::function<void(void)> ChargeAttackBuff,
		const std::function<void(void)> ChargeAttackBuffDelete,
		const std::function<void(void)> DefaultChangeState
	);

	// デストラクタ
	~TomatoPlayerTackleState()override = default;

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

	// タックル処理
	void Tackle(void);

#pragma region 定数定義

	// 移動量
	const float MOVE_SPEED;

	// 回転量
	const float ROTATION_POW;

	// クールタイム
	const int COOL_TIME;

	// このスタート角度まで回転したら、突進をスタートする
	static constexpr float TACKLE_START_ANGLE = 20.0f;

	// 突進してから終了までのカウントタイム
	static constexpr int COUNT_MAX = 20;

	// チャージ時間
	static constexpr int CHARGE_TIME = 60;

#pragma endregion 

#pragma region 変数定義

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;

	TomatoPlayerTackleCollOperator& collOperator;

	// 向いている方向
	Vector3 moveDir;

	// タックル終了までのカウント用
	int timeCounter;

	// チャージ用カウンタ
	int chargeCounter;

	// チャージした時、ダメージを増やすための関数のポインタ
	const std::function<void(void)> ChargeAttackBuff;
	const std::function<void(void)> ChargeAttackBuffDelete;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

#pragma endregion 

	Transform trans;

};