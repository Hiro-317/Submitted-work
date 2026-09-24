#pragma once

#include "../ClearCharaPreviewBase.h"

class ClearCharaPreviewOrange : public ClearCharaPreviewBase
{
public:
	ClearCharaPreviewOrange(const Vector3& pos, unsigned char operatorNumber) : ClearCharaPreviewBase(pos, operatorNumber) {}
	~ClearCharaPreviewOrange()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Orange/OrangeModel");

		// モデルのスケール
		trans.scale = 1.5f;

		// モデルのズレ
		trans.centerDiff = Vector3(0, 60, 0) * trans.scale;
	}

	void CharacterInit(void)override {
		// アニメーションを登録する
		AddInFbxAnimation(10, 1.0f, 0);
	}
};