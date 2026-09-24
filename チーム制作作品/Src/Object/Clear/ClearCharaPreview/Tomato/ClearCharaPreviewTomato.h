#pragma once

#include "../ClearCharaPreviewBase.h"

class ClearCharaPreviewTomato : public ClearCharaPreviewBase
{
public:
	ClearCharaPreviewTomato(const Vector3& pos, unsigned char operatorNumber) : ClearCharaPreviewBase(pos, operatorNumber) {}
	~ClearCharaPreviewTomato()override = default;;

private:

	void CharacterLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Tomato/Tomato");

		// モデルのスケール
		trans.scale = 2.5f;
	}
};