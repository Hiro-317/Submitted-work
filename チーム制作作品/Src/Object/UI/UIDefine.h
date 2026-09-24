#pragma once
#include <vector>

// UI画像のファイルパスの種類
enum class FILE_PATH_TYPE
{
	PLAYER_HP,
	PLAYER_STAMINA,
	PLAYER_SKILL,

	BOSS_HP,

	MAX
};

// 種類別のファイルパス
static const std::string uiKindsPath[(int)FILE_PATH_TYPE::MAX] = {
	"Data/Image/UI/Player/HP/",
	"Data/Image/UI/Player/Stamina/",
	"Data/Image/UI/Player/Skill/",

	"Data/Image/UI/Boss/HP/",
};