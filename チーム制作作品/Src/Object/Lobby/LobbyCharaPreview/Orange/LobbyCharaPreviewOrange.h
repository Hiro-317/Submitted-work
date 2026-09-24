#pragma once

#include "../LobbyCharaPreviewBase.h"

class LobbyCharaPreviewOrange : public LobbyCharaPreviewBase
{
public:
	LobbyCharaPreviewOrange(const Vector3& pos, unsigned char operatorNumber) : LobbyCharaPreviewBase(pos, operatorNumber) {}
	~LobbyCharaPreviewOrange()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Orange/OrangeModel");

		// モデルのスケール
		trans.scale = 1.5f;

		// モデルのズレ
		trans.centerDiff = Vector3(0, 60, 0) * trans.scale;

		// アニメーションを登録する
		AddInFbxAnimation(10, 1.0f, 0);

		// 操作者を示すイメージの高さ
		operatorImagePos = 650.0f;

		for (Vector3& pos : ownOperatorImagePos) { pos.y = 650.0f; }
	}
};