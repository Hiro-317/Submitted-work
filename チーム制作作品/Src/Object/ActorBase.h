#pragma once

#include <string>
#include <vector>
#include <map>

#include "Common/Transform.h"

#include "Common/DataLoad/ParameterLoad.h"

#include "../Manager/Net/SenderID_Define.h"

#include "Common/Collider/ColliderTagDefine.h"

#include "Character/ModifierDefine.h"

class ColliderBase;

struct CharacterStats;
struct SkillStats;


class ActorBase
{
public:
	// デフォルトコンストラクタ
	ActorBase();
	// パラメーターを外部から読み込む場合に使うコンストラクタ
	ActorBase(const std::string& parameterPath);
	virtual ~ActorBase() = default;

	virtual void Load(void) = 0;
	virtual void Init(void);
	virtual void ReceptionUpdate(void) {}
	virtual void Update(void);
	virtual void SendUpdate(void) {}
	virtual void Draw(void);
	virtual void AlphaDraw(void);
	virtual void UiDraw(void) {}
	virtual void Release(void);

	// モデルを複製する
	void ModelDuplicate(int model) { trans.model = MV1DuplicateModel(model); }

	// モデル制御情報構造体のゲット関数
	const Transform& GetTrans(void)const { return trans; }

	// 当たり判定の通知
	virtual void OnCollision(COLLIDER_TAG ownTag, const ColliderBase& other,const Vector3& collisionPoint) {}

	// 接地判定の通知
	virtual void OnGrounded() {
		if (dynamicFlg) {
			accelSum.y = (accelSum.y < 0.0f) ? 0.0f : accelSum.y;
		}
		isGroundMaster = true;
	}

	/// <summary>
	/// コライダーすべてを取得
	/// </summary>
	virtual std::vector<ColliderBase*> GetCollider(void)const { return collider; }

	/// <summary>
	/// 当たり判定フラグの取得
	/// </summary>
	/// <returns>どれか一つでも「判定する」状態ならtrue</returns>
	bool GetJudgeFlg(void);

	// 描画判定の取得
	bool GetIsDraw(void)const { return isDraw; }

private:
	// 当たり判定情報
	std::vector<ColliderBase*> collider;

	// 動的オブジェクトか否か（true = 動的、false = 静的）
	bool dynamicFlg;

	// 重力を適用するかどうか(true = する、false = しない)
	bool isGravity;

	// 衝突時押し出しを行うか否か（true = 押し出す、false = 押し出さず通り抜ける）
	bool pushFlg;

	// 押し出しを行う際の重さ（0 ～ 100 で設定）
	unsigned char pushWeight;

	// 加速度の更新
	void AccelUpdate(void);

	// 重力更新
	void Gravity(void);

	// 重力
	static constexpr float GRAVITY = -0.6f;
	static constexpr float GRAVITY_MAX = -30.0f;

	// 接地判定 管理用(派生先で変更不可で参照渡し)
	bool isGroundMaster;

	// 描画判定 （true = 「描画する」、false = 「描画しない」）
	bool isDraw;

	// アルファ判定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	bool isAlphaDraw;

	// パラメーター外部ファイル管理クラス
	ParameterLoad* parameter;

	// スキル構造体
	std::vector<SkillStats*> skillStats;

protected:
	// モデル制御情報構造体
	Transform trans;

	// 加速度
	Vector3 accelSum;

	// 横軸加速度の１フレームごとの減衰量
	float ATTENUATION = 3.0f;
	// 横軸加速度の最大値
	float ACCEL_MAX = 30.0f;

	// 接地判定(派生先で参照用)
	const bool& isGround = isGroundMaster;

#pragma region 初期設定
	// 当たり判定情報を生成
	void ColliderCreate(ColliderBase* newClass);

	/// <summary>
	/// 移動するかを切り替える
	/// </summary>
	/// <param name="flg">true = 「移動する」に切り替える、false = 「移動しない」に切り替える</param>
	void SetDynamicFlg(bool flg);

