#include "ShadowObject.h"
#include "Object3D.h"
#include "../Framework/ObjectManager.h"

CShadowObject::CShadowObject(Object3D* pOwner, const TCHAR* imagePath, int drawOrder)
    : m_pOwner(pOwner)
    , m_pSpriteImage(new CSpriteImage(imagePath))
{
    SetDrawOrder(1);
}

CShadowObject::~CShadowObject()
{
    delete m_pSpriteImage;
    m_pSpriteImage = nullptr;
}

void CShadowObject::DestroyMe()
{
    ObjectManager::Destroy(this);
}

void CShadowObject::Draw()
{
    if (!ObjectManager::IsExist(m_pOwner))
    {
        ObjectManager::Destroy(this);
        return;
    }

    CFbxMesh* pMesh = m_pOwner->GetMesh();
    if (pMesh == nullptr) return;

    static constexpr float  GROUND_OFFSET   = 0.1f;
    static const    VECTOR2 IMAGE_SCALE      = VECTOR2(2.0f, 1.65f);
    static constexpr float  FORWARD_STRETCH  = 1.5f;
    static constexpr float  CIRCLE_DEPTH     = 1.0f;
    static constexpr float  GROUND_ROTATION  = -XM_PI / 2.0f;
    static constexpr float  CIRCLE_ALPHA     = 0.5f;
    static constexpr float  SPRITE_SIZE      = 1.0f;

    CSprite spr;

    const Transform ownerTransform = m_pOwner->GetTransform();
    const VECTOR2   objectSize     = ToVec2XZ(pMesh->m_vMax);
    const VECTOR3   groundPos      = VECTOR3(ownerTransform.position.x, GROUND_OFFSET, ownerTransform.position.z);

    const MATRIX4X4 mScale        = XMMatrixScaling(objectSize.x * IMAGE_SCALE.x,
                                                     objectSize.y * IMAGE_SCALE.y,
                                                     CIRCLE_DEPTH);
    const float     forwardOffset  = -(objectSize.y * IMAGE_SCALE.y * (FORWARD_STRETCH - 0.75f) / 6.0f);
    const MATRIX4X4 mForwardShift  = XMMatrixTranslation(0, forwardOffset, 0);
    const MATRIX4X4 mRotX          = XMMatrixRotationX(GROUND_ROTATION);
    const MATRIX4X4 mRotY          = XMMatrixRotationY(ownerTransform.rotation.y);
    const MATRIX4X4 mTranslation   = XMMatrixTranslation(groundPos.x, groundPos.y, groundPos.z);
    const MATRIX4X4 mWorld         = mScale * mForwardShift * mRotX * mRotY * mTranslation;

    const MATRIX4X4 mView = GameDevice()->m_mView;
    const MATRIX4X4 mProj = GameDevice()->m_mProj;

    const DWORD texWidth  = m_pSpriteImage->m_dwImageWidth;
    const DWORD texHeight = m_pSpriteImage->m_dwImageHeight;

    spr.Draw3DWithWorldMatrix(
        m_pSpriteImage,
        mWorld, mView, mProj,
        VECTOR2(SPRITE_SIZE, SPRITE_SIZE),
        VECTOR2(0, 0),
        VECTOR2(static_cast<float>(texWidth), static_cast<float>(texHeight)),
        CIRCLE_ALPHA
    );
}
