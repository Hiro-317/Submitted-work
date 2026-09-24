#pragma once

#include "../../../Manager/Net/NetWorkDefine.h"

#include "../../ActorBase.h"

#include "../../CharaTypeDefine.h"

class CharaSelectPreviewBase;

class CharaSelectPreviewManager : public ActorBase
{
public:
	CharaSelectPreviewManager(CHARA_TYPE selectCharacter);
	~CharaSelectPreviewManager()override = default;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override {}
	void Release(void)override;

	void ChangeCharacter(CHARA_TYPE select);

private:

#pragma region 定数定義

	const Vector2I EXIT_IMAGE_POS = Vector2I(174, 77);

#pragma endregion

	// 枠の画像
	int frameImage;

	// 矢印の画像
	int arrowImage;

	// 決定キーの画像
	int enterImage[2];

	// とじるキーの画像
	int exitImage[2];

	// 拡大演出カウンター
	float easingCounter;
	// 拡大演出の最終的な倍率
	float easingRate;

	// 選択中キャラのプレビュー
	CharaSelectPreviewBase* charaPreview;
};