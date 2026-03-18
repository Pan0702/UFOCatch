#pragma once
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "../../Common/Object3D.h"


class StageData;

class TRS : public Object3D
{
private:
    std::unique_ptr<CTranslation> m_pTranslation_;
    std::unique_ptr<CRotation> m_pRotation;
    std::unique_ptr<CScaling> m_pScaling;
    StageData* m_pStageData = nullptr;
    Transform* m_pOverrideTarget = nullptr;
    Axis m_draggingAxis = Axis::None;

    int m_state = 0;
    int m_selected = 0;
    float m_translateSpeed = 0.1f;
    float m_rotateSpeed = 1.0f;
    float m_scaleSpeed = 1.0f;
    float m_translateAccum = 0.0f;
    float m_rotateAccum = 0.0f;
private:
    /// <summary>ImGui縺ｧ遘ｻ蜍輔せ繝翫ャ繝鈴㍼繧帝∈謚槭☆繧九Λ繧ｸ繧ｪ繝懊ち繝ｳ繧呈緒逕ｻ縺吶ｋ</summary>
    void RadioTranslate();

    /// <summary>ImGui縺ｧTRS繝｢繝ｼ繝牙・譖ｿ縺ｨ蜷・ｨｭ螳啅I繧偵∪縺ｨ繧√※謠冗判縺吶ｋ</summary>
    void DrawImGui();

    /// <summary>ImGui縺ｧ蝗櫁ｻ｢繧ｹ繝翫ャ繝鈴㍼繧帝∈謚槭☆繧九Λ繧ｸ繧ｪ繝懊ち繝ｳ繧呈緒逕ｻ縺吶ｋ</summary>
    void RadioRotate();

    /// <summary>ImGui縺ｧ繧ｹ繧ｱ繝ｼ繝ｫ繧ｹ繝斐・繝峨ｒ驕ｸ謚槭☆繧九Λ繧ｸ繧ｪ繝懊ち繝ｳ繧呈緒逕ｻ縺吶ｋ</summary>
    void RadioScale();

    /// <summary>遘ｻ蜍暮㍼繧偵せ繝翫ャ繝怜腰菴阪↓荳ｸ繧√※compo縺ｫ邏ｯ遨榊刈邂励☆繧・/summary>
    /// <param name="compo">譖ｴ譁ｰ蟇ｾ雎｡縺ｮ蠎ｧ讓呎・蛻・ｼ亥盾辣ｧ・・/param>
    /// <param name="delta">莉翫ヵ繝ｬ繝ｼ繝縺ｮ遘ｻ蜍暮㍼</param>
    void SnapTranslation(float& compo, float delta);

    /// <summary>蝗櫁ｻ｢驥上ｒ繧ｹ繝翫ャ繝怜腰菴阪↓荳ｸ繧√※compo縺ｫ邏ｯ遨榊刈邂励☆繧・/summary>
    /// <param name="compo">譖ｴ譁ｰ蟇ｾ雎｡縺ｮ蝗櫁ｻ｢謌仙・・亥盾辣ｧ・・/param>
    /// <param name="delta">莉翫ヵ繝ｬ繝ｼ繝縺ｮ蝗櫁ｻ｢驥・/param>
    void SnapRotation(float& compo, float delta);
    
    Transform* GetTarget() const;

public:
    TRS();

    /// <summary>TRS縺ｮ謫堺ｽ懊Δ繝ｼ繝峨ｒ險ｭ螳壹☆繧具ｼ・tate蛻玲嫌繧貞盾辣ｧ・・/summary>
    /// <param name="state">險ｭ螳壹☆繧鬼tate蛟､</param>
    void SetState(int state) { m_state = state; }

    void Draw() override;
    void Update() override;

    /// <summary>迴ｾ蝨ｨ縺ｮ繝｢繝ｼ繝峨↓蠢懊§縺溘ぐ繧ｺ繝｢縺ｨ繝ｬ繧､縺ｮ蠖薙◆繧雁愛螳壹ｒ陦後＞縲∝ｽ薙◆縺｣縺溯ｻｸ繧定ｿ斐☆</summary>
    /// <param name="ray">蛻､螳壹↓菴ｿ逕ｨ縺吶ｋ繝ｬ繧､</param>
    /// <returns>蠖薙◆縺｣縺溯ｻｸ・・/Y/Z・峨∝ｽ薙◆繧峨↑縺代ｌ縺ｰNone</returns>
    Axis RayHitTest(const Ray& ray) const;

    /// <summary>繝峨Λ繝・げ荳ｭ縺ｮ霆ｸ縺ｫ豐ｿ縺｣縺ｦ驕ｸ謚槭が繝悶ず繧ｧ繧ｯ繝医・Transform繧呈峩譁ｰ縺吶ｋ</summary>
    void SetTransform();

    /// <summary>繝槭え繧ｹ遘ｻ蜍暮㍼繧呈欠螳夊ｻｸ縺ｮ繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ謚募ｽｱ繝吶け繝医Ν縺ｫ蟆・ｽｱ縺励√Ρ繝ｼ繝ｫ繝臥ｩｺ髢薙・螟牙喧驥上ｒ霑斐☆</summary>
    /// <param name="axis">謫堺ｽ懆ｻｸ</param>
    /// <param name="objPos">蟇ｾ雎｡繧ｪ繝悶ず繧ｧ繧ｯ繝医・繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・/param>
    /// <returns>霆ｸ譁ｹ蜷代・螟牙喧驥・/returns>
    static float AddTransform(Axis axis, const VECTOR3& objPos);

    /// <summary>繝峨Λ繝・げ荳ｭ縺ｮ霆ｸ繧定ｨｭ螳壹☆繧九・one繧呈欠螳壹☆繧九→繝峨Λ繝・げ繧定ｧ｣髯､縺吶ｋ</summary>
    /// <param name="axis">險ｭ螳壹☆繧玖ｻｸ</param>
    void SetDraggingAxis(Axis axis) { dragging_axis_ = axis; }

    void SetOverrideTarget(Transform* t);
    
    enum State : uint8_t
    {
        kNone,
        kTranslation,
        kRotation,
        kScaling
    };
};

