#pragma once
#include "../ActorBase.h"
#include "../Common/AnimationController/AnimationController.h"
#include "CharacterStateBase.h"
#include "../../Manager/Net/NetWorkDefine.h"

#include "../../Application/Application.h"

#include <map>

class UI_Base;

class CharacterBase : public ActorBase
{
public:

	CharacterBase();

	CharacterBase(
		short HP_MAX,
		short ATTACK_POWER,
		short DEFENSE_POWER,
		short SPEED_POWER
	);

	CharacterBase(const std::string& parameterPath);

	// パラメーターを外部から読み込む場合に使うコンストラクタ
	CharacterBase(
		const std::string fileName,
		const std::string hpParameterID,
		const std::string attackPowerParameterID,
		const std::string defensePowerParameterID,
		const std::string moveSpeedParameterID,
		const std::string& parameterPath
	);
	virtual ~CharacterBase()override = default;

	// ステートのゲット関数
	int GetState(void)const { return state; }
	const bool& GetIsDeath(void)const { return isDeath; }
	void SetIsDeath(bool flg) { isDeath = flg; }

	const CharacterStats& GetCharacterStats(void)const { return characterStats; }

	void Load(void)override;
	void UiDraw(void)override;

private:

	// 初期化
	void SubInit(void)override;
	// 更新
	void SubUpdate(void)override;
	// 描画
	void SubDraw(void)override;
	void SubAlphaDraw(void)override;
	// 解放
	void SubRelease(void)override;

	// モデルデフォルトカラー保存用配列
	std::vector<COLOR_F> DEFAULT_COLOR;

	// アニメーションコントローラーのインスタンス
	AnimationController* anime;

	// 無敵カウンター
	unsigned char inviCounter;
	
	// 無敵カウンターの更新
	void Invi(void);

	// 無敵カウンターによるダメージ演出を行うかどうか（true = 「する」、false = 「しない」）← デフォルトは「する」
	bool isInviEffect;

	// ステート管理用マップ（キー：ステート番号、値：状態クラスのポインタ）
	std::map<int, CharacterStateBase*> stateMap;

	// 死んだかどうか
	bool isDeath;

protected:

	// UI管理用インスタンス
	std::vector<UI_Base*> ui_ArrayIns;

	// ステート管理用変数
	int state;

	// 影が出るキャラと座標
	std::map<COLLIDER_TAG, Vector3> shadowPos;

	// ステートの追加
	void AddState(int stateNum, CharacterStateBase* stateIns) { stateMap.emplace(stateNum, stateIns); }

	// 指定のステートインスタンスをゲットする関数
	CharacterStateBase& GetStateIns(int state) {
		auto it = stateMap.find(state);
		if (it != stateMap.end()) { return *(it->second); }
		else { throw std::runtime_error("指定のステートインスタンスが見つかりません"); }
	}

	// 指定のステートへ遷移
	virtual void  ChangeState(int nextState) {
		// 遷移前のステートの終了処理を呼び出す
		if (stateMap.contains(state)) { stateMap.at(state)->Exit(); }

		// 遷移
		state = nextState;

		// 遷移後のステートの初期化処理を呼び出す
		if (stateMap.contains(state)) { stateMap.at(state)->Enter(); }
	}

	// キャラクター固有の処理をここに追加
	virtual void CharacterLoad(void) {}
	virtual void CharacterInit(void) {}
	virtual void CharacterUpdate(void) {}
	virtual void CharacterRemoteUpdate() {}
	virtual void CharacterDraw(void) {}
	virtual void CharacterAlphaDraw(void) {}
	virtual void CharacterUiDraw(void) {}
	virtual void CharacterRelease(void) {}


#pragma region アニメーションコントローラー
	// アニメーションコントローラーの作成
	void CreateAnimationController(void) { if (anime == nullptr) anime = new AnimationController(trans.model); }
	
	/// <summary>
	/// モデルにくっついてるFBXアニメーションを全部登録する
	/// </summary>
	/// <param name="inFbxMaxIndex">くっついてるアニメーションの数</param>
	/// <param name="speed">再生速度</param>
	void AddInFbxAnimation(int inFbxMaxIndex, float speed);

	void AddInFbxAnimation(int inFbxMaxIndex, const float* speed);

	/// <summary>
	/// 別ファイルに保存されているFBXアニメーションを登録する
	/// </summary>
	/// <param name="index">参照番号</param>
	/// <param name="speed">再生速度</param>
	/// <param name="filePath">パス</param>
	void AddAnimation(int index, float speed, const char* filePath);

	virtual void AnimePlay(int type, bool loop = true);

	bool IsAnimeEnd(void)const { return anime->GetAnimEnd(); }

	float GetAnimeRatio(void)const { return anime->GetAnimeRatio();}
	float GetAnimeTotalTime(void)const { return anime->GetAnimeTotalTime();}

	int GetAnimePlayType(void)const { return anime->GetPlayType(); }

	float GetAnimeStep(void)const { return anime->GetStep(); }
	void SetAnimeStep(float step) { anime->SetStep(step); }
	
#pragma endregion オーバーライド不可(使用のみ)

	// 抱える下位クラスを格納する配列の中から特定のオブジェクトを探す
	template<typename SubUiClass = UI_Base>
	SubUiClass* SubUiSerch(void) {
		for (UI_Base* ui : ui_ArrayIns) {
			if (dynamic_cast<SubUiClass*>(ui)) { return dynamic_cast<SubUiClass*>(ui); }
		}
		return nullptr;
	}

	// 無敵カウンターのゲット関数
	unsigned char GetInviCounter(void)const { return inviCounter; }
	// 無敵カウンターのセット関数
	void SetInviCounter(unsigned char counter = 1);

	// 無敵演出フラグのセット関数（true = 「する」、false = 「しない」）← デフォルトは「する」
	void SetInviEffectFlg(bool flg = true) {
		if (isInviEffect == flg) { return; }

		isInviEffect = flg;

		// 無敵演出フラグが変わったとき色をデフォルトに戻す

		// 無敵演出フラグが「する」に変わったときはモデルデフォルトカラーを保存しておく
		int mnum = MV1GetMaterialNum(trans.model);
		for (int i = 0; i < mnum; ++i) {
			COLOR_F emi = MV1GetMaterialEmiColor(trans.model, i);

			emi.r = (std::min)(emi.r + 0.4f, 1.0f);
			emi.g = (std::min)(emi.g + 0.4f, 1.0f);
			emi.b = (std::min)(emi.b + 0.4f, 1.0f);

			// モデルデフォルトカラーの保存
			if (isInviEffect) { DEFAULT_COLOR.emplace_back(emi); }

			MV1SetMaterialEmiColor(trans.model, i, emi);
		}

		// 「しない」に変わったときは必要ないので情報を破棄する
		if (!isInviEffect) { DEFAULT_COLOR.clear(); }
	}

	// ステータス
	CharacterStats characterStats;

	// 誰が操作権を持つか
	MSG_SENDER_ID operatorSenderId;

	// 自分が操作しているかどうか
	bool isOwnOperator;

};