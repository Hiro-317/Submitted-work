#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossStampBomb : public GrapeBossWeaponBase
{
public:
	GrapeBossStampBomb(int model, int front, int back);
	~GrapeBossStampBomb() = default;

	// ロード
	virtual void Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats) override;

	// 爆発までのカウント
	const float BOMBER_COUNT = GetParameter("StampBomb", "BomberCnt");
	// 攻撃持続時間
	const float ATTACK_DURATION = GetParameter("StampBomb", "Duration");

protected:

	// 派生先追加更新
	virtual void SubUpdate(void) override;

private:

	// 攻撃範囲の描画
	const float ATTACK_RANGE = GetParameter("StampBomb", "Range");

	// 攻撃サイズ倍率
	const float ATTACK_SIZE = GetParameter("StampBomb", "Size");

	// モデルのサイズ
	const float MODEL_RADIUS = GetParameter("StampBomb", "Radius");
};
