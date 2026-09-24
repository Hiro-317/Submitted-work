#include "LobbyCharaPreviewManager.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../../Scene/SceneManager/SceneManager.h"

#include "Orange/LobbyCharaPreviewOrange.h"
#include "Tomato/LobbyCharaPreviewTomato.h"
#include "Peach/LobbyCharaPreviewPeach.h"
#include "Grape/LobbyCharaPreviewGrape.h"

LobbyCharaPreviewManager::LobbyCharaPreviewManager() :
	charaPreview{ nullptr, nullptr, nullptr, nullptr },
	ownOperatorImage(-1)
{
}

void LobbyCharaPreviewManager::Load(void)
{
	if(SceneManager::GetIns().GetSelectCharaType(Net::HOST_SENDER_ID) == CHARA_TYPE::None) {
		SceneManager::GetIns().SetSelectCharaType(Net::HOST_SENDER_ID, CHARA_TYPE::Orange);
	}
	if (Net::GetIns().GetState() == Net::NetState::None) { ReloadChara(); }
	else {
		ReloadChara(MSG_SENDER_ID::P1);
		ownOperatorImage = LoadGraph(OWN_OPERATOR_IMAGE[(int)Net::GetIns().GetSenderId()].c_str());
	}
}

void LobbyCharaPreviewManager::Update(void)
{
	// キャラプレビューの更新
	for (LobbyCharaPreviewBase* preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Update();
	}
}

void LobbyCharaPreviewManager::Draw(void)
{
	// キャラプレビューの描画
	for (LobbyCharaPreviewBase* preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Draw();
	}
	if (ownOperatorImage != -1) {
		charaPreview[(int)Net::GetIns().GetSenderId()]->OwnOperatorDraw(ownOperatorImage);
	}
}

void LobbyCharaPreviewManager::Release(void)
{
	if (ownOperatorImage != -1) { DeleteGraph(ownOperatorImage); }

	// キャラプレビューの解放
	for (LobbyCharaPreviewBase*& preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Release();
		delete preview;
		preview = nullptr;
	}
}

void LobbyCharaPreviewManager::ReloadChara(void)
{
	// キャラプレビューの解放
	if (charaPreview[(int)MSG_SENDER_ID::P1] != nullptr) {
		charaPreview[(int)MSG_SENDER_ID::P1]->Release();
		delete charaPreview[(int)MSG_SENDER_ID::P1];
		charaPreview[(int)MSG_SENDER_ID::P1] = nullptr;
	}

	// キャラプレビューの生成
	switch (SceneManager::GetIns().GetSelectCharaType(MSG_SENDER_ID::P1)) {

	case CHARA_TYPE::None: { return; }	// 未選択

	case CHARA_TYPE::Orange: {	// オレンジ
		charaPreview[(int)MSG_SENDER_ID::P1] = new LobbyCharaPreviewOrange(CHARA_PREVIEW_POS, (int)MSG_SENDER_ID::P1 + 1);
		break;
	}

	case CHARA_TYPE::Tomato: {	// トマト
		charaPreview[(int)MSG_SENDER_ID::P1] = new LobbyCharaPreviewTomato(CHARA_PREVIEW_POS, (int)MSG_SENDER_ID::P1 + 1);
		break;
	}

	case CHARA_TYPE::Peach: {	// 桃
		charaPreview[(int)MSG_SENDER_ID::P1] = new LobbyCharaPreviewPeach(CHARA_PREVIEW_POS, (int)MSG_SENDER_ID::P1 + 1);
		break;
	}

	case CHARA_TYPE::Grape: {	// ぶどう
		charaPreview[(int)MSG_SENDER_ID::P1] = new LobbyCharaPreviewGrape(CHARA_PREVIEW_POS, (int)MSG_SENDER_ID::P1 + 1);
		break;
	}

	default: { return; }	// 例外
	}

	// キャラプレビューのロードと初期化
	charaPreview[(int)MSG_SENDER_ID::P1]->Load();
	charaPreview[(int)MSG_SENDER_ID::P1]->Init();
}

void LobbyCharaPreviewManager::ReloadChara(MSG_SENDER_ID senderId)
{
	// 送信元IDのチェック
	if (senderId <= MSG_SENDER_ID::None || MSG_SENDER_ID::Max <= senderId) { return; }

	// キャラプレビューの解放
	if (charaPreview[(int)senderId] != nullptr) {
		charaPreview[(int)senderId]->Release();
		delete charaPreview[(int)senderId];
		charaPreview[(int)senderId] = nullptr;
	}

	// 接続参加状態を参照、参加していなければ処理終了
	if (!Net::GetIns().GetConnectStatus().IsEntry(senderId)) { return; }

	// キャラプレビューの生成
	switch (SceneManager::GetIns().GetSelectCharaType(senderId)) {

	case CHARA_TYPE::None: { return; }	// 未選択

	case CHARA_TYPE::Orange: {	// オレンジ
		charaPreview[(int)senderId] = new LobbyCharaPreviewOrange(MULTI_CHARA_PREVIEW_POS[(int)senderId], (int)senderId + 1);
		break;
	}

	case CHARA_TYPE::Tomato: {	// トマト
		charaPreview[(int)senderId] = new LobbyCharaPreviewTomato(MULTI_CHARA_PREVIEW_POS[(int)senderId], (int)senderId + 1);
		break;
	}

	case CHARA_TYPE::Peach: {	// 桃
		charaPreview[(int)senderId] = new LobbyCharaPreviewPeach(MULTI_CHARA_PREVIEW_POS[(int)senderId], (int)senderId + 1);
		break;
	}

	case CHARA_TYPE::Grape: {	// ぶどう
		charaPreview[(int)senderId] = new LobbyCharaPreviewGrape(MULTI_CHARA_PREVIEW_POS[(int)senderId], (int)senderId + 1);
		break;
	}

	default: { return; }	// 例外
	}

	// キャラプレビューのロードと初期化
	charaPreview[(int)senderId]->Load();
	charaPreview[(int)senderId]->Init();
}