#pragma once

#include "../LobbyCharaPreviewBase.h"

class LobbyCharaPreviewGrape : public LobbyCharaPreviewBase
{
public:
	LobbyCharaPreviewGrape(const Vector3& pos, unsigned char operatorNumber) : LobbyCharaPreviewBase(pos, operatorNumber) {}
	~LobbyCharaPreviewGrape()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Grape/GrapeModel");

		// モデルのスケール
		trans.scale = 2.5f;

		AddInFbxAnimation(11, 1.0f, 0);

		// 操作者を示すイメージの高さ
		operatorImagePos = 600.0f;
		for (Vector3& pos : ownOperatorImagePos) { pos.y = 600.0f; }
	}
};