#pragma once

#include "../../../../CharacterStateBase.h"

#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Common/Vector3.h"

class GrapeBossStalkerState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="bombType">攻撃種のクラス取得</param>
	/// <param name="GetTarget">ターゲット番号の取得</param>
	/// <param name="PlayAttackAnim">攻撃アニメーションの再生</param>
	/// <param name="GetAnimPlayRatio">攻撃アニメーションの再生割合取得</param>
	/// <param name="IsAnimeEnd">アニメーションが終了しているか</param>
	/// <param name="DefaultChangeState">攻撃後遷移ステート</param>
	/// <param name="SetCoolTime">クールタイムの設定</param>
	GrapeBossStalkerState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		const std::vector<const Vector3*> playerPos,
		BombType* bombType,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> PlayAttackAnim,
		const std::function<float(void)> GetAnimPlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> SetCoolTime
	);
	~GrapeBossStalkerState()override = default;

	// 状態遷移後1度行う初期化処理
	void Enter(void)override;
	// 更新処理
	void Update(void)override;
	// 状態遷移前1度行う終了処理
	void Exit(void)override;

	// 状態関係なく呼び出す処理
	void AlwaysUpdate(void)override;

private:
#pragma region 定数定義

	// 爆弾の発生高さ
	static constexpr float HEIGHT = 1000.0f;

	// 爆弾のランダム発生距離
	static constexpr int RADIUS = 300;

	// 予測線用のローカル座標
	const Vector3 FOOT_VIEW_POS = Vector3(-23.16f, 0.0f, -85.34f);

	// 弾の間隔時間
	const int DISP_TIME = 15;

#pragma endregion

#pragma region 受け取る参照変数・関数

	const std::vector<const Vector3*> playerPos;

	// ボムの参照
	BombType* bombType;

	// プレイヤーのターゲット番号
	const std::function<int(void)> GetTarget;

	// アタックアニメーションの再生
	const std::function<void(void)> PlayAttackAnim;
	const std::function<void(void)> IdleAttackAnim;

	// アニメーションの再生割合取得
	const std::function<float(void)> GetAnimPlayRatio;
	
	// アニメーションが終わっているかどうか
	const std::function<bool(void)> IsAnimeEnd;
	
	// ステートを抜けたあとのステート
	const std::function<void(void)> DefaultChangeState;
	const std::function<void(void)> SetCoolTime;
#pragma endregion

	// 終わっているか
	bool end;

	// ターゲット番号の保持
	int target;

	// 弾の数
	int num;

	// 弾の間隔
	int disp;
};