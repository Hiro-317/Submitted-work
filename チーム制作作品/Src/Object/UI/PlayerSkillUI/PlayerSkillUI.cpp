#include "PlayerSkillUI.h"

PlayerSkillUI::PlayerSkillUI(
	Vector2I pos, 
	const int& coolTimeCounter, 
	int COOL_TIME, 
	SKILL_UI_COLOR color,
	std::string skillImagePath,
	const bool isChargeSkill
) :
	pos(pos),
	COOL_TIME(COOL_TIME),
	coolTimeRatio(0.0f),
	offset(0.0f),
	coolTimeCounter(coolTimeCounter),
	isChargeSkill(isChargeSkill)
{
	// 画像の読み込み
	UILoadImage("SkillSlotFrame", (int)IMAGE_TYPE::FRAME, FILE_PATH_TYPE::PLAYER_SKILL);
	UILoadImage(skillImagePath, (int)IMAGE_TYPE::SKILL, FILE_PATH_TYPE::PLAYER_SKILL);
	UILoadImage(chargeImagePath.find(color)->second.c_str(), (int)IMAGE_TYPE::COLOR_IMAGE1, FILE_PATH_TYPE::PLAYER_SKILL);
	UILoadImage(chargingImagePath.find(color)->second.c_str(), (int)IMAGE_TYPE::COLOR_IMAGE2, FILE_PATH_TYPE::PLAYER_SKILL);
	UILoadImage("ChargeOK", (int)IMAGE_TYPE::CHARGE_OK, FILE_PATH_TYPE::PLAYER_SKILL);
}

void PlayerSkillUI::SubUpdate()
{
	// クールタイムの量に応じて増減する
	coolTimeRatio = (float)coolTimeCounter / (float)COOL_TIME;
	offset = IMAGE_SIZE.y * (1.0f - coolTimeRatio);
}

void PlayerSkillUI::SubDraw(void)
{
	// スキルUIのフレーム画像
	DrawGraph(
		pos.x - (IMAGE_SIZE.x / 2),
		pos.y - (IMAGE_SIZE.y / 2),
		uiImages.at((int)IMAGE_TYPE::FRAME),
		true
	);

	int offset_I = (int)offset;
	
	// クールタイム中、クールタイムの割合に応じてこの画像を描画する
	DrawRectGraph(
		pos.x - (IMAGE_SIZE.x / 2),
		(pos.y + IMAGE_SIZE.y / 2) - offset_I,
		0, IMAGE_SIZE.y - offset_I,
		IMAGE_SIZE.x, offset_I,
		uiImages.at((int)IMAGE_TYPE::COLOR_IMAGE1),
		true
	);

	// スキルが使える場合の時に描画する画像
	if (coolTimeRatio <= 0.0f) {
		DrawRotaGraph(
			pos.x,
			pos.y,
			1.0f,
			0.0f,
			uiImages.at((int)IMAGE_TYPE::COLOR_IMAGE2),
			true
		);

		if (isChargeSkill) {
			// 溜め攻撃スキルの場合、スキルが使える状態の時に描画する画像
			DrawRotaGraph(
				pos.x,
				pos.y - (IMAGE_SIZE.y / 3) * 2,
				0.1f,
				0.0f,
				uiImages.at((int)IMAGE_TYPE::CHARGE_OK),
				true
			);
		}
	}

	// スキルの特徴を示す画像
	DrawRotaGraph(
		pos.x,
		pos.y,
		1.0f,
		0.0f,
		uiImages.at((int)IMAGE_TYPE::SKILL),
		true
	);
}

void PlayerSkillUI::SubRelease(void)
{ 
}
