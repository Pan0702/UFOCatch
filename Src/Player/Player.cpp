#define NOMINMAX
#include "Player.h"
#include "PCamera.h"
#include <iostream>
#include <algorithm>
#include "PlayerHP.h"
#include "../Utils/Lerp.h"
#include "../System/VisionSystem.h"
#include "../Utils/Sprite3D.h"

////////////////////
// 原点から移動できる距離
// 例えば-20~20なら20と入力
// @param moveRange 移動可能範囲 //
////////////////////
CPlayer::CPlayer(float moveRange)
    : m_moveRange(moveRange)
{
    transform.position = VECTOR3(0, 5, 0);
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Player/UFO.mesh");
    new CPlayerHP(3);
    m_coneDegree = 7;
    //半径の計算//
    m_coneRadius = transform.position.y * tan(DegToRad * m_coneDegree);
    m_allExp = 1;
    m_exp = 0.001;
    m_lv = 1;
    m_zoomUp = false;
    m_coneTopPos = transform.position.y;
    transform.scale = VECTOR3(0.5f, 0.5f, 0.5f);
    m_SuctionActive = false;
    m_draw = true;

    // 吸い込み円用のテクスチャを読み込み
    m_pCircleImage = new CSpriteImage(TEXT("data/CircleSuction.png"));
    
    m_pParticleImage = new CSpriteImage(_T("data/particle.png"));
    m_pEmitter = new CParticleEmitter(m_pParticleImage);
    m_pEmitter->SetConeShape(m_coneRadius,m_coneTopPos);
}

CPlayer::~CPlayer()
{
    SAFE_DELETE(m_pMesh);
    SAFE_DELETE(m_pCircleImage);
    SAFE_DELETE(m_pParticleImage);
    SAFE_DELETE(m_pEmitter);
}

void CPlayer::Update()
{
    if (!m_SuctionActive)
    {
        HandleMovementInput();
    }
    
    //吸い込みキーの状態を取得
    if (!ObjectManager::FindGameObject<CPlayerHP>()->GetFoundFlag())
    {
        m_SuctionActive = GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_J);
    }
    else
    {
        m_SuctionActive = false;
    }
    
    CheckLevel();

    // Lerp処理//
    UpdateHeightAndRadiusLerp();

    // カメラ位置を更新//
    UpdateCameraPos();

    m_pEmitter->SetConeShape(m_coneRadius,transform.position.y);
    CVisionSystem* pVision = ObjectManager::FindGameObject<CVisionSystem>();
    pVision->SetCircleCenter(transform.position);
    pVision->SetCircleRadius(m_coneRadius);

    m_pEmitter->SetActive(m_SuctionActive);  // 吸い込み中だけ生成
    m_pEmitter->Update(SceneManager::DeltaTime(), this->transform.position);  // UFOの位置で生成
}

////////////////////
// プレイヤーの移動入力を処理する //
////////////////////
void CPlayer::HandleMovementInput()
{
    //移動制限//
    transform.position.x = std::max(-m_moveRange, std::min(m_moveRange, transform.position.x));
    transform.position.z = std::max(-m_moveRange, std::min(m_moveRange, transform.position.z));

    constexpr float moveSpeed = 5.0f; // 秒間5ユニットの移動速度//
    const float moveAmount = moveSpeed * SceneManager::DeltaTime();
    auto* input = GameDevice()->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) transform.position.z += moveAmount;
    if (input->CheckKey(KD_DAT, DIK_S)) transform.position.z -= moveAmount;
    if (input->CheckKey(KD_DAT, DIK_A)) transform.position.x -= moveAmount;
    if (input->CheckKey(KD_DAT, DIK_D)) transform.position.x += moveAmount;
}

void CPlayer::CheckLevel()
{
    //レベルアップ処理
    if (m_exp >= m_allExp)
    {
        float tmp = m_exp - m_allExp;
        m_allExp *= 1.3f;
        m_exp = tmp;
        m_lv++;
        IncreaseSuctionConeHeight();
    }
}

////////////////////
// 吸い込みコーンの高さを増加させる //
////////////////////
void CPlayer::IncreaseSuctionConeHeight()
{
    //吸い込むコーンの大きさを変更
    const float targetHeight = m_coneTopPos + 1.0f;
    const float targetRadius = targetHeight * tan(DegToRad * m_coneDegree);

    //大きさ変更したときのLerp
    m_heightLerp.Start(m_coneTopPos, targetHeight, 0.05f);
    m_radiusLerp.Start(m_coneRadius, targetRadius, 0.05f);
}

