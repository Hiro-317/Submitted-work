#include "ClearCharaPreviewBase.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../Common/AnimationController/AnimationController.h"

ClearCharaPreviewBase::ClearCharaPreviewBase(const Vector3& pos, unsigned char operatorNumber) :
	ActorBase(),

	anime(nullptr),
	playAnimeType(0)
{
	trans.pos = pos;
}

void ClearCharaPreviewBase::AnimationControllerCreate(void)
{
	// 生成していなければアニメーションコントローラーを生成
	if (anime == nullptr) { anime = new AnimationController(trans.model); }
}

void ClearCharaPreviewBase::Load(void)
{
	CharacterLoad();
}

void ClearCharaPreviewBase::SubInit(void)
{
#pragma region 基底クラスにある機能の挙動設定
	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(false);
	// 衝突時の押し出しを無効にする
	SetPushFlg(false);
#pragma endregion

	CharacterInit();

	// アニメーション再生
	if (anime) { anime->Play(playAnimeType); }
}

void ClearCharaPreviewBase::Update(void)
{
	// アニメーション更新
	if (anime) { anime->Update(); }
}

void ClearCharaPreviewBase::SubRelease(void)
{
	// アニメーションコントローラーを削除
	if (anime) {
		anime->Release();
		delete anime;
		anime = nullptr;
	}
}

void ClearCharaPreviewBase::AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType)
{
	AnimationControllerCreate();
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed, index);
	}
	this->playAnimeType = playAnimeType;
}

void ClearCharaPreviewBase::AddAnimation(const char* filePath, float speed, int index)
{
	AnimationControllerCreate();
	anime->Add(index, speed, filePath);
	playAnimeType = index;
}