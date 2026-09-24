#pragma once

#include "../CharaSelectPreviewBase.h"

class TomatoCharaSelectPreview : public CharaSelectPreviewBase
{
public:
	TomatoCharaSelectPreview() :
		CharaSelectPreviewBase("Data/Parameter/Character/Player/Tomato/")
	{
	}
	~TomatoCharaSelectPreview()override = default;

private:

	void SubLoad(void)override {
		// ÉÇÉfÉãÇì«Ç›çûÇﬁ
		trans.Load("Character/Tomato/Tomato");


		trans.scale = 2.5f;
		trans.centerDiff = Vector3::Yonly(-200);

		CHARA_NAME = "ÉäÉRÉsÉì";


		HP = GetParameterToInt("TomatoParameter", "PlayerHP");
		ATTACK = GetParameterToInt("TomatoParameter", "PlayerAttackPower");
		DEFENCE = GetParameterToInt("TomatoParameter", "PlayerDefensePower");
		SPEED = GetParameterToInt("TomatoParameter", "PlayerMoveSpeed");

		skill_1_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill1.png");
		skill_2_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill2.png");
		skill_3_image = LoadGraph("Data/Image/Lobby/CharaSelect/Tomato/Skill3.png");
	}
};