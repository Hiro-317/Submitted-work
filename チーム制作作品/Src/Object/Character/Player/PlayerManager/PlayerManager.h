#pragma once
#include <map>
#include "PlayerFactory.h"

#include "../../../../Manager/Net/NetWorkManager.h"
#include "../../../ActorBase.h"


class PlayerBase;

class PlayerManager : public ActorBase
{
public:

	PlayerManager();	// コンストラクタ
	~PlayerManager();	// デストラクタ

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void ReceptionUpdate(void)override;
	void SendUpdate(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override;
	void UiDraw(void)override;
	void Release(void)override;

	const PlayerBase* GetPlayerIns(const MSG_SENDER_ID id)const { return playerInfo[(int)id].instance; }
	const bool IsPlayerAllDeath(void)const {
		for (const PlayerInfo& info : playerInfo) {
			if (!info.instance->GetIsDeath()) { return false; }
		}
		return true;
	}

	// 状態遷移後1度行う初期化処理
	std::vector<ColliderBase*> GetCollider(void)const override {
		std::vector<ColliderBase*> ret = {};
		// 自身のコライダーを返却用一時変数に格納
		for (const PlayerInfo& player : playerInfo) {
			for (ColliderBase* coll : player.instance->GetCollider()) { ret.emplace_back(coll); }
		}
		// 最終的な返却用一時変数を返却
		return ret;
	}

	// ボスの座標のポインタをセット
	void SetBossPos(const Vector3* bossPos) {
		for (PlayerInfo& info : playerInfo) { info.instance->SetBossPos(bossPos); } 
	}

private:

	const int PLAYER_MAX = Net::GetIns().GetConnectStatus().EntryCount();

	std::vector<PlayerInfo> playerInfo;
};

