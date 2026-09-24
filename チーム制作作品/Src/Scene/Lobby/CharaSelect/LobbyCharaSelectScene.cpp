#include "LobbyCharaSelectScene.h"

#include "../../../Application/Application.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../SceneManager/SceneManager.h"

#include "../../ObjectUseDefine.h"

#include "../../../Object/Lobby/CharaSelectScene/CharaSelectPreviewManager.h"

LobbyCharaSelectScene::LobbyCharaSelectScene(
	const std::function<void(void)>& LobbyPreviewCharaChange,
	const std::function<void(void)>& LobbyReceptionUpdate
) :
	SceneBase(),
	LobbyPreviewCharaChange(LobbyPreviewCharaChange),
	LobbyReceptionUpdate(LobbyReceptionUpdate),
	selectCharaType(CHARA_TYPE::None)
{
}

void LobbyCharaSelectScene::SubPostLoad(void)
{
	// 選択キャラの初期化
	MSG_SENDER_ID sederId = Net::GetIns().GetSenderId();
	selectCharaType = SceneManager::GetIns().GetSelectCharaType(sederId == MSG_SENDER_ID::None ? MSG_SENDER_ID::P1 : sederId);
	if (selectCharaType <= CHARA_TYPE::None || CHARA_TYPE::Max <= selectCharaType) { selectCharaType = (CHARA_TYPE)((int)CHARA_TYPE::None + 1); }

	ObjAdd(new CharaSelectPreviewManager(selectCharaType));
}

void LobbyCharaSelectScene::SubPostUpdate(void)
{
	// 戻る
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		// このシーンを破棄
		SceneManager::GetIns().PopScene();

		return;
	}

	// キャラ変更を記録
	bool changeCharacter = false;;

	// 操作権者による操作によって選択キャラを変える
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {
		// 1つ前のタイプへ
		selectCharaType = (CHARA_TYPE)((int)selectCharaType - 1);
		// 範囲外だったら逆の端へ
		if (selectCharaType <= CHARA_TYPE::None) { selectCharaType = (CHARA_TYPE)((int)CHARA_TYPE::Max - 1); }
		// キャラを変更したことを記録
		changeCharacter = true;
	}
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {
		// 1つ次のタイプへ
		selectCharaType = (CHARA_TYPE)((int)selectCharaType + 1);
		// 範囲外だったら逆の端へ
		if (selectCharaType >= CHARA_TYPE::Max) { selectCharaType = (CHARA_TYPE)((int)CHARA_TYPE::None + 1); }
		// キャラを変更したことを記録
		changeCharacter = true;
	}
	// プレビューを変更
	if (changeCharacter) { ObjSerch<CharaSelectPreviewManager>(objects)->ChangeCharacter(selectCharaType); }

	// キャラ確定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

		// 範囲チェック
		if (selectCharaType <= CHARA_TYPE::None || CHARA_TYPE::Max <= selectCharaType) { return; }

		// 選択中のキャラで、SceneManagerが抱えているキャラ選択情報を書き換える
		SceneManager::GetIns().SetSelectCharaType(Net::GetIns().GetSenderId() == MSG_SENDER_ID::None ? MSG_SENDER_ID::P1 : Net::GetIns().GetSenderId(), selectCharaType);

		// キャラ変更情報を送信
		Net::GetIns().Send(MsgDataCharaSelect((int)selectCharaType));

		// このパソコンのロビーシーンのプレビューを更新する
		LobbyPreviewCharaChange();

		// キャラ変更終了でこのシーンを破棄
		SceneManager::GetIns().PopScene();

		// 以降はthisがnullptrとなっているため終了
		return;
	}

	// マルチロビー画面の受信処理
	if (LobbyReceptionUpdate != nullptr) { LobbyReceptionUpdate(); }
}

void LobbyCharaSelectScene::SubPreDraw(void)
{
	// 画面全体を半透明の黒で塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}