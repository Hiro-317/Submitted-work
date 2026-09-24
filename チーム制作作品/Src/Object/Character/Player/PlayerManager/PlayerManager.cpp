#include "PlayerManager.h"


PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}

// ロード
void PlayerManager::Load(void)
{
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
		if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
		
		// プレイヤーを生成
		playerInfo.emplace_back(PlayerFactory::CreatePlayer((MSG_SENDER_ID)id));
	}
	
	for (PlayerInfo& own : playerInfo) {
		// プレイヤーのロード
		own.instance->Load();

		for (PlayerInfo& other : playerInfo) {
			// 自身以外のプレイヤーの座標を渡す
			if (own.instance->GetOperatorSenderId() != other.instance->GetOperatorSenderId()) {
				own.instance->SetOtherPlayerTrans(&other.instance->GetTrans());
			}
		}
	}

}

// 初期化処理
void PlayerManager::Init(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->Init();
	}
}

// 更新処理
void PlayerManager::Update(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->Update();
	}
}

// 描画処理
void PlayerManager::Draw(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->Draw();
	}
}

// UI描画処理
void PlayerManager::UiDraw(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->UiDraw();
	}
}

// 解放処理
void PlayerManager::Release()
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->Release();
	}
}

void PlayerManager::AlphaDraw(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->AlphaDraw();
	}
}

void PlayerManager::ReceptionUpdate(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->ReceptionUpdate();
	}
}

void PlayerManager::SendUpdate(void)
{
	for (PlayerInfo& info : playerInfo) {
		info.instance->SendUpdate();
	}
}