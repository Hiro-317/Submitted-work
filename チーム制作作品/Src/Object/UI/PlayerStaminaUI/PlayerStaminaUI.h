#pragma once
#include "../../../pch.h"
#include <algorithm>

#include "../UI_Base.h"

#include "../../../Common/Vector2.h"
#include "../../../Application/Application.h"

class PlayerStaminaUI : public UI_Base
{
private:

	const int STAMINA_MAX;
	static const int IMAGE_MAX = 7;
	const int IMAGE_X_NUM = 7;
	const int IMAGE_Y_NUM = 1;

	const int IMAGE_X_SIZE = 50;
	const int IMAGE_Y_SIZE = 50;

	const Vector2I DEFAULT_POS = Vector2I(App::SCREEN_SIZE_X_HALF + 80, App::SCREEN_SIZE_Y_HALF);

public:

	/// <summary>
	/// スタミナのUIのコンストラクタ
	/// </summary>
	/// <param name="stamina">スタミナの現在地</param>
	/// <param name="STAMINA_MAX">スタミナの最大値</param>
	PlayerStaminaUI(const short& stamina, const short STAMINA_MAX);
	// デストラクタ
	~PlayerStaminaUI();

	void Load(void) override;	// ロード
	void SubInit(void)override {}
	void SubUpdate(void)override {}
	void SubDraw(void)override;	// 描画
	void SubRelease(void)override;	// 解放

private:

	// スタミナの割合で配列を動かす
	int GetStaminaIndex(int stamina, int STAMINA_MAX, int& index) {
		float rate = (float)stamina / (float)STAMINA_MAX;
		if (rate == 1.0f) { return false; }
		index = (int)(rate * IMAGE_MAX + 0.5);
		index = std::clamp(index, 0, IMAGE_MAX - 1);
		return true;
	}

	const short& stamina;	// スタミナ量

	Vector2I pos;			// 描画座標
	int images[IMAGE_MAX];	// スタミナUIの描画

};