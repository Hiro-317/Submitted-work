#pragma once
#include "../GrapeBossWeaponBase.h"


class GrapeBossSingleBomb : public GrapeBossWeaponBase
{
public:
	GrapeBossSingleBomb(int model, int front, int back);
	~GrapeBossSingleBomb() = default;

	// ロード
	virtual void Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats) override;

	// 爆発までのカウント
	const float BOMBER_COUNT = GetParameter("SingleBomb", "BomberCnt");
	// 攻撃持続時間
	const float ATTACK_DURATION = GetParameter("SingleBomb", "Duration");

protected:

	// 派生先追加更新
	virtual void SubUpdate(void) override;

private:

	// 攻撃範囲の描画
	const float ATTACK_RANGE = GetParameter("SingleBomb", "Range");

	// 攻撃サイズ倍率
	const float ATTACK_SIZE = GetParameter("SingleBomb", "Size");

	// モデルのサイズ
	const float MODEL_RADIUS = GetParameter("SingleBomb", "Radius");
};
