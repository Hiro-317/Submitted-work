#pragma once

#include "../../../../CharacterStateBase.h"

#include "TomatoStampCollOperator.h"

#include "../../../../../../Common/Vector3.h"
#include "../../../../../Common/Collider/ColliderBase.h"

class TomatoBossStampState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="collOperator">当たり判定オペレーターの参照</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="isGround">地面に当たっているか</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="GetTarget">ターゲット番号取得</param>
	/// <param name="DefaultChangeState">攻撃後変わるステート</param>
	/// <param name="offCollider">ボス自体のコライダの消滅</param>
	/// <param name="onCollider">ボス自体のコライダの再生</param>
	/// <param name="SetCoolTime">クールタイムの設定</param>
	TomatoBossStampState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		TomatoStampCollOperator* collOperator,
		Vector3& pos, Vector3& angle, const bool& isGround,
		const std::vector<const Vector3*>playerPos,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> offCollider,
		const std::function<void(void)> onCollider,
		const std::function<void(void)> SetCoolTime
	);

	~TomatoBossStampState()override = default;

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

	// 飛ぶ強さ
	static constexpr float JUMP_POW = 29.0f;

	// 目標までの到達時間(割合)
	static constexpr float TIME_RATE = 18.0f;

	// 攻撃の発生持続時間
	static constexpr int ATTACK_DURATION = 5;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;

	// 設置判定の読み取り
	const bool& isGround;

	// コリジョンオペレーター
	TomatoStampCollOperator* collOperator;

	// プレイヤーの座標の取得
	const std::vector<const Vector3*>playerPos;

	// 攻撃対象のプレイヤー番号
	const std::function<int(void)> GetTarget;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;

	// 攻撃時のエネミー自体のコライダーを消すようにのポインター
	const std::function<void(void)> offCollider;

	// 攻撃時のエネミー自体のコライダーをつけるようにのポインター
	const std::function<void(void)> onCollider;

	// クールタイムの設定
	const std::function<void(void)> SetCoolTime;

#pragma endregion

	// 攻撃座標
	Vector3 attackPos;

	// 攻撃する距離
	Vector3 attackDistRate;

	// 今の攻撃タイム
	int nowAttackTime;

	// 攻撃発生中フレーム
	int attackCnt;

	// 攻撃中かフラグ
	bool isAttack;

	// プレイヤーターゲット
	int target;

	// 直前の座標
	float prevPos;

};