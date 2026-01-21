#include "ParticleSystem.h"
#include "../Core/Game/GameMain.h"

////////////////////
// コンストラクタ
// @param tex パーティクルに使用するテクスチャ //
////////////////////
CParticleEmitter::CParticleEmitter(CSpriteImage* tex)
{
    m_pTexture = nullptr;
    m_pTexture = tex;
    m_isActive = false;
    m_coneRadius = 4.0f;  // 円錐の底面半径のデフォルト値 //
    m_coneHeight = 8.0f;  // 円錐の高さのデフォルト値 //
}

////////////////////
// デストラクタ //
////////////////////
CParticleEmitter::~CParticleEmitter()
{
    m_pParticles.clear();
}

////////////////////
// パーティクルに使用するテクスチャを設定する
// @param tex 設定するテクスチャ 
////////////////////
void CParticleEmitter::SetTexture(CSpriteImage* tex)
{
    m_pTexture = tex;
}

////////////////////
// 指定位置から円錐状にパーティクルを生成する
// @param pos 放出位置
// @param count 生成する個数 
////////////////////
void CParticleEmitter::Emit(const VECTOR3& pos, int count)
{
    for(int i = 0; i < count; i++)
    {
        Particle p;

        // ランダムな高さ位置（0.0~1.0）
        float heightRatio = Randomf(0.0f, 1.0f);

        // その高さでの半径（下に行くほど広がる）
        float currentRadius = m_coneRadius * heightRatio;

        // ランダムな角度
        float angle = Randomf(0.0f, 360.0f) * 3.14159f / 180.0f;

        // ランダムな距離
        float distance = Randomf(0.0f, 1.0f) * currentRadius;

        // 位置を計算
        p.position.x = pos.x + std::cos(angle) * distance;
        p.position.y = pos.y - heightRatio * m_coneHeight;
        p.position.z = pos.z + std::sin(angle) * distance;

        // 速度：真上に向かって一直線
        p.velocity = VECTOR3(0.0f, 3.0f, 0.0f);

        // サイズは少しランダム
        p.size = 0.2f + Randomf(0.0f, 0.3f);  // 0.3~0.8

        // 寿命
        p.life = 2.0f;
        // 色：少し黄色っぽくして、アルファ値を下げる
        p.color = VECTOR4(1.0f, 1.0f, 0.8f, 0.4f);  // 半透明に
        m_pParticles.push_back(p);
    }
}
////////////////////
// パーティクルの生成を有効/無効にする
// @param active true: 生成有効, false: 生成無効 
////////////////////
void CParticleEmitter::SetActive(bool active)
{
    m_isActive = active;
    if (not active)
    {
        m_pParticles.clear();  // 非アクティブ時は全パーティクルを削除 //
    }
}

////////////////////
// パーティクル放出時の円錐形状を設定する
// @param radius 円錐の底面半径
// @param height 円錐の高さ 
////////////////////
void CParticleEmitter::SetConeShape(float radius, float height)
{
    m_coneRadius = radius;
    m_coneHeight = height;
}

////////////////////
// パーティクルの生成と更新を行う
// @param deltaTime 前フレームからの経過時間
// @param emitPos パーティクル放出位置 
////////////////////
void CParticleEmitter::Update(float deltaTime,const VECTOR3& emitPos)
{
    // アクティブな時だけ生成 //
    if(m_isActive)
    {
        Emit(emitPos, 2);  // 毎フレーム5個生成 //
    }

    // パーティクルの更新は常に行う（消えるまで） //
    for(auto it = m_pParticles.begin(); it != m_pParticles.end(); )
    {
        it->position += it->velocity * deltaTime;  // 位置を速度分移動 //
        it->life -= deltaTime;  // 寿命を減らす //

        // 寿命が尽きたか、一定の高さより上に到達したら削除 //
        if(it->life <= 0 || it->position.y > emitPos.y)
        {
            it = m_pParticles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
////////////////////
// 全てのパーティクルを3D空間に描画する 
////////////////////
void CParticleEmitter::Render()
{
    if(!m_pTexture) return;  // テクスチャが未設定なら描画しない //

    CSprite sprite;
    sprite.SetImage(m_pTexture);
    sprite.m_nBlend = 2;  // 加算合成（光らせる効果） //

    for(auto& p : m_pParticles)
    {
        // パーティクルのサイズとテクスチャ座標を設定 //
        sprite.SetSrc3D(p.size, p.size, 0, 0,
                       m_pTexture->m_dwImageWidth,
                       m_pTexture->m_dwImageHeight);
        sprite.Draw3D(p.position);  // 3D空間に描画 //
    }
}