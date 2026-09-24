#pragma once

#include"../../SceneBase.h"

#include "../../../Manager/Net/NetWorkDefine.h"

class GamePause : public SceneBase
{
public:
	GamePause(MSG_SENDER_ID operatorSenderId = MSG_SENDER_ID::None);
	~GamePause()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 初期化
	void SubPostInit(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubPreDraw(void)override;

	// 終了
	void SubPreRelease(void);

#pragma endregion

	bool IsUseLoadingScreen(void)const { return false; }

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }

	enum class SELECT { YES, NO, MAX };
	SELECT nowSelect;
	int img[(int)SELECT::MAX];

	MSG_SENDER_ID operatorSenderId;
	bool isOperator;
};
