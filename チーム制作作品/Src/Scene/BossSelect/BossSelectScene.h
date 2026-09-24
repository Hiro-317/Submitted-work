#pragma once

#include "../SceneBase.h"

class BossSelectScene : public SceneBase
{
public:
	BossSelectScene();
	~BossSelectScene()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	void SubPostInit(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubUiDraw(void)override;

	// 解放
	void SubPostRelease(void)override;
#pragma endregion

	void CreateCamera(void)override;

	int boardImage;

	int operatorGuideImage[2];
};