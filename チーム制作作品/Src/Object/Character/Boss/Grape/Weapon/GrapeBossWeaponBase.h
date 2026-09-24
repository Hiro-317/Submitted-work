#pragma once
#include "../../../../ActorBase.h"


// 種類の列挙型定義
enum class WeaponType {
	None = -1,

	// 直線投擲
	Straight,

	// 爆弾投下（蹴り攻撃と同時に身の回りに落とす爆弾）
	KickBomb,

	// 爆弾投下（スタンプ攻撃と同時に身の回りに落とす爆弾）
	StampBomb,

	// 爆弾投下（プレイヤーに向かって降らせる爆弾）
	SingleBomb,

	// 爆弾投下（1体のプレイヤーに一定時間降らせ続ける爆弾）
	StalkerBomb,

	// 爆弾投下（ランダムに地上に大量投下する爆弾）
	RandomBomb,

	Max
};

static constexpr int WeponDuplicateNum[(int)WeaponType::Max] = {
	// 直線投擲
	3,

	// 爆弾投下（蹴り攻撃と同時に身の回りに落とす爆弾）
	7,

	// 爆弾投下（スタンプ攻撃と同時に身の回りに落とす爆弾）
	10,

	// 爆弾投下（プレイヤーに向かって降らせる爆弾）
	1,

	// 爆弾投下（1体のプレイヤーに一定時間降らせ続ける爆弾）
	5,

	// 爆弾投下（ランダムに地上に大量投下する爆弾）
	30
};

class GrapeBossWeaponBase : public ActorBase
{
public:

	GrapeBossWeaponBase(int model, int front, int back);
	~GrapeBossWeaponBase() = default;

	// 規定用のLoad
	virtual void Load(void) override {};
	// 行うロード
	virtual void Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats) = 0;

	// 初期設定用関数
	void SetStartPos(Vector3 pos) { trans.pos = pos; }
	void ResetCount(void) { count = 0.0f; }

	// コライダーのオン/オフ
	void SetColliderFlg(bool flg) { SetJudge(flg); }
	void ResetEnd(void) { end = false; }

	// 攻撃が終わったか
	bool IsEnd(void) { return end; }

	// 予測線の設定(円形)
	void SetViewScaleCircle(float scale) { collFront.scale = Vector3(scale); };
	void SetViewPosCircle(void) { collBack.pos = trans.pos; collFront.pos = trans.pos; collBack.pos.y = VIEW_HEIGHT; collFront.pos.y = VIEW_HEIGHT; };

protected:

	// 基本
	void SubDraw(void) override;

	// 予測線
	Transform collBack;
	Transform collFront;

	// 行動カウント
	float count;

	// 終了フラグ
	bool end;

	// 今開始されたか
	bool now;

private:
#pragma region 定数定義

	static constexpr float VIEW_HEIGHT = 37.0f;

#pragma endregion
};
