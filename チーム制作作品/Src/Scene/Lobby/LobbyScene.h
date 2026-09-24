#pragma once

#include <string>

#include "../SceneBase.h"

#include "../../Common/Vector2.h"

#include "../../Application/Application.h"

class LobbyScene : public SceneBase
{
public:
	LobbyScene();
	~LobbyScene() = default;

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

#pragma region 定数定義
	enum class CHOICE {
		None = -1,

		Exit,			// タイトルに戻る
		Multi,			// マルチプレイ
		CharaChange,	// キャラチェンジ
		Enter,			// 出撃

		Max
	};

	// 画像データのディレクトリ
	const std::string IMAGE_DATA_FILE_DIR = "Data/Image/Lobby/";

	// 各ボタン画像の名前
	const std::string CHOICE_BUTTON_IMAGE_NAME[(int)CHOICE::Max] = {
		"LobbyExit",		// タイトルに戻る
		"LobbyMulti",		// マルチプレイ
		"LobbyCharaChange",	// キャラチェンジ
		"LobbyEnter",		// 出撃
	};

	// ボタン画像の 選択時/非選択時 の修飾される名前
	const std::string CHOICE_BUTTON_IMAGE_DECORATION = "ToSelect";			// 選択時
	const std::string NOT_CHOICE_BUTTON_IMAGE_DECORATION = "ToNotSelect";	// 非選択時

	// ボタンの画像の位置
	const Vector2I CHOICE_BUTTON_POS[(int)CHOICE::Max] = {
		{ 100, App::GetIns().SCREEN_SIZE_Y - 100 },									// タイトルに戻る
		{ App::GetIns().SCREEN_SIZE_X - 500, App::GetIns().SCREEN_SIZE_Y - 100 },	// マルチプレイ
		{ App::GetIns().SCREEN_SIZE_X - 300, App::GetIns().SCREEN_SIZE_Y - 100 },	// キャラチェンジ
		{ App::GetIns().SCREEN_SIZE_X - 100, App::GetIns().SCREEN_SIZE_Y - 100 },	// 出撃
	};
#pragma endregion

	// 選択中のボタンのインデックス
	CHOICE choice;

	// 看板の画像
	int boardImage;

	// ボタンの画像
	int choiceButtonImage[(int)CHOICE::Max][2];

	// 選択中のボタンの上に表示する矢印の画像
	int arrowImage;

	// 選択中のボタンの上に表示する決定キーの画像
	int enterKeyImage[2];

};