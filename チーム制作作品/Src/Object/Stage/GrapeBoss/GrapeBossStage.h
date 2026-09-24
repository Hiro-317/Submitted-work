#pragma once

#include"../../ActorBase.h"

class GrapeBossStage : public ActorBase
{
private:

#pragma region 定数定義

    // 地面の当たり判定のサイズ
    const Vector3 GROUND_COLLISION_SIZE = Vector3(10077.034f, 70.0f, 9234.112f);
    // モデルの描画位置と当たり判定の中心座標の差分
    const Vector3 MODEL_CENTER_DIFF = Vector3::Yonly(-105.0f);

    // 壁の高さ
    const float WALL_HEIGHT = 1000.0f;

    struct ColliderInfo
    {
        Vector3 pos;    // 座標
        Vector3 size;   // 大きさ
        float yAngle;    // 壁の角度
	};

    const std::vector<ColliderInfo> WALL_COLLISION_INFO =
    {
        { GetParameterToVector3("Collider","WallCollPos1"), GetParameterToVector3("Collider","WallCollSize"), Deg2Rad(GetParameter("Collider","WallCollYAngle1")) },
        { GetParameterToVector3("Collider","WallCollPos2"), GetParameterToVector3("Collider","WallCollSize"), Deg2Rad(GetParameter("Collider","WallCollYAngle2")) },
        { GetParameterToVector3("Collider","WallCollPos3"), GetParameterToVector3("Collider","WallCollSize"), Deg2Rad(GetParameter("Collider","WallCollYAngle3")) },
        { GetParameterToVector3("Collider","WallCollPos4"), GetParameterToVector3("Collider","WallCollSize"), Deg2Rad(GetParameter("Collider","WallCollYAngle4")) },
        { GetParameterToVector3("Collider","WallCollPos5"), GetParameterToVector3("Collider","WallCollSize"), Deg2Rad(GetParameter("Collider","WallCollYAngle4")) },
        { GetParameterToVector3("Collider","WallCollPos6"), GetParameterToVector3("Collider","WallCollSize"), Deg2Rad(GetParameter("Collider","WallCollYAngle3")) },
        { GetParameterToVector3("Collider","WallCollPos7"), GetParameterToVector3("Collider","WallCollSize"), Deg2Rad(GetParameter("Collider","WallCollYAngle2")) },
        { GetParameterToVector3("Collider","WallCollPos8"), GetParameterToVector3("Collider","WallCollSize"), Deg2Rad(GetParameter("Collider","WallCollYAngle1")) },
    };

#pragma endregion
            
public:

    GrapeBossStage();              // コンストラクタ
    ~GrapeBossStage() = default;   // デストラクタ

    void Load(void)override;    // 読み込み

protected:

    void SubDraw(void)override;
    void SubRelease(void)override;

private:

    void ColliderLoad();    // 当たり判定の読み込み

    Transform sky;      // スカイドーム用
};