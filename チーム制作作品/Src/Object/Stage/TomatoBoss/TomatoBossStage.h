#pragma once

#include"../../ActorBase.h"

class TomatoBossStage : public ActorBase
{
private:

#pragma region 定数定義

    // 地面の当たり判定のサイズ
    const Vector3 GROUND_COLLISION_SIZE = Vector3(10077.034f, 50.0f, 9234.112f);
    // モデルの描画位置と当たり判定の中心座標の差分
    const Vector3 MODEL_CENTER_DIFF = Vector3::Yonly(-80.0f);
    // モデルの描画位置
    const Vector3 WALL_LOCAL_ENDPOS = Vector3(0, -100, 0);
    // モデルの描画位置
    const Vector3 ROCK_LOCAL_ENDPOS = Vector3(0, -100, 0);

    struct ColliderInfo
    {
        Vector3 pos;    // 座標
        float radius;   // 半径
	};

    const std::vector<ColliderInfo> WALL_COLLISION_INFO =
    {
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos1") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius1") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos2") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius2") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos3") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius3") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos4") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius4") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos5") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius5") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos6") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius6") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos7") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius7") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos8") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius8") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos9") + MODEL_CENTER_DIFF,   GetParameter("TomatoStageParameter", "WallCollisionRadius9") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos10") + MODEL_CENTER_DIFF,  GetParameter("TomatoStageParameter", "WallCollisionRadius10") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos11") + MODEL_CENTER_DIFF,  GetParameter("TomatoStageParameter", "WallCollisionRadius11") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos12") + MODEL_CENTER_DIFF,  GetParameter("TomatoStageParameter", "WallCollisionRadius12") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos13") + MODEL_CENTER_DIFF,  GetParameter("TomatoStageParameter", "WallCollisionRadius13") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos14") + MODEL_CENTER_DIFF,  GetParameter("TomatoStageParameter", "WallCollisionRadius14") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos15") + MODEL_CENTER_DIFF,  GetParameter("TomatoStageParameter", "WallCollisionRadius15") },
        { GetParameterToVector3("TomatoStageParameter", "WallCollisionPos16") + MODEL_CENTER_DIFF,  GetParameter("TomatoStageParameter", "WallCollisionRadius16") },
    };

#pragma endregion
            
public:

    TomatoBossStage();              // コンストラクタ
    ~TomatoBossStage() = default;   // デストラクタ

    void Load(void)override;    // 読み込み

protected:

    void SubDraw(void)override;
    void SubRelease(void)override;

private:

    void ColliderLoad();    // 当たり判定の読み込み

    Transform sky;      // スカイドーム用
};