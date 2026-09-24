#include "BossSelectSpotManager.h"

#include "../../../Manager/Input/KeyManager.h"

#include "Tomato/BossSelectTomatoSpot.h"
#include "Grape/BossSelectGrapeSpot.h"
#include "Banana/BossSelectBananaSpot.h"

BossSelectSpotManager::BossSelectSpotManager() :
	ActorBase(),

	spot(),
	selectedImage()
{
}

void BossSelectSpotManager::Load(void)
{
	selectedImage[(int)true] = LoadGraph("Data/Image/BossSelect/BossSelectEnterController.png");
	selectedImage[(int)false] = LoadGraph("Data/Image/BossSelect/BossSelectEnterKeyboard.png");

	// スポット生成ラムダ関数
	auto CreatSpot = [&](BossSelectSpotBase* newSpot)->void {

		// 安全処理
		if (newSpot == nullptr) { return; }

		// 追加
		spot.emplace_back(newSpot);

		// 読み込み
		spot.back()->Load();

		};

	CreatSpot(new BossSelectTomatoSpot());
	CreatSpot(new BossSelectGrapeSpot());
	CreatSpot(new BossSelectBananaSpot());
}

void BossSelectSpotManager::SubInit(void)
{
	for (BossSelectSpotBase* s : spot) { s->Init(); }
}

void BossSelectSpotManager::SubUpdate(void)
{
	for (BossSelectSpotBase* s : spot) { s->Update(); }
}

void BossSelectSpotManager::SubDraw(void)
{
	for (BossSelectSpotBase* s : spot) { s->Draw(); }
	for (BossSelectSpotBase* s : spot) { s->SelectedDraw(selectedImage[(int)Key::GetIns().LastInputKinds()]); }
}

void BossSelectSpotManager::SubAlphaDraw(void)
{
	for (BossSelectSpotBase* s : spot) { s->AlphaDraw(); }
}

void BossSelectSpotManager::SubRelease(void)
{
	for (BossSelectSpotBase*& s : spot) {
		if (s == nullptr) { continue; }
		s->Release();
		delete s;
		s = nullptr;
	}
	DeleteGraph(selectedImage[(int)true]);
	DeleteGraph(selectedImage[(int)false]);
}	

std::vector<ColliderBase*> BossSelectSpotManager::GetCollider(void)const {
	std::vector<ColliderBase*> ret = {};

	for (BossSelectSpotBase* const& s : spot) {
		for (ColliderBase* coll : s->GetCollider()) { ret.emplace_back(coll); }
	}

	return ret;
}

