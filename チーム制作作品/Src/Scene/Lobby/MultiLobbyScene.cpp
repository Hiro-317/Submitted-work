#include "MultiLobbyScene.h"

#include "../../pch.h"

#include "../../Manager/Input/KeyManager.h"
#include "../../Manager/Camera/FixedPoint/FixedPointCamera.h"
#include "../../Manager/Camera/CameraEvent/StartCameraEvent/StartCameraEvent.h"

#include "../../Manager/Net/NetWorkManager.h"
#include "../../Manager/Sound/SoundManager.h"

#include "../../Scene/SceneManager/SceneManager.h"

#include "../ObjectUseDefine.h"

#include "CharaSelect/LobbyCharaSelectScene.h"

#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Lobby/LobbyStage/LobbyStage.h"
#include "../../Object/Lobby/LobbyCharaPreview/LobbyCharaPreviewManager.h"

MultiLobbyScene::MultiLobbyScene() : 
	SceneBase(),
	IS_HOST(Net::GetIns().IsHost()),

	choice(),

	buttonSelectionState(),

	boardImage(-1),

	choiceButtonImage(),

	arrowImage(-1),

	enterKeyImage()
{
}

void MultiLobbyScene::SubPostLoad(void)
{
	// オブジェクト生成
	ObjAdd(new SkyDome());
	ObjAdd(new LobbyStage());
	ObjAdd(new LobbyCharaPreviewManager());

	// ホストは自分以外の全員、クライアントは自分のみの準備完了状態を管理する
	readyList.resize((int)MSG_SENDER_ID::Max, (unsigned char)false);

#pragma region 各画像の読み込み

	// 画面上部のボードの画像
	boardImage = LoadGraph((IMAGE_DATA_FILE_DIR + "LobbyBoard.png").c_str());

	// 選択肢ボタンの画像
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		// 名前
		const std::string IMAGE_NAME = IMAGE_DATA_FILE_DIR + CHOICE_BUTTON_IMAGE_NAME[choiceIndex];

		// 非選択時/選択時/選択不可 の画像を順番に読み込む
		for (int selectionState = 0; selectionState < (int)SELECTION_STATE::Max; selectionState++) {
			choiceButtonImage[choiceIndex][selectionState] =
				LoadGraph((IMAGE_NAME + CHOICE_BUTTON_IMAGE_DECORATION[selectionState] + ".png").c_str());
		}
	}

	// 選択中の矢印
	arrowImage = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectArrow.png").c_str());

	// 選択中の決定キー
	enterKeyImage[(int)false] = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectKeyboard.png").c_str());
	enterKeyImage[(int)true] = LoadGraph((IMAGE_DATA_FILE_DIR + "NowSelectController.png").c_str());

#pragma endregion
}

void MultiLobbyScene::SubPostInit(void)
{
	SetFogEnable(false);

	// クライアントの準備完了状態を初期化
	for (unsigned char& ready : readyList) { ready = (unsigned char)false; }
	// ホストは常に準備完了状態
	readyList.at((int)Net::HOST_SENDER_ID) = (unsigned char)true;

	// 選択中の値を初期化
	choice = CHOICE::CharaChange;
	// ボタンごとの選択状態を初期化
	ButtonSelectionStateReload();

	// 自信のキャラクタータイプを初期化する
	if (SceneManager::GetIns().GetSelectCharaType(Net::GetIns().GetSenderId()) == CHARA_TYPE::None) {
		SceneManager::GetIns().SetSelectCharaType(Net::GetIns().GetSenderId(), (CHARA_TYPE)((int)CHARA_TYPE::None + 1));
	}

	// キャラ変更情報を送信
	Net::GetIns().Send(MsgDataCharaSelect((int)SceneManager::GetIns().GetSelectCharaType(Net::GetIns().GetSenderId())));

	// キャラプレビューの初期化
	ObjSerch<LobbyCharaPreviewManager>(objects)->ReloadChara(Net::GetIns().GetSenderId());
}

