#pragma once

#include "../../../CharacterStateBase.h"

#include "../../../../../Common/Vector3.h"

#include "../../../../../Manager/Input/KeyManager.h"

#include "PlayerSimpleAttackCollOperator.h"

class PlayerSimpleAttackState : public CharacterStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="ATTACK_KEY">使用するキー</param>
	/// <param name="COOL_TIME">攻撃そのもののクールタイム</param>
	/// <param name="COLL_START_TIME">>攻撃の判定を発生させる開始時間（アニメーションの再生割合）</param>
	/// <param name="COLL_END_TIME">攻撃の判定を発生させる終了時間（アニメーションの再生割合）</param>
	/// <param name="ATTACK_MOVE_SPEED">攻撃中の移動速度</param>
	/// <param name="collOperator">当たり判定のオペレーターの参照</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="PlayAttackAnime">攻撃アニメーションの再生関数</param>
	/// <param name="GetAnimePlayRatio">アニメーションの再生割合を取得する関数</param>
	/// <param name="IsAnimeEnd">アニメーションの終了フラグを返す関数</param>
	/// <param name="DefaultChangeState">攻撃終了後の状態遷移関数</param>
	PlayerSimpleAttackState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,

		KEY_TYPE ATTACK_KEY,
		int COOL_TIME, float COLL_START_TIME, float COLL_END_TIME,
		float ATTACK_MOVE_SPEED,

		PlayerSimpleAttackCollOperator& collOperator,
		Vector3& pos, Vector3& angle,

		const std::function<void(void)> PlayAttackAnime,
		const std::function<float(void)> GetAnimePlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState
	);
	~PlayerSimpleAttackState() = default;

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


private:

#pragma region 定数

	// 攻撃に使うキー
	const KEY_TYPE ATTACK_KEY;

	// 攻撃そのもののクールタイム
	const int COOL_TIME;

	// 攻撃の判定を発生させる開始時間（アニメーションの再生割合）
	const float COLL_START_TIME;
	// 攻撃の判定を発生させる終了時間（アニメーションの再生割合）
	const float COLL_END_TIME;

	// 攻撃中の移動速度
	const float ATTACK_MOVE_SPEED;

#pragma endregion


#pragma region 受け取る参照変数・関数

	// 当たり判定のオペレーターの参照
	PlayerSimpleAttackCollOperator& collOperator;

	// 座標の参照
	Vector3& pos;
	// 角度の参照
	Vector3& angle;

	// 攻撃アニメーションの再生関数のポインタ
	const std::function<void(void)> PlayAttackAnime;
	// アニメーションの再生割合を取得する関数のポインタ
	const std::function<float(void)> GetAnimePlayRatio;
	// アニメーションが終了したかのフラグを取得する関数のポインタ
	const std::function<bool(void)> IsAnimeEnd;

	// 攻撃終了後の状態遷移関数のポインタ
	const std::function<void(void)> DefaultChangeState;
#pragma endregion


};