void CPlayer::UpdateHeightAndRadiusLerp()
{
    //Lerp中かどうか
    if (m_heightLerp.IsAnimating())
    {
        m_coneTopPos = m_heightLerp.Update(SceneManager::DeltaTime());
    }
    if (m_radiusLerp.IsAnimating())
    {
        m_coneRadius = m_radiusLerp.Update(SceneManager::DeltaTime());
    }
}

void CPlayer::UpdateCameraPos()
{
    //吸い込み中じゃなかったら
    if (not m_SuctionActive)
    {
        if (m_zoomUp)
        {
            //始点切り替え
            ObjectManager::FindGameObject<CPlayerCamera>()->ZoomOut(transform.position);
            m_zoomUp = false;
        }
        else
        {
            // カメラ位置をコーンの高さに基づいて設定//
            ObjectManager::FindGameObject<CPlayerCamera>()->PosSet(
                transform.position, m_coneTopPos);
        }
    }
    else
    {
        //吸い込み中だったら
        m_zoomUp = true;
        //始点切り替え
        ObjectManager::FindGameObject<CPlayerCamera>()->ZoomIn(transform.position);
    }
}


////////////////////
// 引き寄せるための移動量を計算する
// 高さの差が大きいほど遅く、近いほど速く吸い込む
// @param moveTimeSecond 移動にかける時間
// @param animalPos 動物の位置
// @return 1回当たりの移動量 //
////////////////////
VECTOR3 CPlayer::CalcSuctionDisplacement(const float& moveTimeSecond, const VECTOR3& animalPos) const
{
    float heightDiff = m_coneTopPos - animalPos.y;
    float progress = 1.0f - (heightDiff / m_coneTopPos);
    progress = std::max(0.0f, std::min(1.0f, progress));
    float eased = progress * progress * progress;

    // 速度係数を計算//
    constexpr float minSpeed = 0.4f;
    constexpr float maxSpeed = 1.8f;
    float heightSpeedMultiplier = minSpeed + (maxSpeed - minSpeed) * eased;

    //Y座標が0の点（求めるための内分比の係数//
    float projectionFactorY0 = avoidZero((0 - animalPos.y) / (animalPos.y - m_coneTopPos));
    //0地点での動物のポジション//
    VECTOR3 targetPointOnPlane =
        VECTOR3(animalPos.x + projectionFactorY0 * (animalPos.x - transform.position.x), 0,
                animalPos.z + projectionFactorY0 * (animalPos.z - transform.position.z));
    VECTOR3 pullVectorToTarget = transform.position - targetPointOnPlane;

    VECTOR3 suctionDisplacementPerFrame = pullVectorToTarget / moveTimeSecond * heightSpeedMultiplier;
    return suctionDisplacementPerFrame * SceneManager::DeltaTime();
}

////////////////////
// オブジェクトがコーンの範囲内にいるかチェックする
// @param targetPos 対象の位置
// @return 範囲内ならtrue //
////////////////////
bool CPlayer::IsWithSuctionCone(const VECTOR3& targetPos) const
{
    const float distanceAnimalFromPlayer = m_coneTopPos - targetPos.y;
    const float coneRadiusAtTargetHeight = distanceAnimalFromPlayer * std::tan(DegToRad * m_coneDegree);
    if (Pow2(targetPos.x - transform.position.x) + Pow2(targetPos.z - transform.position.z)
        <= Pow2(coneRadiusAtTargetHeight))
    {
        return true;
    }
    return false;
}

void CPlayer::Draw()
{
    CPlayerHP* hp = ObjectManager::FindGameObject<CPlayerHP>();
    if (hp->GetFoundFlag())
    {
        m_draw = !m_draw;
    }
    else
    {
        m_draw = true;
    }
    if (m_draw)
    {
        Object3D::Draw();
    }

    // 吸い込み円を描画
    DrawSuctionCircle();

    m_pEmitter->Render();
    //DrawCircle(VECTOR3(transform.position.x, 0, transform.position.z), m_coneRadius, RGB(0, 255, 0));
}

