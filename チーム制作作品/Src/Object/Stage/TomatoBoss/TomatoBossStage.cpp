#include "TomatoBossStage.h"

#include "../../Common/Collider/BoxCollider.h"
#include "../../Common/Collider/CapsuleCollider.h"
#include "../../Common/Collider/SphereCollider.h"
#include "../../Common/Collider/XZCircleCollider.h"

TomatoBossStage::TomatoBossStage() :
	ActorBase("Data/Parameter/Stage/"), sky()
{
}

void TomatoBossStage::Load(void)
{
	trans.Load("Stage/TomatoBoss/TomatoBossStage");

	sky.Load("Stage/Sky");

	// ステージのモデルの座標の補正
	trans.centerDiff = MODEL_CENTER_DIFF;

	sky.scale = Vector3(4.0f);

	for (int index = 0; index < MV1GetFrameNum(trans.model); index++) {
		std::string name = MV1GetFrameName(trans.model, index);
		if (name.find("Rock") != std::string::npos) {

			MV1SetFrameVisible(trans.model, index, false);
		}
	}

#pragma region 基底クラスにある機能の挙動設定

	// 動的オブジェクトとしての挙動を有効にする
	SetDynamicFlg(false);

	// 衝突時の押し出しを有効にする
	SetPushFlg(true);

#pragma endregion
	
	// 衝突判定の情報読み込み
	ColliderLoad();
}

void TomatoBossStage::SubDraw(void)
{
	SetUseLighting(false);
	sky.Draw();
	SetUseLighting(true);
}

void TomatoBossStage::SubRelease(void)
{
	sky.Release();
}

void TomatoBossStage::ColliderLoad()
{
	// 地面の当たり判定
	ColliderCreate(new BoxCollider(COLLIDER_TAG::STAGE, GROUND_COLLISION_SIZE));

	// ステージの壁の当たり判定
	for (const ColliderInfo& info : WALL_COLLISION_INFO) {
		ColliderCreate(new XZCircleCollider(COLLIDER_TAG::STAGE, info.radius, 1000.0f, info.pos));
	}
}


