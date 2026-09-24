#pragma once

#include "../../../../CharacterStateBase.h"

#include "TomatoHeadbuttCollOperator.h"
#include "../../../../../../Common/Vector3.h"

class TomatoBossHeadbuttState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="MOVE_SPEED"></param>
	/// <param name="ATTACK_TIME"></param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="collOperator">当たり判定オペレータの参照</param>
	/// <param name="GetTarget">ターゲット番号の取得</param>
	/// <param name="DeleteColl">ボス自体のコライダの消滅</param>
	/// <param name="ReviveColl">ボス自体のコライダの再生</param>
	/// <param name="DefaultChangeState">攻撃後のステート</param>
	/// <param name="SetCoolTime">クールタイムの設定</param>
	TomatoBossHeadbuttState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		const float MOVE_SPEED, const float ATTACK_TIME,
		Vector3& pos, Vector3& angle,
		const std::vector<const Vector3*> playerPos,
		TomatoHeadbuttCollOperator* collOperator,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> DeleteColl,
		const std::function<void(void)> ReviveColl,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> SetCoolTime
	);
	~TomatoBossHeadbuttState()override = default;

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

	const float MOVE_SPEED;

	const float ATTACK_TIME;

	static constexpr int START_CNT = 100;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;

	// プレイヤーの座標の読み取り
	const std::vector<const Vector3*> playerPos;

	// コリジョンオペレーターのポインタ
	TomatoHeadbuttCollOperator* collOperator;

	const std::function<int(void)> GetTarget;
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

	float time;

	// プレイヤーターゲット
	int target;
};