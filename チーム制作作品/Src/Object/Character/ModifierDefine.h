#pragma once

#include "../../Manager/Net/SenderID_Define.h"

#pragma region バフ/デバフ に関する定義

// バフ/デバフ の最大数
static constexpr char MODIFIER_MAX_NUM = 10;

// デバフの数値上限
static constexpr float DEBUFF_MAX = 0.01f;

// バフ/デバフ の対象の列挙型
enum class ModifierTarget {
	None = -1,

	Hp,
	Attack,
	Defense,
	Speed,
	CriticalRate,
	CriticalDamage,
};

// バフ/デバフ タイプ
enum class ModifierType
{
	None = -1,

	TackleChargeMaxBuff,

	PeachPlayerSkillBuff,

	Max
};

// バフ/デバフ タイプと対象の列挙型の対応表
static constexpr ModifierTarget MODIFIER_TARGET_TABLE[(int)ModifierType::Max] =
{
	ModifierTarget::Attack,		// TackleChangeMaxBuff
	ModifierTarget::Speed,		// PeachPlayerSkillBuff
};

// バフ/デバフ タイプを識別番号へ変換する関数
static unsigned char ModifierTypeConversionId(MSG_SENDER_ID operatorSenderId, ModifierType modifierType) {

	unsigned char ret = 0;

	// 左2ビットを操作者IDの領域とする
	ret = (int)operatorSenderId << 6;

	// 左2ビットはそのままでそれ以降のビットをModifierTypeの領域とする
	ret += (int)modifierType;

	// 完成した一意の整数値を識別番号とし、結果を返す
	return ret;
}

// ModifierTypeの識別番号を操作者IDへ変換する関数
static MSG_SENDER_ID IdConversionOperatorSenderId(unsigned char modifierTypeId) {
	return (MSG_SENDER_ID)(modifierTypeId >> 6);
}

// ModifierTypeの識別番号をModifierTypeへ変換する関数
static ModifierType IdConversionModifierType(unsigned char modifierTypeId) {
	return (ModifierType)(modifierTypeId & 0b00111111);
}

// 補正倍率(バフ/デバフ)の構造体
struct ModifierData
{
	// ID（同IDの重複不可）
	unsigned char id;

	// 増減率(バフ/デバフ)の数値（0.0が基準値）（生成関数を通して基準値を補正 例:80->0.8 -80->-0.8）
	float rate;
	// 効果時間（フレーム数）
	short time;

	// 効果時間の更新（戻り値：「true」= 効果時間継続、「false」= 効果時間終了）
	bool TimeUpdate(void) {
		if (--time > 0) { return true; }
		else { return false; }
	}

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="id">ID（同IDの重複不可）</param>
	/// <param name="rate">補正倍率(バフ/デバフ)の数値（0が基準値 例:80->1.8倍 -80->0.2倍）</param>
	/// <param name="time">効果時間（フレーム数）</param>
	ModifierData(unsigned char id, short rate, short time) : id(id), rate(((float)rate * 0.01f)), time(time) {}

	ModifierData(void) :id(0), rate(0.0f), time(0) {}
};

#pragma endregion
