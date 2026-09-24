#include "GrapeBossWeaponManager.h"

#include "../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../Manager/Effect/EffectManager.h"

#include "GrapeBossWeaponFactory.h"


GrapeBossWeaponManager* GrapeBossWeaponManager::ins = nullptr;

GrapeBossWeaponManager::GrapeBossWeaponManager(const MSG_SENDER_ID& operatorSenderId, const CharacterStats& stats)
	: operatorSenderId(operatorSenderId), stats(stats)
{
	// 複製ハンドル
	bombModel = MV1LoadModel("Data/Model/Character/Grape/Bomb.mv1");
	frontModel = MV1LoadModel("Data/Model/Range/CircleRangeFront.mv1");
	backModel = MV1LoadModel("Data/Model/Range/CircleRangeBack.mv1");

	// 初期化用の数
	int weaponNumber = 0;

	// 武器の種類数とその中身の数まで回す
	for (int i = 0; i < (int)WeaponType::Max; i++) {
		for (int j = 0; j < WeponDuplicateNum[i]; j++) {

			// 作ってもらった武器を格納
			weapons[weaponNumber] = GrapeBossWeaponFactory::Create(bombModel, i, frontModel, backModel);

			// 数の更新
			weaponNumber++;
		}
	}
	// 複製用ハンドルの消去
	MV1DeleteModel(bombModel);
}

void GrapeBossWeaponManager::Load(void)
{
	// 武器の初期化
	for (auto& i : weapons) i.weaponIns->Load(operatorSenderId, stats);
}

void GrapeBossWeaponManager::SubUpdate()
{
	for (auto& i : weapons) {
		// 生きているもののみ更新
		if (i.live) {
			i.weaponIns->Update();

			// 終了しているなら殺す
			if (i.weaponIns->IsEnd()) {
				
				i.live = false;
				i.weaponIns->ResetEnd();
				Snd::GetIns().Play("Exprosion");
			}
			// 例外処理
			if (i.weaponIns->GetTrans().pos < EXIT_HEIGHT) {

				i.live = false;
				i.weaponIns->ResetEnd();
			}
		}
	}
}

void GrapeBossWeaponManager::SubAlphaDraw()
{
	for (auto& i : weapons) {
		if (i.live) {
			i.weaponIns->Draw();
		}
	}
}

void GrapeBossWeaponManager::SubRelease()
{
	for (auto& i : weapons) {
		i.weaponIns->Release();
		delete i.weaponIns;
		i.weaponIns = nullptr;
	}
	MV1DeleteModel(frontModel);
	MV1DeleteModel(backModel);
}