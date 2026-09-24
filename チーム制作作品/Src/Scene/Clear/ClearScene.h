#pragma once

#include"../SceneBase.h"

class ClearScene : public SceneBase
{
public:
	ClearScene();
	~ClearScene()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;
		
	// 初期化
	void SubPostInit(void)override;

	// 更新（前）
	void SubPreUpdate(void)override;
	// 更新（後）
	void SubPostUpdate(void)override;

	// 描画
	void SubUiDraw(void)override;

	// 終了
	void SubPreRelease(void);

#pragma endregion

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

	// カメラ生成
	void CreateCamera(void)override;

	int clearTextImage;

	int clearTimeFrameImage;

	float clearTime;
};
