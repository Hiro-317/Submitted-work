#include "ClearCharaPreviewManager.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../../Scene/SceneManager/SceneManager.h"

#include "Orange/ClearCharaPreviewOrange.h"
#include "Tomato/ClearCharaPreviewTomato.h"
#include "Peach/ClearCharaPreviewPeach.h"
#include "Grape/ClearCharaPreviewGrape.h"

ClearCharaPreviewManager::ClearCharaPreviewManager() :
	charaPreview{ nullptr, nullptr, nullptr, nullptr }
{
}

void ClearCharaPreviewManager::Load(void)
{
	for (int id = 0; id < Net::GetIns().GetConnectStatus().EntryCount(); id++) { LoadChara((MSG_SENDER_ID)id); }
}

void ClearCharaPreviewManager::Init(void)
{
	// キャラプレビューの更新
	for (ClearCharaPreviewBase* preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Init();
	}
}

void ClearCharaPreviewManager::Update(void)
{
	// キャラプレビューの更新
	for (ClearCharaPreviewBase* preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Update();
	}
}

void ClearCharaPreviewManager::Draw(void)
{
	// キャラプレビューの描画
	for (ClearCharaPreviewBase* preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Draw();
	}
}

void ClearCharaPreviewManager::Release(void)
{
	// キャラプレビューの解放
	for (ClearCharaPreviewBase*& preview : charaPreview) {
		if (preview == nullptr) { continue; }
		preview->Release();
		delete preview;
		preview = nullptr;
	}
}

void ClearCharaPreviewManager::LoadChara(MSG_SENDER_ID senderId)
{
	// 送信元IDのチェック
	if (senderId <= MSG_SENDER_ID::None || MSG_SENDER_ID::Max <= senderId) { return; }

	// キャラプレビューの生成
	switch (SceneManager::GetIns().GetSelectCharaType(senderId)) {

	case CHARA_TYPE::None: { return; }	// 未選択

	case CHARA_TYPE::Orange: {	// オレンジ
		charaPreview[(int)senderId] = new ClearCharaPreviewOrange(MULTI_CHARA_PREVIEW_POS[(int)senderId], (int)senderId + 1);
		break;
	}

	case CHARA_TYPE::Tomato: {	// トマト
		charaPreview[(int)senderId] = new ClearCharaPreviewTomato(MULTI_CHARA_PREVIEW_POS[(int)senderId], (int)senderId + 1);
		break;
	}

	case CHARA_TYPE::Peach: {	// 桃
		charaPreview[(int)senderId] = new ClearCharaPreviewPeach(MULTI_CHARA_PREVIEW_POS[(int)senderId], (int)senderId + 1);
		break;
	}

	case CHARA_TYPE::Grape: {	// ぶどう
		charaPreview[(int)senderId] = new ClearCharaPreviewGrape(MULTI_CHARA_PREVIEW_POS[(int)senderId], (int)senderId + 1);
		break;
	}

	default: { return; }	// 例外
	}

	// キャラプレビューのロードと初期化
	charaPreview[(int)senderId]->Load();
}