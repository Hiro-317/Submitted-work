#pragma once

#include "../../../../CharacterStateBase.h"

#include "../../../../../../Common/Vector3.h"

class TomatoBossIdleState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="CoolTime">クールタイムの取得</param>
	/// <param name="GetTarget">ターゲット番号の取得</param>
	/// <param name="HeadbuttChangeState">頭突き攻撃へのステート</param>
	/// <param name="MoveChangeState">移動へのステート</param>
	/// <param name="StampChangeState">スタンプ攻撃へのステート</param>
	/// <param name="TackleChangeState">タックル攻撃へのステート</param>
	/// <param name="hitRock">岩に当たっているか</param>
	/// <param name="rockReset">岩に当たっているかのリセット</param>
	TomatoBossIdleState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& pos, const std::vector<const Vector3*>playerPos,
		const std::function<int(void)> CoolTime,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> HeadbuttChangeState,
		const std::function<void(void)> MoveChangeState,
		const std::function<void(void)> StampChangeState,
		const std::function<void(void)> TackleChangeState,
		const std::function<bool(void)> hitRock,
		const std::function<void(void)> rockReset
	);
	~TomatoBossIdleState()override = default;

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

	// プレイヤーの座標の読み取り
	const std::vector<const Vector3*> playerPos;

	const std::function<int(void)> GetTarget;
	// クールタイムの参照
	const std::function<int(void)> CoolTime;

	// 頭突きへ
	const std::function<void(void)> HeadbuttChangeState;
	// 移動へ
	const std::function<void(void)> MoveChangeState;
	// スタンプへ
	const std::function<void(void)> StampChangeState;
	// 突進へ
	const std::function<void(void)> TackleChangeState;

	// 岩との当たり判定
	const std::function<bool(void)> hitRock;
	// 岩との当たり判定の初期化
	const std::function<void(void)> rockReset;

#pragma endregion


#pragma region 定数定義

	// 確率の全体
	static constexpr int RANDOM = 10000;

	// それぞれの確率
	static constexpr int HEADBUTT_LUCK = 4000;
	static constexpr int STAMP_LUCK = 8000;
	static constexpr int RELOOT_LUCK = 9000;

	// 移動に移る距離
	static constexpr float DISTANCE = 400.0f;

#pragma endregion

	// カウント
	int cnt;

	// プレイヤーターゲット
	int target;
};