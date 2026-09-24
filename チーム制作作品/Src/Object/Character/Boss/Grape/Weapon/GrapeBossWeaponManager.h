#pragma once
#include "GrapeBossWeaponBase.h"
#include "../../../../ActorBase.h"

struct CharacterStats;

static constexpr int ArraySum(const int* const array, unsigned char arrayNum) {
	int ret = 0;
	for (int i = 0; i < arrayNum; i++) { ret += array[i]; }
	return ret;
}

struct BombType {

	WeaponType type = WeaponType::None;
	GrapeBossWeaponBase* weaponIns = nullptr;
	bool live = false;
};

class GrapeBossWeaponManager : public ActorBase{
public:

	GrapeBossWeaponManager(const MSG_SENDER_ID& operatorSenderId, const CharacterStats& stats);
	~GrapeBossWeaponManager() = default;

	// ほしいボムタイプの配列で返す
	BombType* GetWeapons(WeaponType type) {

		// 配列分出す
		for (auto& weapon : weapons) {
			// 配列内のタイプとほしいものが同じなら
			if (weapon.type == type) {
				// 格納
				return &weapon;
			}
		}
		// ないならぬるぽ
		return nullptr;
	}

	void Load(void) override;

	// 状態遷移後1度行う初期化処理
	std::vector<ColliderBase*> GetCollider(void)const override {
		std::vector<ColliderBase*> ret = {};
		// 自身のコライダーを返却用一時変数に格納
		for (ColliderBase*& coll : ActorBase::GetCollider()) { ret.emplace_back(coll); }

		// 抱える下位クラスの返却用一時変数に格納
		for (auto& subObj : weapons) {
			for (ColliderBase*& coll : subObj.weaponIns->GetCollider()) { ret.emplace_back(coll); }
		}
		// 最終的な返却用一時変数を返却
		return ret;
	}

protected:

	void SubUpdate(void) override;
	void SubAlphaDraw(void) override;
	void SubRelease(void) override;

private:

	static GrapeBossWeaponManager* ins;

	// 例外処理用
	static constexpr float EXIT_HEIGHT = -10.0f;

#pragma region 参照値

	const MSG_SENDER_ID& operatorSenderId;
	const CharacterStats& stats;

#pragma endregion

	// 複製用ハンドル
	int bombModel;
	int backModel;
	int frontModel;

	// 武器の配列
	BombType weapons[ArraySum(WeponDuplicateNum, (int)WeaponType::Max)];
};
