#include "BossConfirmScene.h"

#include "../../../pch.h"

#include "../../../Manager/Input/KeyManager.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../../Manager/Sound/SoundManager.h"

#include "../../../Manager/Font/FontManager.h"

#include "../../SceneManager/SceneManager.h"

BossConfirmScene::BossConfirmScene() :
	SceneBase(),
	IS_HOST(Net::GetIns().IsHost()),
	ENTRY_COUNT(Net::GetIns().GetConnectStatus().EntryCount()),

	RANKING_DATA(Ranking::GetIns().GetRanking(SceneManager::GetIns().GetSelectBossType())),

	choice(),

	buttonSelectionState(),

	readyList(),

	frameImage(-1),

	bossSelectImage(-1),

	playerIconBackImage(),
	playerIconFrameImage(),
	playerIconImage(),

	choiceButtonImage(),

	arrowImage(-1),

	enterKeyImage(),

	arrowEasingCounter(0.0f),
	arrowEasingRate(0.0f)
{
}

void BossConfirmScene::SubPostLoad(void)
{
	// ホストは自分以外の全員、クライアントは自分のみの準備完了状態を管理する
	readyList.resize((int)Net::GetIns().GetConnectStatus().EntryCount(), (unsigned char)false);

#pragma region 各画像の読み込み

	// 画像読み込みのラムダ関数
	auto loadBossConfirmImage = [&](std::string name)->int { return LoadGraph((IMAGE_DATA_FILE_DIR + name + ".png").c_str()); };

	// 枠の画像
	frameImage = loadBossConfirmImage(FRAME_IMAGE_NAME);

	// ボスの詳細イメージ
	bossSelectImage = loadBossConfirmImage(BOSS_SELECT_IMAGE_NAME[(int)SceneManager::GetIns().GetSelectBossType()]);

	// プレイヤーキャラアイコンの同じ画像の重複のロード避けるためのフラグ配列
	signed char playerIconDupli[(int)CHARA_TYPE::Max];
	for (signed char& dupli : playerIconDupli) { dupli = -1; }

	// プレイヤーアイコン
	for (char id = 0; id < ENTRY_COUNT; id++) {

		// 背景
		playerIconBackImage[id] = loadBossConfirmImage(PLAYER_ICON_BACK_IMAGE_NAME[id]);
		// フレーム
		playerIconFrameImage[id] = loadBossConfirmImage(PLAYER_ICON_FRAME_IMAGE_NAME[id]);


		// キャラ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～

		// 選択キャラ取得
		CHARA_TYPE charaType = SceneManager::GetIns().GetSelectCharaType((MSG_SENDER_ID)id);

		// 重複ロード確認
		if (playerIconDupli[(int)charaType] == -1) {
			// 初回ロード
			playerIconImage[id] = loadBossConfirmImage(PLAYER_ICON_IMAGE_NAME[(int)charaType]);
			// ロードしたことを保存する
			playerIconDupli[(int)charaType] = id;
		}
		// 2回目以降（ハンドルをコピー）
		else { playerIconImage[id] = playerIconImage[playerIconDupli[(int)charaType]]; }

		// ～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～～キャラ
	}

	// 選択肢ボタンの画像
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {

		// 非選択時/選択時/選択不可 の画像を順番に読み込む
		for (int selectionState = 0; selectionState < (int)SELECTION_STATE::Max; selectionState++) {
			choiceButtonImage[choiceIndex][selectionState] =
				loadBossConfirmImage(CHOICE_BUTTON_IMAGE_NAME[choiceIndex] + CHOICE_BUTTON_IMAGE_DECORATION[selectionState]);
		}
	}

	// 選択中の矢印
	arrowImage = loadBossConfirmImage(ARROW_IMAGE_NAME);

	// 選択中の決定キー
	enterKeyImage[(int)false] = loadBossConfirmImage(ENTER_KEYBOARD_IMAGE_NAME);
	enterKeyImage[(int)true] = loadBossConfirmImage(ENTER_CONTROLLER_IMAGE_NAME);

#pragma endregion
}

void BossConfirmScene::SubPostInit(void)
{
	// クライアントの準備完了状態を初期化
	for (unsigned char& ready : readyList) { ready = (unsigned char)false; }
	// ホストは常に準備完了状態
	readyList.at((int)Net::HOST_SENDER_ID) = (unsigned char)true;

	// 選択中の値を初期化
	choice = CHOICE::Enter;
	// ボタンごとの選択状態を初期化
	ButtonSelectionStateReload();
}

void BossConfirmScene::SubPostUpdate(void)
{
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

		case BossConfirmScene::CHOICE::None: { break; }	// 選択肢なし（ありえないはず）

		case BossConfirmScene::CHOICE::Exit: {	// キャンセル

			// クライアントかつ、準備完了状態だったら、処理をしない
			if (!IS_HOST && readyList.at((int)Net::GetIns().GetSenderId()) == (unsigned char)true) { break; }

			// キャンセル送信
			Net::GetIns().Send(MsgDataSystemInform(MsgDataSystemInform::INFORM_TYPE::PopSceneBossConfirm));

			// 自身のシーン破棄
			SceneManager::GetIns().PopScene();

			break;
		}

		case BossConfirmScene::CHOICE::Enter: {	// <ホスト>出撃 / <クライアント>準備完了

			// ホスト
			if (IS_HOST) {

				// 全クライアントの準備完了フラグを確認する
				if (buttonSelectionState[(int)choice] == SELECTION_STATE::Disable) { break; }

				// ゲームシーン遷移を通知
				Net::GetIns().Send(MsgDataSystemInform(MsgDataSystemInform::INFORM_TYPE::JumpSceneGame));

				// ゲームシーン遷移
				SceneManager::GetIns().JumpSceneFade(SCENE_ID::Game);

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

			break;
		}
		}
	}

