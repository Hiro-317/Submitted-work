#pragma once
#include "../GrapeBossWeaponBase.h"

class GrapeBossStalkerBomb : public GrapeBossWeaponBase
{
public:
	GrapeBossStalkerBomb(int model, int front, int back);
	~GrapeBossStalkerBomb() = default;

	// ロード
	virtual void Load(const MSG_SENDER_ID operatorSenderId, const CharacterStats& stats) override;

	// 爆発までのカウント
	const float BOMBER_COUNT = GetParameter("StalkerBomb", "BomberCnt");
	// 攻撃持続時間
	const float ATTACK_DURATION = GetParameter("StalkerBomb", "Duration");

protected:

	// 派生先追加更新
	virtual void SubUpdate(void) override;

private:

	// 攻撃範囲の描画
	const float ATTACK_RANGE = GetParameter("StalkerBomb", "Range");

	// 攻撃サイズ倍率
	const float ATTACK_SIZE = GetParameter("StalkerBomb", "Size");

	// モデルのサイズ
	const float MODEL_RADIUS = GetParameter("StalkerBomb", "Radius");
};
