#pragma once

#include "../../SceneBase.h"

class MatchingLoadingScene : public SceneBase
{
public:
	MatchingLoadingScene();
	~MatchingLoadingScene()override = default;

private:

#pragma region 主要関数再定義

	// 初期化
	void SubPostInit(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubPostDraw(void)override;

#pragma endregion

	bool IsUseLoadingScreen(void)const override { return false; }

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

	// マッチングを待つ時間
	const short MATCHING_WAIT_TIME = 1000;

	// マッチング待ち計測用カウンター
	short waitCounter;
};