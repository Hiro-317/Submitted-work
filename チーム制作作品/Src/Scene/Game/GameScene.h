#pragma once

#include"../SceneBase.h"

class BossBase;
class PlayerManager;

class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene()override = default;

private:

	// 読み込み
	void SubPostLoad(void)override;
	// 初期化（メイン処理の後）
	void SubPostInit(void)override;
	// 更新（前）
	void SubPreUpdate(void)override;
	// 更新（後）
	void SubPostUpdate(void)override;

	void SubUiDraw(void)override;

	void SubPostRelease(void)override;

	void CreateCamera(void)override;

	const BossBase* bossInsRef;
	const PlayerManager* playerInsRef;

	// カメラ状態変更フラグ
	bool focusFlg;

	// 1フレームの秒数
	const float FRAME_TIME = 1.0f / 60.0f;
	// 時間計測
	float time;

	// マルチで時間をどのくらいの間隔で同期するか（秒数）
	const char MULTI_TIME_SEND_COUNT = 5;
};