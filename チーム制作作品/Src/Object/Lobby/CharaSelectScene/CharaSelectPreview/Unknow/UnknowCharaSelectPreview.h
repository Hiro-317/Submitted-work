#pragma once

#include "../CharaSelectPreviewBase.h"

class UnknowCharaSelectPreview : public CharaSelectPreviewBase
{
public:
	UnknowCharaSelectPreview(): CharaSelectPreviewBase() {}
	~UnknowCharaSelectPreview()override = default;

private:

	void SubLoad(void)override {

		CHARA_NAME = "？？？？";

		HP = 0;
		ATTACK = 0;
		DEFENCE = 0;
		SPEED = 0;
	}
};