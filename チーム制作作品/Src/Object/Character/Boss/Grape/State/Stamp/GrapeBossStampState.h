#pragma once

#include "../../../../CharacterStateBase.h"

#include "GrapeStampCollOperator.h"
#include "../../Weapon/GrapeBossWeaponManager.h"

#include "../../../../../../Common/Vector3.h"

class GrapeBossStampState : public CharacterStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ownChangeState">自分の状態に遷移する関数</param>
	/// <param name="isOwnState">自分の状態かどうかを返す関数</param>
	/// <param name="pos">座標の参照</param>
	/// <param name="angle">角度の参照</param>
	/// <param name="isGround">地面かどうか</param>
	/// <param name="bombType">攻撃種のクラス取得</param>
	/// <param name="playerPos">プレイヤーの座標の読み取り</param>
	/// <param name="collOperator">当たり判定オペレータの参照</param>
	/// <param name="JUMP_START_ANIM_TYPE">踏み切りアニメーションのenumタイプ</param>
	/// <param name="STAMP_ANIM_TYPE">スタンプアニメーションのenumタイプ</param>
	/// <param name="GetTarget">ターゲット番号の取得</param>
	/// <param name="PlayJumpStartAnim">踏み切りアニメーションの再生</param>
	/// <param name="PlayJumpAnim">ジャンプ中アニメーションの再生</param>
	/// <param name="PlayStampAnim">スタンプアニメーションの再生</param>
	/// <param name="GetAnimPlayRatio">攻撃アニメーションの再生割合取得</param>
	/// <param name="IsAnimeEnd">アニメーションが終了しているか</param>
	/// <param name="DeleteColl">ボス自体のコライダの消滅</param>
	/// <param name="ReviveColl">ボス自体のコライダの再生</param>
	/// <param name="DefaultChangeState">攻撃後遷移ステート</param>
	/// <param name="SetCoolTime">クールタイムの設定</param>
	GrapeBossStampState(
		const std::function<void(void)>& ownChangeState,
		const std::function<bool(void)>& isOwnState,
		Vector3& pos, Vector3& angle, const bool& isGround,
		BombType* bombType,
		const std::vector<const Vector3*> playerPos,
		GrapeStampCollOperator* collOperator,
		const int JUMP_START_ANIM_TYPE,
		const int STAMP_ANIM_TYPE,
		const std::function<int(void)> GetTarget,
		const std::function<void(void)> PlayJumpStartAnim,
		const std::function<void(void)> PlayJumpAnim,
		const std::function<void(void)> PlayStampAnim,
		const std::function<int(void)> GetAnimType,
		const std::function<float(void)> GetAnimPlayRatio,
		const std::function<bool(void)> IsAnimeEnd,
		const std::function<void(void)> DefaultChangeState,
		const std::function<void(void)> offCollider,
		const std::function<void(void)> onCollider,
		const std::function<void(void)> SetCoolTime
	);
	~GrapeBossStampState()override = default;

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

	// 飛ぶ強さ
	static constexpr float JUMP_POW = 29.0f;

	// 目標までの到達時間(割合)
	static constexpr float TIME_RATE = 15.0f;

	// 攻撃の発生持続時間
	static constexpr int ATTACK_DURATION = 5;

#pragma endregion

#pragma region 受け取る参照変数・関数

	// 座標の参照
	Vector3& pos;

	// 角度の参照
	Vector3& angle;
	
	const bool& isGround;

	// ボムの参照
	BombType* bombType;

	// プレイヤーの座標の参照
	const std::vector<const Vector3*> playerPos;

	// コライダーのオペレーターのポインター
	GrapeStampCollOperator* collOperator;

	const int JUMP_START_ANIM_TYPE;
	const int STAMP_ANIM_TYPE;

	// プレイヤーの標的番号
	const std::function<int(void)> GetTarget;
	
	// アタックアニメーションの再生
	const std::function<void(void)> PlayJumpStartAnim;
	const std::function<void(void)> PlayJumpAnim;
	const std::function<void(void)> PlayStampAnim;

	// アニメーションのタイプの取得
	const std::function<int(void)> GetAnimType;
	
	// アニメーションの再生割合取得
	const std::function<float(void)> GetAnimPlayRatio;
	
	// アニメーションが終わっているかどうか
	const std::function<bool(void)> IsAnimeEnd;
	
	// ステートを抜けたあとのステート
	const std::function<void(void)> DefaultChangeState;

	const std::function<void(void)> offCollider;
	const std::function<void(void)> onCollider;

	const std::function<void(void)> SetCoolTime;

#pragma endregion

	// 一度だけ通すように
	bool first;

	// 攻撃持続のカウント
	int cnt;

	// 予測線描画座標
	Vector3 viewPos;

	// 標的番号
	int target;

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

	// 直前の座標
	float prevPos;

};