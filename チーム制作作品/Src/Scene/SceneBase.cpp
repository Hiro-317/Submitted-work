#include "SceneBase.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "../pch.h"

#include "../Application/Application.h"

#include "../Common/Vector2.h"

#include "../Manager/Camera/CameraBase.h"
#include "../Manager/Collision/CollisionManager.h"
#include "../Manager/Effect/EffectManager.h"
#include "../Object/ActorBase.h"

int SceneBase::hitStop = 0;

int SceneBase::slow = 0;
int SceneBase::slowInter = 1;
int SceneBase::slowCounter = 0;

int SceneBase::shake = 0;
SceneBase::ShakeKinds SceneBase::shakeKinds = ShakeKinds::DIAG;
SceneBase::ShakeSize SceneBase::shakeSize = ShakeSize::MEDIUM;

SceneBase::SceneBase(void) :

	state(STATE::CREATED),

	mainScreen(-1),

	camera(nullptr),
	collision(nullptr),

	objects()
{
}

void SceneBase::Load(void)
{
	// 同じシーンへLoadが2回呼ばれた場合、モデルや画像の二重ロードにつながるため検出する
	if (state != STATE::CREATED) { throw std::logic_error("SceneBase::Load()が不正な状態で呼ばれました"); }

	// 派生先の読み込み（前）
	SubPreLoad();

	// 画面揺れを適用するため、一度このスクリーンへ3D描画をまとめる
	mainScreen = MakeScreen(App::SCREEN_SIZE_X, App::SCREEN_SIZE_Y, true);
	if (mainScreen < 0) { throw std::runtime_error("SceneBaseのメインスクリーン生成に失敗しました"); }

	// シーンごとに独立した当たり判定管理クラスを生成する
	if (UseCollisionManager()) { collision = new CollisionManager(); }

	// 派生先の読み込み（後）
	// Actorの生成はCollisionManager生成後に行う必要があるため、基本的にはここで行う
	SubPostLoad();

	// 全Actorが追加された後、巨大な静的オブジェクト等のチャンクを一度構築する
	if (collision != nullptr) { collision->InitBuildChunks(); }

	state = STATE::LOADED;
}

void SceneBase::Init(void)
{
	if (state != STATE::LOADED) { throw std::logic_error("SceneBase::Init()がLoad完了前、または二重に呼ばれました"); }

	// 派生先の初期化（前）
	SubPreInit();

	// シーンごとに独立したカメラを生成する
	CreateCamera();
	// カメラ初期化
	if (camera != nullptr) { camera->Init(); }

	// シーンが所有するActorをすべて初期化する
	for (ActorBase* obj : objects) { obj->Init(); }

	// 派生先の初期化（後）
	SubPostInit();

	state = STATE::INITIALIZED;
}

void SceneBase::Update(void)
{
	// ロード中や解放済みのシーンは更新しない
	if (state != STATE::INITIALIZED) { return; }

	// 入力判定やシーン固有の事前処理
	SubPreUpdate();

	// ヒットストップ・スローによりゲーム本体を更新しないフレーム
	if (!IsUpdateFrame()) { return; }

	// カメラ情報をDxLibへ反映
	if (camera != nullptr) { camera->Apply(); }

	// オブジェクト全ての受信処理
	for (ActorBase* obj : objects) { obj->ReceptionUpdate(); }
	// オブジェクト全ての更新処理
	for (ActorBase* obj : objects) { obj->Update(); }

	// 当たり判定更新
	if (collision != nullptr) { collision->Check(); }

	// オブジェクト全ての送信処理
	for (ActorBase* obj : objects) { obj->SendUpdate(); }

	// 当たり判定による押し出し後に行う共通処理
	SubPostUpdate();

	// カメラ更新
	if (camera != nullptr) { camera->Update(); }
}