////////////////////
// 地面に吸い込み範囲の円を描画する //
////////////////////
void CPlayer::DrawSuctionCircle()
{
    // 吸い込み円描画の定数 //
    static constexpr float GROUND_OFFSET = 0.01f;        // 地面から少し浮かせる高さ（Zファイティング回避用） //
    static constexpr float CIRCLE_DIAMETER_SCALE = 2.0f; // 半径から直径への変換倍率 //
    static constexpr float CIRCLE_DEPTH = 1.0f;          // 円の奥行き（Z軸方向のスケール） //
    static constexpr float GROUND_ROTATION = -XM_PI / 2.0f; // 地面に平行にするための回転角度（-90度） //
    static constexpr float CIRCLE_ALPHA = 0.5f;          // 円の透明度（半透明） //
    static constexpr float SPRITE_SIZE = 1.0f;           // スプライトのベースサイズ（ワールド行列でスケールするため） //

    CSprite spr;

    // 地面（Y=0）のUFO真下に配置する位置 //
    VECTOR3 groundPos = VECTOR3(transform.position.x, GROUND_OFFSET, transform.position.z);

    // ワールドマトリックスを作成 //
    // スケール：吸い込み半径に合わせる //
    MATRIX4X4 mScale = XMMatrixScaling(
        m_coneRadius * CIRCLE_DIAMETER_SCALE,
        m_coneRadius * CIRCLE_DIAMETER_SCALE,
        CIRCLE_DEPTH
    );
    // 回転：X軸周りに-90度回転して地面に平行にする //
    MATRIX4X4 mRotation = XMMatrixRotationX(GROUND_ROTATION);
    // 平行移動：地面のUFO真下へ //
    MATRIX4X4 mTranslation = XMMatrixTranslation(groundPos.x, groundPos.y, groundPos.z);
    // ワールドマトリックスを合成（スケール → 回転 → 平行移動） //
    MATRIX4X4 mWorld = mScale * mRotation * mTranslation;
    // カメラのビュー・プロジェクションマトリックスを取得 //
    MATRIX4X4 mView = GameDevice()->m_mView;
    MATRIX4X4 mProj = GameDevice()->m_mProj;

    // テクスチャのサイズを取得 //
    DWORD texWidth = m_pCircleImage->m_dwImageWidth;
    DWORD texHeight = m_pCircleImage->m_dwImageHeight;

    // 円形スプライトを描画 //
    spr.Draw3DWithWorldMatrix(
        m_pCircleImage,
        mWorld,
        mView,
        mProj,
        VECTOR2(SPRITE_SIZE, SPRITE_SIZE),            // サイズ（ワールドマトリックスでスケールするので1.0） //
        VECTOR2(0, 0),                                // テクスチャ座標（左上） //
        VECTOR2((float)texWidth, (float)texHeight),   // テクスチャサイズ（全体を使用） //
        CIRCLE_ALPHA                                  // アルファ（半透明） //
    );
}

#if 0
///Debug///
void CPlayer::DrawCircle(const VECTOR3& center, float radius, DWORD color)
{
    CSprite spr;
    constexpr int segments = 32; // 円を構成する線分の数
    const float angleStep = 2.0f * 3.14159f / segments; // 各線分の角度

    for (int i = 0; i < segments; ++i)
    {
        // 現在の点
        float angle1 = static_cast<float>(i) * angleStep;
        VECTOR3 point1 = center;
        point1.x += radius * cos(angle1);
        point1.z += radius * sin(angle1);

        // 次の点
        float angle2 = (i + 1) % segments * angleStep;
        VECTOR3 point2 = center;
        point2.x += radius * cos(angle2);
        point2.z += radius * sin(angle2);

        // 線を描画
        spr.DrawLine3D(point1, point2, color);
    }
    spr.DrawLine3D(transform.position, VECTOR3(center.x + m_coneRadius, center.y, center.z), color);
    spr.DrawLine3D(transform.position, VECTOR3(center.x - m_coneRadius, center.y, center.z), color);
    spr.DrawLine3D(transform.position, VECTOR3(center.x, center.y, center.z + m_coneRadius), color);
    spr.DrawLine3D(transform.position, VECTOR3(center.x, center.y, center.z - m_coneRadius), color);
}
#endif