	/// <summary>
	/// 重力を適用するかを切り替える
	/// </summary>
	/// <param name="flg">true = 「する」に切り替える、false = 「しない」に切り替える</param>
	void SetGravityFlg(bool flg) { isGravity = flg; }

	/// <summary>
	/// 衝突時押し出しを行うかを設定する
	/// </summary>
	/// <param name="flg">true = 押し出す、false = 押し出さず通り抜ける</param>
	void SetPushFlg(bool flg);

	/// <summary>
	/// 押し出しを行う際の重さ
	/// </summary>
	/// <param name="weight">0 ～ 100 で設定（数値が大きいほど重い）</param>
	void SetPushWeight(unsigned char weight);

#pragma region パラメーター外部ファイル管理に関する関数

	bool IsParameterExist(const std::string& fileName, const std::string& parameterName)const;

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターの指定の配列番号の値だけを取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <param name="index">配列番号（指定なしで0）</param>
	/// <returns></returns>
	float GetParameter(const std::string& fileName, const std::string& parameterName, int index = 0)const;

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターを配列ごと取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	const std::vector<float>& GetParameterArray(const std::string& fileName, const std::string& parameterName)const;

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターの指定の配列番号の値だけをint型にキャストして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <param name="index">配列番号（指定なしで0）</param>
	/// <returns></returns>
	int GetParameterToInt(const std::string& fileName, const std::string& parameterName, int index = 0)const;

	/// <summary>
	/// パラメーター外部ファイル管理クラスから指定のパラメーターをVector3構造体にして取得する
	/// </summary>
	/// <param name="parameterName">パラメーターのID</param>
	/// <returns></returns>
	Vector3 GetParameterToVector3(const std::string& fileName, const std::string& parameterName);

#pragma endregion

#pragma region スキル設定

	// コライダーにスキル詳細を設定
	void ColliderToSetSkill(void);

	// 攻撃スキル詳細生成
	void CreateAttackSkill(MSG_SENDER_ID operatorSenderId, short SKILL_POWER, const CharacterStats* characterStats, COLLIDER_TAG tag = COLLIDER_TAG::NON);

	// 回復スキル詳細生成
	void CreateHealSkill(MSG_SENDER_ID operatorSenderId, short SKILL_POWER, COLLIDER_TAG tag = COLLIDER_TAG::NON);

	// バフ/デバフスキル詳細生成
	void CreateModifierSkill(MSG_SENDER_ID operatorSenderId, ModifierType modifierType, short SKILL_POWER, short SKILL_TIME, COLLIDER_TAG tag = COLLIDER_TAG::NON);

#pragma endregion

#pragma endregion

	// 当たり判定の設定（true = 「判定する」、false = 「判定しない」）
	void SetJudge(bool flg);

	bool GetDynamicFlg(void)const { return dynamicFlg; }
	bool GetGravityFlg(void)const { return isGravity; }

	// 描画判定の設定（true = 「描画する」、false = 「描画しない」）
	void SetIsDraw(bool flg) { isDraw = flg; }
	// 描画判定の設定（引数省略で現在の逆にスイッチ）
	void SetIsDraw(void) { isDraw = !isDraw; }

	// アルファ判定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	bool GetIsAlphaDraw(void)const { return isAlphaDraw; }

	// アルファ判定の設定（true = 「メインの描画にアルファをかける」、false = 「メインの描画を普通に描画する」）（メインの描画 = 基底クラスで自動で描画されるモデルなど）
	void SetIsAlphaDraw(bool flg) { isAlphaDraw = flg; }
	// アルファ判定の設定（引数省略で現在の逆にスイッチ）
	void SetIsAlphaDraw(void) { isAlphaDraw = !isAlphaDraw; }

	// 派生先追加初期化
	virtual void SubInit(void) {}
	// 派生先追加更新
	virtual void SubUpdate(void) {}
	// 派生先追加描画
	virtual void SubDraw(void) {}
	// 派生先追加アルファ描画
	virtual void SubAlphaDraw(void) {}
	// 派生先追加解放
	virtual void SubRelease(void) {}
};