#pragma once

#include "../CharaSelectPreviewBase.h"

class GrapeCharaSelectPreview : public CharaSelectPreviewBase
{
public:
	GrapeCharaSelectPreview() :
		CharaSelectPreviewBase("Data/Parameter/Character/Player/Grape/")
	{
	}
	~GrapeCharaSelectPreview()override = default;

private:

	void SubLoad(void)override {
		// モデルを読み込む
		trans.Load("Character/Grape/GrapeModel");

		trans.scale = 2;
		trans.centerDiff = Vector3::Yonly(-250);

		// アニメーションを登録する
		AddInFbxAnimation(11, 1.0f, 0);

		CHARA_NAME = "ブしドウ";

		HP = GetParameterToInt("GrapeParameter", "PlayerHP");
		ATTACK = GetParameterToInt("GrapeParameter", "PlayerAttackPower");
		DEFENCE = GetParameterToInt("GrapeParameter", "PlayerDefensePower");
		SPEED = GetParameterToInt("GrapeParameter", "PlayerMoveSpeed");

		skill_1_image = LoadGraph("Data/Image/Lobby/CharaSelect/Grape/Skill1.png");
		skill_2_image = LoadGraph("Data/Image/Lobby/CharaSelect/Grape/Skill2.png");
		skill_3_image = LoadGraph("Data/Image/Lobby/CharaSelect/Grape/Skill3.png");
	}
};