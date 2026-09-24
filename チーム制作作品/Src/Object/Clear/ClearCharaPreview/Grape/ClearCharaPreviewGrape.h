#pragma once

#include "../ClearCharaPreviewBase.h"

class ClearCharaPreviewGrape : public ClearCharaPreviewBase
{
public:
	ClearCharaPreviewGrape(const Vector3& pos, unsigned char operatorNumber) : ClearCharaPreviewBase(pos, operatorNumber) {}
	~ClearCharaPreviewGrape()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Grape/GrapeModel");

		// モデルのスケール
		trans.scale = 2.5f;
	}

	void CharacterInit(void)override {
		AddInFbxAnimation(11, 1.0f, 0);
	}
};