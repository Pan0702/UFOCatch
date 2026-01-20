#include "ParticleSystem.h"
#include "../Core/Game/GameMain.h"

//------------------------------------------------------------------------
// コンストラクタ
//------------------------------------------------------------------------
CParticleEmitter::CParticleEmitter(CSpriteImage* tex)
{
    texture = nullptr;
    texture = tex;
    isActive = false;
    coneRadius = 4.0f;  // デフォルト値
    coneHeight = 8.0f;
}

//------------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------------
CParticleEmitter::~CParticleEmitter()
{
    particles.clear();
}

/////////////////////////
///テクスチャ設定
/////////////////////////
void CParticleEmitter::SetTexture(CSpriteImage* tex)
{
    texture = tex;
}

/////////////////////////
///パーティクル作成
/////////////////////////
void CParticleEmitter::Emit(const VECTOR3& pos, int count)
{
    for(int i = 0; i < count; i++)
    {
        Particle p;
        
        // ランダムな高さ位置（0.0~1.0）
        float heightRatio = (rand() % 100) / 100.0f;
        
        // その高さでの半径（下に行くほど広がる）
        float currentRadius = coneRadius * heightRatio;
        
        // ランダムな角度
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        
        // ランダムな距離
        float distance = (rand() % 100) / 100.0f * currentRadius;
        
        // 位置を計算
        p.position.x = pos.x + cos(angle) * distance;
        p.position.y = pos.y - heightRatio * coneHeight;
        p.position.z = pos.z + sin(angle) * distance;
        
        // 速度：基本は上向きだが、少しランダムに揺らぐ
        float velocityY = 2.0f + (rand() % 100) / 100.0f;  // 2.0~3.0
        float velocityX = ((rand() % 100) - 50) / 100.0f;  // -0.5~0.5
        float velocityZ = ((rand() % 100) - 50) / 100.0f;  // -0.5~0.5
        
        p.velocity = VECTOR3(velocityX, velocityY, velocityZ);
        
        // サイズもランダムに
        p.size = 0.2f + (rand() % 100) / 200.0f;  // 0.2~0.7
        
        p.life = 1.0f + (rand() % 100) / 100.0f;  // 1.0~2.0秒
        p.color = VECTOR4(1, 1, 1, 1);
        
        particles.push_back(p);
    }
}

void CParticleEmitter::SetActive(bool active)
{
    isActive = active;
}

void CParticleEmitter::SetConeShape(float radius, float height)
{
    coneRadius = radius;
    coneHeight = height;
}

/////////////////////////
///更新
/////////////////////////
void CParticleEmitter::Update(float deltaTime,const VECTOR3& emitPos)
{
    // アクティブな時だけ生成
    if(isActive)
    {
        Emit(emitPos, 5);  // 毎フレーム5個
    }
    
    // パーティクルの更新は常に行う（消えるまで）
    for(auto it = particles.begin(); it != particles.end(); )
    {
        it->position += it->velocity * deltaTime;
        it->life -= deltaTime;
        
        if(it->life <= 0)
        {
            it = particles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
/////////////////////////
// 描画//
/////////////////////////
void CParticleEmitter::Render()
{
    if(!texture) return;
    
    CSprite sprite;
    sprite.SetImage(texture);
    sprite.m_nBlend = 2;  // 加算合成
    
    for(auto& p : particles)
    {
        sprite.SetSrc3D(p.size, p.size, 0, 0, 
                       texture->m_dwImageWidth, 
                       texture->m_dwImageHeight);
        sprite.Draw3D(p.position);
    }
}