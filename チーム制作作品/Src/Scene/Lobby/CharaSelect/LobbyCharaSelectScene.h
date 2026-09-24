#pragma once

#include <functional>

#include "../../SceneBase.h"

#include "../../../Object/CharaTypeDefine.h"

class LobbyCharaSelectScene : public SceneBase
{
public:
	LobbyCharaSelectScene(
		const std::function<void(void)>& LobbyPreviewCharaChange,
		const std::function<void(void)>& LobbyReceptionUpdate = nullptr
	);
	~LobbyCharaSelectScene()override = default;

private:

#pragma region 主要関数再定義

	// 読み込み
	void SubPostLoad(void)override;

	// 更新
	void SubPostUpdate(void)override;

	// 描画
	void SubPreDraw(void)override;

#pragma endregion

	bool IsUseLoadingScreen(void)const override { return false; }

	// 当たり判定管理は使用しない
	bool UseCollisionManager(void)const override { return false; }
	
	// ロビー画面のキャラプレビューを更新する関数のポインターを受け取る
	const std::function<void(void)> LobbyPreviewCharaChange;

	// マルチロビー画面の受信処理を受け取る
	const std::function<void(void)> LobbyReceptionUpdate;

	// 選択中のキャラタイプ
	CHARA_TYPE selectCharaType;
};