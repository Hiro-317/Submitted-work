#include "LobbyCharaPreviewBase.h"

#include "../../../Manager/Net/NetWorkManager.h"

#include "../../Common/AnimationController/AnimationController.h"

LobbyCharaPreviewBase::LobbyCharaPreviewBase(const Vector3& pos, unsigned char operatorNumber) :
	ActorBase(),

	anime(nullptr),
	playAnimeType(0),

	OPERATOR_IMAGE_PATH("Data/Image/Lobby/P" + std::to_string(operatorNumber) + ".png"),
	operatorImage(-1),
	operatorImagePos(600.0f)
{
	trans.pos = pos;
}

void LobbyCharaPreviewBase::AnimationControllerCreate(void)
{
	// 生成していなければアニメーションコントローラーを生成
	if (anime == nullptr) { anime = new AnimationController(trans.model); }
}

void LobbyCharaPreviewBase::Load(void)
{
	if (Net::GetIns().GetState() != Net::NetState::None) {
		operatorImage = LoadGraph(OPERATOR_IMAGE_PATH.c_str());
	}

	CharacterLoad();
}

void LobbyCharaPreviewBase::SubInit(void)
{
#pragma region 基底クラスにある機能の挙動設定
	// 動的オブジェクトとしての挙動を無効にする
	SetDynamicFlg(false);
	// 衝突時の押し出しを無効にする
	SetPushFlg(false);
#pragma endregion

	// アニメーション再生
	if (anime) { anime->Play(playAnimeType); }
}

void LobbyCharaPreviewBase::Update(void)
{
	// アニメーション更新
	if (anime) { anime->Update(); }
}

void LobbyCharaPreviewBase::SubDraw(void)
{
	if (operatorImage != -1) {
		DrawBillboard3D((trans.pos + Vector3::Yonly(operatorImagePos)).ToVECTOR(), 0.5f, 0.5f, 200.0f, 0.0f, operatorImage, true);
	}
}

void LobbyCharaPreviewBase::OwnOperatorDraw(int image)
{
	DrawBillboard3D((trans.pos + ownOperatorImagePos[(int)Net::GetIns().GetSenderId()]).ToVECTOR(), 0.5f, 0.5f, 200.0f, 0.0f, image, true);
}

void LobbyCharaPreviewBase::SubRelease(void)
{
	// アニメーションコントローラーを削除
	if (anime) {
		anime->Release();
		delete anime;
		anime = nullptr;
	}
	if (operatorImage != -1) {
		DeleteGraph(operatorImage);
	}
}

void LobbyCharaPreviewBase::AddInFbxAnimation(int inFbxMaxIndex, float speed, int playAnimeType)
{
	AnimationControllerCreate();
	for (int index = 0; index < inFbxMaxIndex; index++) {
		anime->AddInFbx(index, speed, index);
	}
	this->playAnimeType = playAnimeType;
}

void LobbyCharaPreviewBase::AddAnimation(const char* filePath, float speed, int index)
{
	AnimationControllerCreate();
	anime->Add(index, speed, filePath);
	playAnimeType = index;
}