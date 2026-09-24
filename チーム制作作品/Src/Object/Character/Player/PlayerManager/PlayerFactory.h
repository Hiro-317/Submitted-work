#pragma once
#include "../PlayerCharacters/Orange/OrangePlayer.h"
#include "../PlayerCharacters/Tomato/TomatoPlayer.h"
#include "../PlayerCharacters/Peach/PeachPlayer.h"
#include "../PlayerCharacters/Grape/GrapePlayer.h"

#include "../../../CharaTypeDefine.h"
#include "../../../../Scene/SceneManager/SceneManager.h"

struct PlayerInfo
{
	PlayerBase* instance;
};

class PlayerFactory
{
public:

	// ID番目のプレイヤーが選んだキャラを生成し、returnする
	static PlayerInfo CreatePlayer(MSG_SENDER_ID id) {
		PlayerInfo ret = {};
		// プレイヤー選択画面で選択されたキャラを取得
		CHARA_TYPE chara = SceneManager::GetIns().GetSelectCharaType(id);

		// 取得したキャラを生成
		switch (chara) {
		case CHARA_TYPE::Orange: { ret.instance = new OrangePlayer(id); break; }	// オレンジ
		case CHARA_TYPE::Tomato: { ret.instance = new TomatoPlayer(id); break; }	// トマト
		case CHARA_TYPE::Peach: { ret.instance = new PeachPlayer(id); break; }	// トマト
		case CHARA_TYPE::Grape: { ret.instance = new GrapePlayer(id); break; }	// ブドウ
		default: { ret.instance = nullptr; break; }
		}

		// 生成したキャラを返す
		return ret;
	}

};