#pragma once

#include "../../ActorBase.h"

#include "LobbyCharaPreviewBase.h"

#include "../../CharaTypeDefine.h"

#include "../../../Manager/Net/NetWorkDefine.h"

class LobbyCharaPreviewManager : public ActorBase
{
public:
	LobbyCharaPreviewManager();
	~LobbyCharaPreviewManager()override = default;

	void Load(void)override;
	void Init(void)override {}
	void Update(void)override;
	void Draw(void)override;
	void AlphaDraw(void)override {}
	void Release(void)override;

	// SceneManagerの持つ情報をもとにキャラプレビューを更新する（ソロ用）
	void ReloadChara(void);
	// SceneManagerの持つ情報をもとに指定のIDのキャラプレビューを更新する（マルチ用）
	void ReloadChara(MSG_SENDER_ID senderId);

private:
	// 現在選択中のキャラプレビュー
	LobbyCharaPreviewBase* charaPreview[(int)MSG_SENDER_ID::Max];

	// ソロでの座標情報
	const Vector3 CHARA_PREVIEW_POS = Vector3::XZonly(0.0f, -300.0f);

	// マルチでの座標情報
	const Vector3 MULTI_CHARA_PREVIEW_POS[(int)MSG_SENDER_ID::Max] = {
		Vector3::XZonly(-350.0f, -300.0f),
		Vector3::XZonly(350.0f, -300.0f),
		Vector3::XZonly(-700.0f, -600.0f),
		Vector3::XZonly(700.0f, -600.0f),
	};

	const std::string OWN_OPERATOR_IMAGE[(int)MSG_SENDER_ID::Max] = {
		"Data/Image/Lobby/RightSideOwnOperator.png",
		"Data/Image/Lobby/LeftSideOwnOperator.png",
		"Data/Image/Lobby/LeftSideOwnOperator.png",
		"Data/Image/Lobby/RightSideOwnOperator.png",
	};

	// 自分のキャラを示すイメージ
	int ownOperatorImage;
};