#pragma once

#include "../../ActorBase.h"

#include "ClearCharaPreviewBase.h"

#include "../../CharaTypeDefine.h"

#include "../../../Manager/Net/NetWorkDefine.h"

class ClearCharaPreviewManager : public ActorBase
{
public:
	ClearCharaPreviewManager();
	~ClearCharaPreviewManager()override = default;

	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override {}
	void Release(void)override;

	// SceneManagerの持つ情報をもとに指定のIDのキャラプレビューをロード
	void LoadChara(MSG_SENDER_ID senderId);

private:
	// 現在選択中のキャラプレビュー
	ClearCharaPreviewBase* charaPreview[(int)MSG_SENDER_ID::Max];

	// ソロでの座標情報
	const Vector3 CHARA_PREVIEW_POS = Vector3::XZonly(0.0f, -300.0f);

	// マルチでの座標情報
	const Vector3 MULTI_CHARA_PREVIEW_POS[(int)MSG_SENDER_ID::Max] = {
		Vector3::XZonly(-350.0f, -300.0f),
		Vector3::XZonly(350.0f, -300.0f),
		Vector3::XZonly(-700.0f, -600.0f),
		Vector3::XZonly(700.0f, -600.0f),
	};
};