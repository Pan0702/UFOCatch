#pragma once
#include "MyMath.h"
#include "Sprite3D.h"

////////////////////
// パーティクル構造体 
////////////////////
struct Particle {
    VECTOR3 position;  // パーティクルの位置 //
    VECTOR3 velocity;  // パーティクルの速度 //
    float life;        // 寿命（秒） //
    float size;        // サイズ //
    VECTOR4 color;     // 色 //

};

////////////////////
// パーティクルエミッタークラス
// 円錐状にパーティクルを放出し、管理する
////////////////////
class CParticleEmitter {
public:
    ////////////////////
    // コンストラクタ
    // @param tex パーティクルに使用するテクスチャ 
    ////////////////////
    CParticleEmitter(CSpriteImage* tex);
    
    ~CParticleEmitter();

    ////////////////////
    // パーティクルに使用するテクスチャを設定する
    // @param tex 設定するテクスチャ 
    ////////////////////
    void SetTexture(CSpriteImage* tex);

    ////////////////////
    // パーティクルの生成と更新を行う
    // @param deltaTime 前フレームからの経過時間
    // @param emitPos パーティクル放出位置 
    ////////////////////
    void Update(float deltaTime, const VECTOR3& emitPos);

    ////////////////////
    // 全てのパーティクルを3D空間に描画する 
    ////////////////////
    void Render();

    ////////////////////
    // 指定位置から円錐状にパーティクルを生成する
    // @param pos 放出位置
    // @param count 生成する個数 
    ////////////////////
    void Emit(const VECTOR3& pos, int count);

    ////////////////////
    // パーティクルの生成を有効/無効にする
    // @param active true: 生成有効, false: 生成無効 
    ////////////////////
    void SetActive(bool active);

    ////////////////////
    // パーティクル放出時の円錐形状を設定する
    // @param radius 円錐の底面半径
    // @param height 円錐の高さ 
    ////////////////////
    void SetConeShape(float radius, float height);

private:
    std::vector<Particle> m_pParticles;  // パーティクルリスト //
    CSpriteImage* m_pTexture;  // パーティクルのテクスチャ //

    bool m_isActive;  // パーティクル生成が有効かどうか //
    float m_coneRadius;  // 円錐の底面半径 //
    float m_coneHeight;  // 円錐の高さ //

};
