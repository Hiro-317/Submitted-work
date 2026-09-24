#pragma once
#include "GrapeBossWeaponManager.h"

#include "Straight/GrapeBossStraight.h"
#include "KickBomb/GrapeBossKickBomb.h"
#include "StampBomb/GrapeBossStampBomb.h"
#include "SingleBomb/GrapeBossSingleBomb.h"
#include "StalkerBomb/GrapeBossStalkerBomb.h"
#include "RandomBomb/GrapeBossRandomBomb.h"


class GrapeBossWeaponFactory {

public:

	// インスタンスの生産
	static BombType Create(int model, int type, int front, int back) {

		// 返り値用
		BombType res;

		// タイプの取得
		res.type = (WeaponType)type;

		// タイプに応じて生産
		switch ((WeaponType)type)
		{
		case WeaponType::Straight:
		{
			res.weaponIns = new GrapeBossStraight(model, front, back);
			break;
		}
		case WeaponType::KickBomb:
		{
			res.weaponIns = new GrapeBossKickBomb(model, front, back);
			break;
		}
		case WeaponType::StampBomb:
		{
			res.weaponIns = new GrapeBossStampBomb(model, front, back);
			break;
		}
		case WeaponType::SingleBomb:
		{
			res.weaponIns = new GrapeBossSingleBomb(model, front, back);
			break;
		}
		case WeaponType::StalkerBomb:
		{
			res.weaponIns = new GrapeBossStalkerBomb(model, front, back);
			break;
		}
		case WeaponType::RandomBomb:
		{
			res.weaponIns = new GrapeBossRandomBomb(model, front, back);
			break;
		}
		default:
			break;
		}

		// 生産品を返す
		return res;
	}
};