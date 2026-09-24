#pragma once

#include "../LobbyCharaPreviewBase.h"

class LobbyCharaPreviewPeach : public LobbyCharaPreviewBase
{
public:
	LobbyCharaPreviewPeach(const Vector3& pos, unsigned char operatorNumber) : LobbyCharaPreviewBase(pos, operatorNumber) {}
	~LobbyCharaPreviewPeach()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Peach/PeachModel");

		// ライティングの調整
		MV1SetSpcColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		MV1SetDifColorScale(trans.model, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));

		// モデルのスケール
		trans.scale = 2.5f;

		AddInFbxAnimation(7, 1.0f, 0);

		// 操作者を示すイメージの高さ
		operatorImagePos = 550.0f;
		for (Vector3& pos : ownOperatorImagePos) { pos.y = 550.0f; }
	}
};