void MultiLobbyScene::SubPostUpdate(void)
{
	// タイトル画面に戻る
	if (Key::GetIns().GetInfo(KEY_TYPE::PAUSE).down) {

		// 効果音
		Snd::GetIns().Play("SystemButton");

		// ネットワーク切断
		Net::GetIns().Disconnection();

		// ロビー画面に戻る
		SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Lobby);
		return;
	}

	// 左
	if (Key::GetIns().GetInfo(KEY_TYPE::LEFT).down) {

		// 効果音
		Snd::GetIns().Play("SystemSelect");

		// 1つ左へ（選択中の値を1つ減らす）
		choice = (CHOICE)((int)choice - 1);
		// 範囲を超えていたら戻す
		if (choice <= CHOICE::None) { choice = (CHOICE)((int)CHOICE::None + 1); }

		// ボタンごとの選択状態を更新
		ButtonSelectionStateReload();
	}
	// 右
	if (Key::GetIns().GetInfo(KEY_TYPE::RIGHT).down) {

		// 効果音
		Snd::GetIns().Play("SystemSelect");

		// 1つ右へ（選択中の値を1つ増やす)）
		choice = (CHOICE)((int)choice + 1);
		// 範囲を超えていたら戻す
		if (choice >= CHOICE::Max) { choice = (CHOICE)((int)CHOICE::Max - 1); }

		// ボタンごとの選択状態を更新
		ButtonSelectionStateReload();
	}

	// 決定
	if (Key::GetIns().GetInfo(KEY_TYPE::ENTER).down) {

		// 効果音
		if (choice > CHOICE::None && choice < CHOICE::Max &&
			buttonSelectionState[(int)choice] != SELECTION_STATE::Disable
			) {
			Snd::GetIns().Play("SystemButton");
		}
		else { /*Snd::GetIns().Play("");*/ }

		// 選択肢ごとの処理
		switch (choice) {

		case MultiLobbyScene::CHOICE::None: { break; }	// 選択肢なし（ありえないはず）

		case MultiLobbyScene::CHOICE::Disconnected: {	// 切断

			// クライアントかつ、準備完了状態だったら、処理をしない
			if (!IS_HOST && readyList.at((int)Net::GetIns().GetSenderId()) == (unsigned char)true) { break; }

			// ネットワーク切断
			Net::GetIns().Disconnection();

			// 通常のロビー画面に戻る
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Lobby);

			// 以降はthisがnullptrとなっているため終了
			return;
		}

		case MultiLobbyScene::CHOICE::CharaChange: {	// キャラ変更

			// クライアントかつ、準備完了状態だったら、処理をしない
			if (!IS_HOST && readyList.at((int)Net::GetIns().GetSenderId()) == (unsigned char)true) { break; }

			// 専用のシーンを追加する
			SceneManager::GetIns().PushScene(
				std::make_unique<LobbyCharaSelectScene>(
					// キャラ変更シーンから戻ってきたときに、プレビューを更新
					[&]() { ObjSerch<LobbyCharaPreviewManager>(objects)->ReloadChara(Net::GetIns().GetSenderId()); },
					std::bind(&MultiLobbyScene::ReceptionUpdate, this)
				)
			);

			// このシーンの処理は終了
			return;
		}

		case MultiLobbyScene::CHOICE::Enter: {	// <ホスト>出撃 / <クライアント>準備完了

			// ホスト
			if (IS_HOST) {

				// 全クライアントの準備完了フラグを確認する
				if (buttonSelectionState[(int)choice] == SELECTION_STATE::Disable) { break; }

				// 接続人数を確定する
				Net::GetIns().CloseReceptionToConnected();

				// ゲームシーン遷移を通知
				Net::GetIns().Send(MsgDataSystemInform(MsgDataSystemInform::INFORM_TYPE::ChangeSceneBossSelect));

				// ゲームシーン遷移
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::BossSelect);

				// 以降はthisがnullptrとなっているため終了
				return;
			}
			// クライアント
			else {
				// 自信の準備状態の参照を生成
				unsigned char& ready = readyList.at((int)Net::GetIns().GetSenderId());

				// 準備 完了/未完了 切り替え
				ready = 1 - ready;

				// 準備の状態を送信
				Net::GetIns().Send(MsgDataClientReady((bool)ready));

				// ボタンごとの選択状態を更新
				ButtonSelectionStateReload();
			}
		}
		}
	}

	// 受信処理
	ReceptionUpdate();
}

void MultiLobbyScene::SubPostDraw(void)
{
	// ボードの描画
	DrawRotaGraph(BOARD_POS.x, BOARD_POS.y, 1, 0, boardImage, true);

	// 選択肢の描画
	for (int i = 0; i < (int)CHOICE::Max; i++) {
		DrawRotaGraph(CHOICE_BUTTON_POS[i].x, CHOICE_BUTTON_POS[i].y, 1, 0, choiceButtonImage[i][(int)buttonSelectionState[i]], true);
	}
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y, 1, 0, arrowImage, true);
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y - 75, 0.3f, 0, enterKeyImage[(int)Key::GetIns().LastInputKinds()], true);
}

void MultiLobbyScene::SubPreRelease(void)
{
#pragma region 画像の解放

	// 選択中の決定キー
	DeleteGraph(enterKeyImage[(int)true]);
	DeleteGraph(enterKeyImage[(int)false]);

	// 選択中の矢印
	DeleteGraph(arrowImage);

	// 選択肢ボタンの画像
	for(int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		for (int selectionState = 0; selectionState < (int)SELECTION_STATE::Max; selectionState++) {
			DeleteGraph(choiceButtonImage[choiceIndex][selectionState]);
		}
	}

	// 画面上部のボードの画像
	DeleteGraph(boardImage);

#pragma endregion

	SetFogEnable(true);
}

void MultiLobbyScene::CreateCamera(void)
{
	// 定点カメラを生成する
	camera = new FixedPointCamera(FixedPointCamera::LookAt, Vector3::YZonly(750.0f, -2000.0f), Vector3::Yonly(150.0f));

	// イベントカメラを生成
	camera->StartEvent(new StartCameraEvent(*camera, Vector3(0.0f, 1500.0f, -5000.0f), Vector3::Yonly(Deg2Rad(-30.0f)), 0.0f, 200.0f));
}