void SceneBase::Draw(void)
{
	if (state != STATE::INITIALIZED || mainScreen < 0) { return; }

#pragma region 画面揺れ用スクリーンへ描画

	// 3D部分を一度mainScreenへ描画する
	SetDrawScreen(mainScreen);
	ClearDrawScreen();

	// カメラ情報をDxLibへ反映
	if (camera != nullptr) { camera->Apply(); }
	Effekseer_Sync3DSetting();

#pragma endregion

#pragma region メイン描画

	// 通常描画
	SubPreDraw();
	for (ActorBase* obj : objects) { obj->Draw(); }
	SubPostDraw();

	// 半透明描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	for (ActorBase* obj : objects) { obj->AlphaDraw(); }
	SubAlphaDraw();

	// デバッグ用チャンク描画
	if (collision != nullptr && camera != nullptr) { collision->DrawChunkGrid(camera->GetPos()); }

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// Effekseer描画
	DrawEffekseer3D_Begin(); 
	EffectManager::GetIns()->DrawEffect();
	DrawEffekseer3D_End();

#pragma endregion

#pragma region mainScreenを画面へ転送

	SetDrawScreen(DX_SCREEN_BACK);

	// 描画座標
	const Vector2I shakePoint = GetShakePoint();
	// 画面が揺れている場合は、画面端に黒い線が出ないように裏にもう1枚描画する
	if (shakePoint != 0) { DrawGraph(0, 0, mainScreen, true); }
	// 画面揺れを反映してmainScreenを描画する
	DrawGraph(shakePoint.x, shakePoint.y, mainScreen, true);

#pragma endregion

#pragma region UI描画

	// UIは画面揺れの影響を受けないよう、mainScreen転送後に描画する
	for (ActorBase* obj : objects) { obj->UiDraw(); }
	SubUiDraw();

	if (camera != nullptr) { camera->DrawDebug(); }

#pragma endregion
}

void SceneBase::Release(void)
{
	// デストラクタとSceneManagerの両方から呼ばれても二重解放しない
	if (state == STATE::RELEASED) { return; }

	// 派生先の解放（前）
	SubPreRelease();

	// 全てのオブジェクトを解放
	for (ActorBase*& obj : objects) {
		obj->Release();
		delete obj;
		obj = nullptr;
	}
	objects.clear();

	// 当たり判定管理解放
	if (collision != nullptr) {
		collision->Clear();
		delete collision;
		collision = nullptr;
	}

	// カメラ解放
	if(camera != nullptr) {
		camera->Release();
		delete camera;
		camera = nullptr;
	}

	// 画面演出用のスクリーン解放
	if (mainScreen >= 0) {
		DeleteGraph(mainScreen);
		mainScreen = -1;
	}

	// 派生先の解放（後）
	SubPostRelease();

	state = STATE::RELEASED;
}

void SceneBase::ObjAdd(ActorBase* newObj)
{
	// 安全処理
	if (newObj == nullptr) { return; }

	// Actor共通読み込み
	newObj->Load();

	// Actorが持つコライダーをCollisionManagerへ登録
	if (collision != nullptr) { collision->Add(newObj->GetCollider()); }

	// Actorをシーンの所有リストへ追加する
	objects.emplace_back(newObj);
}

void SceneBase::HitStop(int time)
{
	hitStop = (std::max)(0, time);
}

void SceneBase::Slow(int time, int inter)
{
	slow = (std::max)(0, time);
	slowInter = (std::max)(1, inter);
	slowCounter = 0;
}

void SceneBase::Shake(ShakeKinds kinds, ShakeSize size, int time)
{
	// 短時間に何度も同じ揺れが来た場合、毎フレーム最初からやり直さないようにする
	if (std::abs(shake - time) > 10 || shake <= 0)
	{
		shake = (std::max)(0, time);
	}

	shakeKinds = kinds;
	shakeSize = size;
}

bool SceneBase::IsUpdateFrame(void)
{
	// ヒットストップ中はActor / Collision / Cameraの更新を止める
	if (hitStop > 0)
	{
		hitStop--;
		return false;
	}

	// スロー中は指定間隔に1回だけ更新する
	if (slow > 0)
	{
		slow--;
		slowCounter = (slowCounter + 1) % slowInter;
		return slowCounter == 0;
	}

	return true;
}

Vector2I SceneBase::GetShakePoint(void)
{
	Vector2I ret = {};

	if (shake <= 0) { return ret; }

	// 5フレームごとに正負を反転させる
	const int direction = ((shake / 5) % 2) * 2 - 1;
	const int power = static_cast<int>(shakeSize);

	switch (shakeKinds)
	{
	case ShakeKinds::WID:
		ret.x = direction * power;
		break;

	case ShakeKinds::HIG:
		ret.y = direction * power;
		break;

	case ShakeKinds::DIAG:
		ret.x = direction * power;
		ret.y = direction * power;
		break;

	case ShakeKinds::ROUND:
	{
		// DxLibの三角関数はラジアンを使用する
		const float angle = static_cast<float>((shake % 12) * 30) * DX_PI_F / 180.0f;
		const float radius = static_cast<float>(power) * 1.5f;

		ret.x = static_cast<int>(radius * std::cos(angle));
		ret.y = static_cast<int>(radius * std::sin(angle));
		break;
	}
	default:
		break;
	}

	shake--;
	return ret;
}
