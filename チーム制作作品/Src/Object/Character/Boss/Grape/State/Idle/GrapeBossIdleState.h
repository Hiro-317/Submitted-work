#pragma once

#include "../../../../CharacterStateBase.h"

#include "../../../../../../Common/Vector3.h"

class GrapeBossIdleState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="CoolTime">クールタイムの取得</param>
	/// <param name="GetTarget">ターゲット番号の取得</param>
	/// <param name="PlayIdleAnim">アイドルアニメーションの再生</param>
	/// <param name="PlayWalkAnim">歩きアニメーションの再生</param>
	/// <param name="MoveChangeState">歩くへのステート</param>
	/// <param name="KickDownChangeState">かかと落としへのステート</param>
	/// <param name="StraightChangeState">直線投擲へのステート</param>
	/// <param name="StampChangeState">スタンプ攻撃へのステート</param>
	/// <param name="SingleChangeState">一発攻撃へのステート</param>
	/// <param name="StalkerChangeState">追従攻撃へのステート</param>
	/// <param name="RandomChangeState">ランダム攻撃へのステート</param>
	GrapeBossIdleState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& pos, Vector3& angle, 
		const std::vector<const Vector3*>playerPos,
		const std::function<int(void)> CoolTime,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> PlayIdleAnim,
		const std::function<void(void)> PlayWalkAnim,
		const std::function<void(void)> MoveChangeState,
		const std::function<void(void)> KickDownChangeState,
		const std::function<void(void)> StraightChangeState,
		const std::function<void(void)> StampChangeState,
		const std::function<void(void)> SingleChangeState,
		const std::function<void(void)> StalkerChangeState,
		const std::function<void(void)> RandomChangeState
	);
	~GrapeBossIdleState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Vector3& angle;

	// プレイヤーの座標の読み取り
	const std::vector<const Vector3*> playerPos;

	const std::function<int(void)> GetTarget;
	// クールタイムの参照
	const std::function<int(void)> CoolTime;

	// アイドルアニメーション
	const std::function<void(void)> PlayIdleAnim;
	// 回転時アニメーション
	const std::function<void(void)> PlayWalkAnim;

	// 移動へ
	const std::function<void(void)> MoveChangeState;
	// キックへ
	const std::function<void(void)> KickDownChangeState;
	// 投擲へ
	const std::function<void(void)> StraightChangeState;
	// スタンプへ
	const std::function<void(void)> StampChangeState;
	// 追従へ
	const std::function<void(void)> SingleChangeState;
	// たくさんの追従へ
	const std::function<void(void)> StalkerChangeState;
	// ランダムへ
	const std::function<void(void)> RandomChangeState;

#pragma endregion

#pragma region 定数定義

	// 確率の全体
	static constexpr int RANDOM = 12000;

	// それぞれの確率
	static constexpr int KICKDOWN_LUCK = 3000;
	static constexpr int STRAIGHT_LUCK = 4000;
	static constexpr int STAMP_LUCK = 6000;
	static constexpr int SINGLE_LUCK = 9000;
	static constexpr int STALKER_LUCK = 11000;
	static constexpr int RELOOT_LUCK = 4500;

	// 移動に移る距離
	static constexpr float DISTANCE = 400.0f;

	// 回らない範囲
	const float NOT_ROT = Deg2Rad(15.0f);
	// 一度に回る角度
	const float ROT_POW = Deg2Rad(2.0f);

#pragma endregion

	// カウント
	int cnt;

	// プレイヤーターゲット
	int target;

	// アングル合わせるフラグ
	bool angleFlg;
};