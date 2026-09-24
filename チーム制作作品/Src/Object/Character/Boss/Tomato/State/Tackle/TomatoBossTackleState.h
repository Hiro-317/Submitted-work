#pragma once

#include "../../../../CharacterStateBase.h"

#include "TomatoTackleCollOperator.h"

#include "../../../../../../Common/Vector3.h"

class TomatoBossTackleState : public CharacterStateBase
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
	/// <param name="trans">情報の読み取り</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="ResetAngle">角度を戻す</param>
	/// <param name="DeleteColl">XZのコライダを消す</param>
	/// <param name="ReviveColl">XZのコライダを復活させる</param>
	/// <param name="DefaultChangeState">戻すステート</param>
	TomatoBossTackleState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		float MOVE_SPEED, float ROTATION_POW,
		Vector3& pos, Vector3& angle, const Transform* trans,
		const std::vector<const Vector3*> playerPos,
		TomatoTackleCollOperator* collOperator,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> ResetAngle,
		const std::function<void(void)> DeleteColl,
		const std::function<void(void)> ReviveColl,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> SetCoolTime
	);
	~TomatoBossTackleState()override = default;

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

	// チャージ
	static constexpr int CHARGE_POW = 180;

	// 強制突進タイム
	static constexpr int FORCE_MOVE_TIME = 200;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// ボスの座標
	Vector3& pos;

	// ボスの角度
	Vector3& angle;

	// ボスの情報の読み取り
	const Transform* trans;

	// プレイヤーの座標の読み取り
	const std::vector<const Vector3*> playerPos;

	// コリジョンオペレーター
	TomatoTackleCollOperator* collOperator;

	// ターゲットを得る
	const std::function<int(void)> GetTarget;

	// 角度を戻す
	const std::function<void(void)> ResetAngle;

	// XZのコライダを消す
	const std::function<void(void)> DeleteColl;

	// XZのコライダを復活させる
	const std::function<void(void)> ReviveColl;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

	// クールタイムの設定
	const std::function<void(void)> SetCoolTime;

#pragma endregion

	Vector3 moveDir;

	float rotPow;

	int time;

	// プレイヤーターゲット
	int target;
};