#pragma region イージング
	arrowEasingCounter += 0.1f;
	if (arrowEasingCounter > 100000.0f) { arrowEasingCounter = 0.0f; }
	arrowEasingRate = 1.0f + (sinf(arrowEasingCounter) * 0.15f);
#pragma endregion

	// 受信処理
	if (Net::GetIns().GetState() != Net::NetState::None) { ReceptionUpdate(); }
}

void BossConfirmScene::SubPostDraw(void)
{
	// 画面全体を半透明の黒で塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 画像描画のラムダ関数
	auto drawImage = [](int handle, const Vector2I& pos, float rate = 1.0f, float angle = 0.0f)->void { DrawRotaGraph(pos.x, pos.y, rate, angle, handle, true); };

	// 枠の描画
	drawImage(frameImage, FRAME_POS);

	// ボス詳細イメージ
	drawImage(bossSelectImage, BOSS_SELECT_IMAGE_POS);

	// プレイヤーアイコン
	for (char id = 0; id < ENTRY_COUNT; id++) {
		drawImage(playerIconBackImage[id], PLAYER_ICON_POS[id]);
		drawImage(playerIconImage[id], PLAYER_ICON_POS[id]);
		drawImage(playerIconFrameImage[id], PLAYER_ICON_POS[id]);
	}

	Vector2I rankingPos = Vector2I(917, 220);
	for (int rank = 0; rank < RANKING_DATA.size(); rank++) {
		std::string st = "---";
		if (RANKING_DATA.at(rank).score != -1.0f) {
			st = std::to_string(rank + 1) + "位:" + "%.2f秒";
		}
		DrawFormatStringToHandle(rankingPos.x, rankingPos.y, 0x000000, Font::GetIns().GetFont(FontKinds::MARUMINYA_40), st.c_str(), RANKING_DATA.at(rank).score);

		rankingPos.y += 85;
	}

	// 選択肢の描画
	for (int i = 0; i < (int)CHOICE::Max; i++) {
		DrawRotaGraph(CHOICE_BUTTON_POS[i].x, CHOICE_BUTTON_POS[i].y, 1, 0, choiceButtonImage[i][(int)buttonSelectionState[i]], true);
	}
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y, arrowEasingRate, 0, arrowImage, true);
	DrawRotaGraph(CHOICE_BUTTON_POS[(int)choice].x, CHOICE_BUTTON_POS[(int)choice].y - 75, 0.3f, 0, enterKeyImage[(int)Key::GetIns().LastInputKinds()], true);
}

void BossConfirmScene::SubPreRelease(void)
{
#pragma region 画像を解放

	// 選択中の決定キー
	DeleteGraph(enterKeyImage[(int)true]);
	DeleteGraph(enterKeyImage[(int)false]);

	// 選択中の矢印
	DeleteGraph(arrowImage);

	// 選択肢ボタンの画像
	for (int choiceIndex = 0; choiceIndex < (int)CHOICE::Max; choiceIndex++) {
		for (int selectionState = 0; selectionState < (int)SELECTION_STATE::Max; selectionState++) {
			DeleteGraph(choiceButtonImage[choiceIndex][selectionState]);
		}
	}

	for (char id = 0; id < ENTRY_COUNT; id++) {
		DeleteGraph(playerIconImage[id]);
		DeleteGraph(playerIconFrameImage[id]);
		DeleteGraph(playerIconBackImage[id]);
	}

	// ボス詳細イメージ
	DeleteGraph(bossSelectImage);

	// 枠の画像
	DeleteGraph(frameImage);

#pragma endregion
}

void BossConfirmScene::ButtonSelectionStateReload(void)
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
	for (int id = 0; id < (int)Net::GetIns().GetConnectStatus().EntryCount(); id++) {

		// 準備完了していないクライアントがいたら、出撃ボタンを選択不可状態にする
		if (readyList.at(id) == (unsigned char)false) {
			buttonSelectionState[(int)CHOICE::Enter] = SELECTION_STATE::Disable;
			break;
		}
	}
}

void BossConfirmScene::ReceptionUpdate(void)
{
	// 切断 の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataConnectInform>()) {

		if (dataPtr->inform == MsgDataConnectInform::INFORM_TYPE::Disconnect) {
			Net::GetIns().Disconnection();
			SceneManager::GetIns().JumpSceneFade(SCENE_ID::Lobby);
		}

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

	// 選択ボスの受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataBossSelect>()) {

		// 受け取ったボスタイプを保存する
		SceneManager::GetIns().SetSelectBossType((BOSS_TYPE)dataPtr->bossType);

		delete dataPtr;
	}

	// システム通知の受信
	while (auto dataPtr = Net::GetIns().GetMsgData<MsgDataSystemInform>(MSG_SENDER_ID::None, true)) {

		// シーン遷移の受信

		// 自身のシーン破棄
		if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::PopSceneBossConfirm) { SceneManager::GetIns().PopScene(); }

		// ゲームシーン遷移
		if (dataPtr->inform == MsgDataSystemInform::INFORM_TYPE::JumpSceneGame) { SceneManager::GetIns().JumpSceneFade(SCENE_ID::Game); }

		delete dataPtr;
	}
}