#pragma once

#include <string>

#include "../SceneBase.h"

#include "../../Application/Application.h"

#include "../../Common/Vector2.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene()override = default;

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

#pragma region 定数定義

	// 選択肢
	enum class SELECT {
		None = -1,

		Play,
		Option,
		Exit,

		Max
	};

	// 各キャラ
	enum class CHARA {
		None = -1,

		Grape,
		Tomato,
		Peach,
		Orange,

		Max
	};

#pragma endregion


#pragma region 定数定義（各画像のパス）

	// タイトルリソースファイルまでのパス
	const std::string TITLE_RESORCE_FILE_PATH = "Data/Image/Title/";

	// 枠
	const std::string FRAME_IMAGE_PATH = "Frame.png";
	// 背景
	const std::string BACK_IMAGE_PATH = "Back.png";

	// タイトルロゴ
	const std::string LOGO_IMAGE_PATH = "TitleLogo.png";

	// 太陽
	const std::string SUN_IMAGE_PATH = "Sun.png";
	// 気球
	const std::string BALLOON_IMAGE_PATH = "Balloon.png";
	// 飛行船
	const std::string FLYING_BOAT_IMAGE_PATH = "FlyingBoat.png";
	// 雲
	const std::string CLOUD_IMAGE_PATH = "Cloud.png";
	// バナゴン
	const std::string BANAGON_IMAGE_PATH = "Banagon.png";

	// 選択肢
	const std::string SELECT_IMAGE_PATH[(int)SELECT::Max] = {
		"SelectPlay.png",
		"SelectOption.png",
		"SelectExit.png",
	};

	// キャラクター
	const std::string CHARA_IMAGE_PATH[(int)CHARA::Max] = {
		"Grape.png",
		"Tomato.png",
		"Peach.png",
		"Orange.png",
	};

#pragma endregion
#pragma region 定数定義（各画像の 座標/初期座標）

	// 枠
	const Vector2I FRAME_POS = Vector2I(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF);
	// 背景
	const Vector2I BACK_POS = FRAME_POS;

	// タイトルロゴ
	const Vector2I LOGO_POS = FRAME_POS + Vector2I(0, -170);

	// 太陽
	const Vector2I SUN_POS = Vector2I(176, 168);
	// 気球
	const Vector2I BALLOON_POS = Vector2I(353, 120);
	// 飛行船
	const Vector2I FLYING_BOAT_POS = Vector2I(1273, 166);

	// 雲を流す個数（最後の1個はバナゴンに置き換わる）
	static constexpr char CLOUD_NUM = 5;

	// 雲を流す最大速度
	const int CLOUD_MOVE_MAX_SPEED = 5;
	// バナゴンが雲の何倍の速度で動くか
	const int BANAGON_MOVE_SPEED_RATE = 2;

	// 雲（最後の1個はバナゴンに置き換わる）
	const Vector2I CLOUD_INIT_POS[CLOUD_NUM] = {
		Vector2I(App::SCREEN_SIZE_X / CLOUD_NUM * 1, App::SCREEN_SIZE_Y_HALF / CLOUD_NUM * 1),
		Vector2I(App::SCREEN_SIZE_X / CLOUD_NUM * 2, App::SCREEN_SIZE_Y_HALF / CLOUD_NUM * 2),
		Vector2I(App::SCREEN_SIZE_X / CLOUD_NUM * 3, App::SCREEN_SIZE_Y_HALF / CLOUD_NUM * 3),
		Vector2I(App::SCREEN_SIZE_X / CLOUD_NUM * 4, App::SCREEN_SIZE_Y_HALF / CLOUD_NUM * 4),
		Vector2I(App::SCREEN_SIZE_X / CLOUD_NUM * 5, App::SCREEN_SIZE_Y_HALF / CLOUD_NUM * 5),
	};

	// 選択肢
	const Vector2I SELECT_POS[(int)SELECT::Max] = {
		Vector2I(App::SCREEN_SIZE_X_HALF, 475),
		Vector2I(App::SCREEN_SIZE_X_HALF, 575),
		Vector2I(App::SCREEN_SIZE_X_HALF, 675),
	};

	// キャラクター
	const Vector2I CHARA_POS[(int)CHARA::Max] = {
		Vector2I(1240, 445),
		Vector2I(185, 435),
		Vector2I(1060, 565),
		Vector2I(415, 560),
	};

#pragma endregion

	// 現在選択している選択肢
	SELECT nowSelect;

#pragma region 各画像ハンドル
	// 枠
	int frameImage;
	// 背景
	int backImage;

	// タイトルロゴ
	int logoImage;

	// 太陽
	int sunImage;
	// 気球
	int balloonImage;
	// 飛行船
	int flyingBoatImage;
	// 雲
	int cloudImage;
	// バナゴン
	int banagonImage;

	// 選択肢
	int selectImage[(int)SELECT::Max];

	// キャラクター
	int charaImage[(int)CHARA::Max];
#pragma endregion

#pragma region 演出用の変数/関数

	// 太陽のイージングカウンター
	float sunEasingCounter;
	// 太陽のイージング数値
	float sunEasingRotate;

	// 気球のイージングカウンター
	float balloonEasingCounter;
	// 気球のイージング数値
	float balloonEasingRotate;

	// 飛行船のイージングカウンター
	float flyingBoatEasingCounter;
	// 飛行船のイージング数値
	float flyingBoatEasingRotate;

	// キャラのイージングカウンター
	float charaEasingCounter;
	// キャラのイージング数値
	float charaEasingRotate[(int)CHARA::Max];

	// 雲の演出用移動方向
	Vector2I cloudVec[CLOUD_NUM];
	// 雲の座標（最後の1個はバナゴンに置き換わる）
	Vector2I cloudPos[CLOUD_NUM];

	// 選択肢のイージングカウンター
	float selectEasingCounter;
	// 選択肢のイージング数値
	float selectEasingRate;

	// ロゴのイージングカウンター
	float logoEasingCounter;
	// ロゴのイージング数値
	float logoEasingRate;

	// 演出更新
	void EasingUpdate(void);

#pragma endregion

};