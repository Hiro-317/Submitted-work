#pragma once

#include "../../ActorBase.h"

class BananaBossStage : public ActorBase
{
public:
	BananaBossStage();
	~BananaBossStage()override = default;

	void Load(void);

private:

#pragma region 外部ファイルから取得する各パラメータの名前

	// 配置するコライダーのサイズ（この後ろに数字がつく）
	const std::string COLLIDER_SIZE_PARAMETER_NAME = "Size";

	// 配置するコライダーの座標（この後ろに数字がつく）
	const std::string COLLIDER_POS_PARAMETER_NAME = "Pos";

	// 配置するコライダーの角度（この後ろに数字がつく）
	const std::string COLLIDER_ANGLE_PARAMETER_NAME = "Angle";

#pragma endregion

	void SubDraw(void)override;
	void SubRelease(void)override;

	int backImage;
};