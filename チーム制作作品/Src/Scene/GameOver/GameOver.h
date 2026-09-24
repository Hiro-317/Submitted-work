#pragma once

#include"../SceneBase.h"

class GameOver : public SceneBase
{
public:
	GameOver();
	~GameOver()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubPostDraw(void)override;

	// 終了
	void SubPreRelease(void);

#pragma endregion

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

	int image;
};
