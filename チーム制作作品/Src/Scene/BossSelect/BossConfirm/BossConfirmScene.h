#pragma once

#include "../../SceneBase.h"

#include <string>
#include <functional>

#include "../../../Application/Application.h"

#include "../../../Object/CharaTypeDefine.h"

#include "../../../Manager/Net/NetWorkDefine.h"

#include "../../../Manager/Ranking/Ranking.h"

class BossConfirmScene : public SceneBase
{
public:

	BossConfirmScene();
	~BossConfirmScene()override = default;

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
	void SubPreRelease(void)override;

#pragma endregion

	// ロードをはさまない
	bool IsUseLoadingScreen(void)const override { return false; }

	// 当たり判定管理クラスを使用しない
	bool UseCollisionManager(void)const override { return false; }

#pragma region 定数定義

	// ホストかどうか
	const bool IS_HOST;

	// プレイヤー接続数
	const char ENTRY_COUNT;

	// 選択択
	enum class CHOICE {
		None = -1,

		Exit,	// キャンセル
		Enter,	// 出撃

		Max
	};

	// 選択状態
	enum class SELECTION_STATE {
		None = -1,

		NotSelect = (int)false,	// 非選択中
		Select = (int)true,		// 選択中

		Disable,				// 選択できない(クライアントの準備完了を押したときの)

		Max
	};

	// 画像データのディレクトリ
	const std::string IMAGE_DATA_FILE_DIR = "Data/Image/BossSelect/BossConfirm/";

	// 枠の画像の名前
	const std::string FRAME_IMAGE_NAME = "Frame";

	const std::vector<Ranking::RankingData>& RANKING_DATA;

	// 各ボスの詳細イメージの名前
	const std::string BOSS_SELECT_IMAGE_NAME[(int)BOSS_TYPE::Max] = {
		"TomatoBossSelectImage",
		"GrapeBossSelectImage",
		"BananaBossSelectImage",
	};

	// パーティー編成に使用するプレイヤーアイコン（背景）の画像の名前
	const std::string PLAYER_ICON_BACK_IMAGE_NAME[(int)MSG_SENDER_ID::Max] = {
		"P1Back",
		"P2Back",
		"P3Back",
		"P4Back",
	};

	// パーティー編成に使用するプレイヤーアイコン（フレーム）の画像の名前
	const std::string PLAYER_ICON_FRAME_IMAGE_NAME[(int)MSG_SENDER_ID::Max] = {
		"P1Frame",
		"P2Frame",
		"P3Frame",
		"P4Frame",
	};

	// パーティー編成に使用するプレイヤーアイコンの画像の名前
	const std::string PLAYER_ICON_IMAGE_NAME[(int)CHARA_TYPE::Max] = {
		"OrangeIcon",
		"TomatoIcon",
		"PeachIcon",
		"GrapeIcon",
	};

	// 各ボタン画像の名前
	const std::string CHOICE_BUTTON_IMAGE_NAME[(int)CHOICE::Max] = {
		"Exit",							// キャンセル
		(IS_HOST) ? "Enter" : "Ready",	// <ホスト>出撃 / <クライアント>準備完了
	};

	// ボタン画像の 選択時/非選択時/選択できないとき の修飾される名前
	const std::string CHOICE_BUTTON_IMAGE_DECORATION[(int)SELECTION_STATE::Max] = {
		"ToNotSelect",	// 非選択時
		"ToSelect",		// 選択時
		"ToDisable",	// 選択できないとき
	};

	// 選択中矢印の画像
	const std::string ARROW_IMAGE_NAME = "NowSelectArrow";

	// 選択中決定ボタンの画像（キーボード）
	const std::string ENTER_KEYBOARD_IMAGE_NAME = "NowSelectKeyboard";
	// 選択中決定ボタンの画像（コントローラー）
	const std::string ENTER_CONTROLLER_IMAGE_NAME = "NowSelectController";

	// 枠の画像の位置
	const Vector2I FRAME_POS = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);

	// ボス詳細イメージの位置
	const Vector2I BOSS_SELECT_IMAGE_POS = FRAME_POS + Vector2I(-205, -100);

	// プレイヤーアイコンの位置
	const Vector2I PLAYER_ICON_POS[(int)MSG_SENDER_ID::Max] = {
		FRAME_POS + Vector2I(-390, 220),
		FRAME_POS + Vector2I(-270, 220),
		FRAME_POS + Vector2I(-150, 220),
		FRAME_POS + Vector2I(-30, 220),
	};

	// ボタンの画像の位置
	const Vector2I CHOICE_BUTTON_POS[(int)CHOICE::Max] = {
		{ 100, App::GetIns().SCREEN_SIZE_Y - 100 },									// 切断
		{ App::GetIns().SCREEN_SIZE_X - 100, App::GetIns().SCREEN_SIZE_Y - 100 },	// <ホスト>出撃 / <クライアント>準備完了
	};

#pragma endregion

	// 選択状態
	CHOICE choice;

	// ボタンごとの選択状態
	SELECTION_STATE buttonSelectionState[(int)CHOICE::Max];

	// <クライアント>準備完了
	std::vector<unsigned char> readyList;

	// ボタンごとの選択状態を choiceとreadyList を参照して更新する
	void ButtonSelectionStateReload(void);

	// 枠の画像
	int frameImage;

	// ボス詳細イメージ
	int bossSelectImage;

	// プレイヤーアイコン（背景）の画像
	int playerIconBackImage[(int)MSG_SENDER_ID::Max];
	// プレイヤーアイコン（フレーム）の画像
	int playerIconFrameImage[(int)MSG_SENDER_ID::Max];
	// プレイヤーアイコンの画像
	int playerIconImage[(int)MSG_SENDER_ID::Max];
	
	// 各選択択の画像
	int choiceButtonImage[(int)CHOICE::Max][(int)SELECTION_STATE::Max];

	// 選択中のボタンの上に表示する矢印の画像
	int arrowImage;

	// 選択中のボタンの上に表示する決定キーの画像
	int enterKeyImage[2];

	// 選択中のイメージ群を動かすカウンター
	float arrowEasingCounter;
	float arrowEasingRate;

	// 受信処理をまとめた関数
	void ReceptionUpdate(void);
};