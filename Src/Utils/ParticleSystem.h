#pragma once
#include "MyMath.h"
#include "Sprite3D.h"

struct Particle {
    VECTOR3 position;
    VECTOR3 velocity;
    float life;        // 0.0~1.0
    float size;
    VECTOR4 color;

};

class CParticleEmitter {
public:
    CParticleEmitter(CSpriteImage* tex);
    ~CParticleEmitter();
    void SetTexture(CSpriteImage* tex);
    void Update(float deltaTime, const VECTOR3& emitPos);
    void Render();
    void Emit(const VECTOR3& pos, int count);
    void SetActive(bool active);
    void SetConeShape(float radius, float height);  

private:
    std::vector<Particle> particles;
    CSpriteImage* texture;  // キラキラテクスチャ
    
    bool isActive;
    float coneRadius;   // 円錐の底面半径
    float coneHeight;   // ：円錐の高さ

};