void MultiLobbyScene::ButtonSelectionStateReload(void)
{
	// クライアントで準備完了を押していた場合、要素すべてを選択不可状態にする
	if (!IS_HOST && readyList.at((int)Net::GetIns().GetSenderId()) == (unsigned char)true) {
		for (auto& state : buttonSelectionState) { state = SELECTION_STATE::Disable; }
		return;
	}

	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		// choice(選択中のボタンの種類) を参照し、
		// そのボタンを 選択中の場合「Select」/ 選択中の場合「NotSelect」
		buttonSelectionState[choiceIndex] = ((int)choice == choiceIndex) ? SELECTION_STATE::Select : SELECTION_STATE::NotSelect;
	}

	if (!IS_HOST) { return; }
	// ホストの場合

	// 全クライアントの準備完了フラグを確認する
	// まだ全員準備完了していなかったら、出撃ボタンを選択不可状態にする
	for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {

		// まず、参加しているクライアントかどうかを確認する（参加していないクライアントの準備完了フラグは見ない）
		if (Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id) == false) { break; }

		// 準備完了していないクライアントがいたら、出撃ボタンを選択不可状態にする
		if (readyList.at(id) == (unsigned char)false) {
			buttonSelectionState[(int)CHOICE::Enter] = SELECTION_STATE::Disable;
			break;
		}
	}
}

void MultiLobbyScene::ReceptionUpdate(void)
{
	// 接続/切断 の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataConnectInform>()) {

		switch (dataPtr->inform) {

		case MsgDataConnectInform::INFORM_TYPE::None: { break; }

		case MsgDataConnectInform::INFORM_TYPE::Connect: {

			if (Net::GetIns().IsHost()) {
				// 現状の選択キャラを送る
				for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
					if (!Net::GetIns().GetConnectStatus().IsEntry((MSG_SENDER_ID)id)) { break; }
					if (id == (int)dataPtr->header.senderId) { continue; }

					Net::GetIns().Send(
						MsgDataCharaSelect((int)SceneManager::GetIns().GetSelectCharaType((MSG_SENDER_ID)id)),
						(MSG_SENDER_ID)id,
						dataPtr->header.senderId
					);
				}
				// 現状の選択ボスを送る
				Net::GetIns().Send(MsgDataBossSelect((int)SceneManager::GetIns().GetSelectBossType()));
			}

			// ボタンごとの選択状態を更新
			ButtonSelectionStateReload();

			break;
		}

		case MsgDataConnectInform::INFORM_TYPE::Disconnect: {
			if (!Net::GetIns().IsHost()) {
				Net::GetIns().Disconnection();
				SceneManager::GetIns().ChangeSceneFade(SCENE_ID::Lobby);
				delete dataPtr;
				return;
			}

			// 切断されたID以降の選択キャラをソートして正す
			for (int id = (int)dataPtr->header.senderId; id < (int)MSG_SENDER_ID::Max - 1; id++) {
				SceneManager::GetIns().SetSelectCharaType(
					(MSG_SENDER_ID)id,
					SceneManager::GetIns().GetSelectCharaType((MSG_SENDER_ID)((int)id + 1))
				);
			}

			// キャラプレビューを更新する
			for (int id = (int)dataPtr->header.senderId; id < (int)MSG_SENDER_ID::Max; id++) {
				ObjSerch<LobbyCharaPreviewManager>(objects)->ReloadChara((MSG_SENDER_ID)id);
			}

			// 最新状態の選択キャラを送りなおす
			for (int id = 0; id < (int)MSG_SENDER_ID::Max; id++) {
				Net::GetIns().Send(
					MsgDataCharaSelect((int)SceneManager::GetIns().GetSelectCharaType((MSG_SENDER_ID)id)),
					(MSG_SENDER_ID)id
				);
			}

			// ボタンごとの選択状態を更新
			ButtonSelectionStateReload();

			break;
		}

		default: { break; }
		}

		delete dataPtr;
	}

	// 選択キャラの受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataCharaSelect>(MSG_SENDER_ID::None, true)) {

		// 受け取ったキャラタイプを保存する
		SceneManager::GetIns().SetSelectCharaType(dataPtr->header.senderId, (CHARA_TYPE)dataPtr->charaType);

		// キャラプレビューを更新する
		ObjSerch<LobbyCharaPreviewManager>(objects)->ReloadChara(dataPtr->header.senderId);

		delete dataPtr;
	}

	// 準備完了の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataClientReady>(MSG_SENDER_ID::None, true)) {

		// 受け取った準備完了フラグを保存する
		readyList.at((int)dataPtr->header.senderId) = (unsigned char)dataPtr->ready;

		// ボタンごとの選択状態を更新
		ButtonSelectionStateReload();

		delete dataPtr;
	}

	// システム通知の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>()) {

		// シーン遷移の受信
		if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::ChangeSceneBossSelect) {
			// ゲームシーン遷移
			SceneManager::GetIns().ChangeSceneFade(SCENE_ID::BossSelect);

			delete dataPtr;
			return;
		}

		delete dataPtr;
	}
}
