#pragma once
#include "../UI_Base.h"
#include "../../../Application/Application.h"

#include <list>
#include <string>

#include "../../../Common/Vector2.h"
#include "../../../Manager/Font/FontManager.h"
#include "../../../Utility/Utility.h"

class HitUI : public UI_Base
{
public:

    HitUI(Vector3& targetPos);                // コンストラクタ
    ~HitUI() override;      // デストラクタ

    void Load(void) override;       // ロード
    void SubInit(void) override;    // 初期化
    void SubUpdate(void) override;  // 更新
    void SubDraw(void) override;    // 描画
    void SubRelease(void) override; // 解放

    /// <summary>
    /// ダメージ値の設定
    /// </summary>
    /// <param name="damage">表示するダメージ値</param>
    /// <param name="isCritical">会心ならば true / 非会心ならば false</param>
    void DamageSetting(const Vector3& collisionPos, const short damage, const bool isCritical = false);

    // ミス！を設定
    void MissSetting();

private:

    static constexpr float DAMAGE_UI_ALIVE_TIME = 60.0f;
    static constexpr float DAMAGE_UI_BOUNCE_START_TIME = DAMAGE_UI_ALIVE_TIME / 3;
    static constexpr float DAMAGE_UI_BOUNCE_TIME = DAMAGE_UI_ALIVE_TIME - DAMAGE_UI_BOUNCE_START_TIME;
    static constexpr float DAMAGE_UI_BOUNCE_POWER = 12.0f;

	const Vector3& targetPos;  // 描画する対象の座標
    
    // ダメージ描画用構造体
    struct AddDamageUiInfo
    {
        // ダメージ描画の時間更新処理
        bool AliveUpdate(int value = 1) {
            if (aliveTime < value) { return false; }
            aliveTime -= value;
            if (aliveTime <= 0) { return false; }
            return true;
        }

        // 描画処理
        void Draw(void) const
        {
            float elapsedTime = (float)(DAMAGE_UI_ALIVE_TIME - aliveTime);
            float drawPosY;

            if (elapsedTime < DAMAGE_UI_BOUNCE_START_TIME) {
                // 上昇    
                drawPosY = pos.y + DAMAGE_UI_BOUNCE_TIME - (elapsedTime * 3.0f);
            }
            else {  
                float bounceTime = elapsedTime - DAMAGE_UI_BOUNCE_START_TIME;
                float bounceRate = 1.0f - (bounceTime / DAMAGE_UI_BOUNCE_TIME);
                UtilityClamp(bounceRate, 0.0f, 1.0f);

                // バウンド
                drawPosY = pos.y - (DAMAGE_UI_BOUNCE_START_TIME + fabsf(sinf(bounceTime * 0.35f)) * DAMAGE_UI_BOUNCE_POWER * bounceRate);
            }

            int alpha = 255;

            // バウンドが終了したら徐々に消える
            if (elapsedTime >= DAMAGE_UI_BOUNCE_START_TIME) {
                float fadeRate = 1.0f - ((elapsedTime - DAMAGE_UI_BOUNCE_START_TIME) / DAMAGE_UI_BOUNCE_TIME);
                UtilityClamp(fadeRate, 0.0f, 1.0f);
                fadeRate *= fadeRate;
                alpha = (int)(255 * fadeRate);
            }

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

            // 文字の表示
            DrawFormatStringToHandle(
                (int)pos.x,
                (int)drawPosY, 
                color, 
                Font::GetIns().GetFont(FontKinds::HIT_UI_64), 
                text.c_str()
            );

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        std::string text;       // 表示するテキスト
        int color = 0xffffff;   // テキストのカラー
        short aliveTime = 0;    // テキストの生存時間
        Vector2 pos;            // テキストの座標
    };

    /// <summary>
    /// 描画するテキストと色を設定し、描画を開始する
    /// </summary>
    /// <param name="text">表示する文字の指定</param>
    /// <param name="color">表示する文字の色指定</param>
    void AddPopup(
        const Vector2 pos,
        const std::string& text,
        int color
    );

    std::list<AddDamageUiInfo> damageUiList;
};
