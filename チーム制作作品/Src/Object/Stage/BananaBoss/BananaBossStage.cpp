#include "BananaBossStage.h"

#include "../../../Application/Application.h"

#include "../../Common/Collider/BoxCollider.h"

BananaBossStage::BananaBossStage() : ActorBase("Data/Parameter/Stage/Banana/"), backImage(-1) {}

void BananaBossStage::Load(void)
{
#pragma region オブジェクト設定

	// 静的オブジェクトに設定
	SetDynamicFlg(false);

	// 押し出しを有効にする
	SetPushFlg(true);

#pragma endregion

#pragma region モデル

	// モデル読み込み
	trans.Load("Stage/BananaBoss/BanagonStage");

	// スケール
	trans.scale = GetParameterToVector3("Model", "Scale");

	// モデルのズレ
	trans.centerDiff = GetParameterToVector3("Model", "CenterDiff") * trans.scale;
	trans.localAngle = GetParameterToVector3("Model", "LocalAngle");

	// 座標
	trans.pos = GetParameterToVector3("Model", "Pos");

	// 角度
	trans.angle = GetParameterToVector3("Model", "Angle");

#pragma endregion

#pragma region コライダー

	// コライダー生成カウント
	unsigned short colliderNum = 1;

	// 受け取った文字列にコライダー生成カウントをくっつけた文字列をつくるラムダ関数
	auto CollNumName = [&colliderNum](const std::string& st)->std::string { return (st + std::to_string(colliderNum)); };

	// パラメーターがある限り生成し続ける
	while (true) {

		// このパラメータがあるかチェック、なければそれ以降の番号もないとみなしコライダー生成を終了する
		if (!IsParameterExist("Collider", COLLIDER_POS_PARAMETER_NAME + std::to_string(colliderNum))) { break; }

		// コライダー生成
		ColliderCreate(
			new BoxCollider(
				COLLIDER_TAG::STAGE,
				GetParameterToVector3("Collider", CollNumName(COLLIDER_SIZE_PARAMETER_NAME)) * trans.scale,
				GetParameterToVector3("Collider", CollNumName(COLLIDER_POS_PARAMETER_NAME)) * trans.scale,
				GetParameterToVector3("Collider", CollNumName(COLLIDER_ANGLE_PARAMETER_NAME)) * trans.scale
			)
		);

		colliderNum++;
	}

#pragma endregion

	backImage = LoadGraph("Data/Model/Stage/BananaBoss/BackImage.png");
}

void BananaBossStage::SubDraw(void)
{
	DrawRotaGraph(App::SCREEN_SIZE_X_HALF, App::SCREEN_SIZE_Y_HALF, 1, 0, backImage, true);
}

void BananaBossStage::SubRelease(void)
{
	DeleteGraph(backImage);
}
