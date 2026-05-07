#include "CubeBox.h"

////////////////////
// メッシュファイルのパスを指定してインスタンスを生成する
// @param meshPath メッシュファイルのパス //
////////////////////
CCubeBox::CCubeBox(const char* meshPath)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load(meshPath);
    transform.scale = VECTOR3(5, 5, 5);
}

CCubeBox::~CCubeBox()
{
}

void CCubeBox::Draw()
{
    if (m_pMesh != nullptr)
    {
        const VECTOR3 flatLightDir = VECTOR3(0, 0, 0);

        m_pMesh->Render(
            transform.matrix(),
            GameDevice()->m_mView,
            GameDevice()->m_mProj,
            flatLightDir,
            GameDevice()->m_vEyePt
        );
    }
}
