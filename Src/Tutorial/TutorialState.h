#pragma once

class CTutorial;

// 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｮ繧ｹ繝・・繝亥渕蠎輔け繝ｩ繧ｹ //
class CTutorialState
{
public:
    // 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
    // @param pT 繝√Η繝ｼ繝医Μ繧｢繝ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ //
    CTutorialState(CTutorial* pT);

    virtual void Enter(){}
    virtual void Update(){}
    virtual void Exit(){}

    // 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｮ繧ｹ繝・・繝育ｨｮ蛻･ //
    enum class State
    {
        None,       // 譛ｪ險ｭ螳・//
        Move,       // 遘ｻ蜍墓桃菴・//
        Suction,    // 蜷ｸ縺・ｾｼ縺ｿ謫堺ｽ・//
        Expands,    // 諡｡螟ｧ謫堺ｽ・//
        Discovery,  // 逋ｺ隕九＆繧後ｋ菴馴ｨ・//
        Play,       // 螳溯ｷｵ繝励Ξ繧､ //
    };

protected:
    CTutorial* m_pTutorial;  // 繝√Η繝ｼ繝医Μ繧｢繝ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ //

    // 繝励Ξ繧､繝､繝ｼ縺ｫ謐慕佐縺輔ｌ縺溷虚迚ｩ繧貞炎髯､縺吶ｋ //
    void RemoveCaughtAnimals();
};

// 遘ｻ蜍墓桃菴懊ｒ謨吶∴繧九せ繝・・繝・//
class CMoveState : public CTutorialState
{
public:
    // @param pT 繝√Η繝ｼ繝医Μ繧｢繝ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ //
    CMoveState(CTutorial* pT);

    void Enter() override{}
    void Update() override;
    void Exit() override;
};

// 蜷ｸ縺・ｾｼ縺ｿ謫堺ｽ懊ｒ謨吶∴繧九せ繝・・繝・//
class CSuctionState : public CTutorialState
{
public:
    // @param pT 繝√Η繝ｼ繝医Μ繧｢繝ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ //
    CSuctionState(CTutorial* pT);

    void Enter() override{}
    void Update() override;
    void Exit() override;
};

// 諡｡螟ｧ謫堺ｽ懊ｒ謨吶∴繧九せ繝・・繝・//
class CExpands : public CTutorialState
{
public:
    // @param pT 繝√Η繝ｼ繝医Μ繧｢繝ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ //
    CExpands(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// 逋ｺ隕九＆繧後ｋ縺薙→繧呈蕗縺医ｋ繧ｹ繝・・繝・//
class CDiscoveryState : public CTutorialState
{
public:
    // @param pT 繝√Η繝ｼ繝医Μ繧｢繝ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ //
    CDiscoveryState(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// 螳溯ｷｵ繝励Ξ繧､繧ｹ繝・・繝・//
class CPlayState : public CTutorialState
{
public:
    // @param pT 繝√Η繝ｼ繝医Μ繧｢繝ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝昴う繝ｳ繧ｿ //
    CPlayState(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override{}
};
