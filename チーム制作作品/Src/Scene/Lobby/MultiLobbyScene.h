#pragma once

#include <string>

#include "../SceneBase.h"

#include "../../Common/Vector2.h"

#include "../../Application/Application.h"

#include "../../Manager/Net/NetWorkDefine.h"

class MultiLobbyScene : public SceneBase
{
public:
	MultiLobbyScene();
	~MultiLobbyScene() = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 初期化
	void SubPostInit(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubPostDraw(void)override;

	// 終了
	void SubPreRelease(void);

#pragma endregion

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

	// カメラ生成
	void CreateCamera(void)override;

	// ホストかどうか
	const bool IS_HOST;

#pragma region 定数定義
	enum class CHOICE {
		None = -1,

		Disconnected,	// 切断
		CharaChange,	// キャラチェンジ
		Enter,			// <ホスト>出撃 / <クライアント>準備完了

		Max
	};

	enum class SELECTION_STATE {
		None = -1,

		NotSelect = (int)false,	// 非選択中
		Select = (int)true,		// 選択中

		Disable,				// 選択できない(クライアントの準備完了を押したときの)

		Max
	};

	// 画像データのディレクトリ
	const std::string IMAGE_DATA_FILE_DIR = "Data/Image/Lobby/";

	// 各ボタン画像の名前
	const std::string CHOICE_BUTTON_IMAGE_NAME[(int)CHOICE::Max] = {
		"LobbyExit",								// 切断
		"LobbyCharaChange",							// キャラチェンジ
		(IS_HOST) ? "LobbyEnter" : "LobbyReady",	// <ホスト>出撃 / <クライアント>準備完了
	};

	// ボタン画像の 選択時/非選択時/選択できないとき の修飾される名前
	const std::string CHOICE_BUTTON_IMAGE_DECORATION[(int)SELECTION_STATE::Max] = {
		"ToNotSelect",	// 非選択時
		"ToSelect",		// 選択時
		"ToDisable",	// 選択できないとき
	};

	// ボタンの画像の位置
	const Vector2I CHOICE_BUTTON_POS[(int)CHOICE::Max] = {
		{ 100, App::GetIns().SCREEN_SIZE_Y - 100 },									// 切断
		{ App::GetIns().SCREEN_SIZE_X - 300, App::GetIns().SCREEN_SIZE_Y - 100 },	// キャラチェンジ
		{ App::GetIns().SCREEN_SIZE_X - 100, App::GetIns().SCREEN_SIZE_Y - 100 },	// <ホスト>出撃 / <クライアント>準備完了
	};

	// ボードの位置
	const Vector2I BOARD_POS = { App::GetIns().SCREEN_SIZE_X_HALF, 115 };
#pragma endregion

	// 選択中のボタンのインデックス
	CHOICE choice;

	// ボタンごとの選択状態
	SELECTION_STATE buttonSelectionState[(int)CHOICE::Max];

	// <クライアント>準備完了
	std::vector<unsigned char> readyList;

	// ボタンごとの選択状態を choiceとreadyList を参照して更新する
	void ButtonSelectionStateReload(void);

	// 看板の画像
	int boardImage;

	// ボタンの画像
	int choiceButtonImage[(int)CHOICE::Max][(int)SELECTION_STATE::Max];

	// 選択中のボタンの上に表示する矢印の画像
	int arrowImage;

	// 選択中のボタンの上に表示する決定キーの画像
	int enterKeyImage[2];

	// 受信処理をまとめた関数
	void ReceptionUpdate(void);
};