#include "CubeBox.h"

////////////////////
// メッシュファイルのパスを指定してインスタンスを生成する
// @param meshPath メッシュファイルのパス //
////////////////////
CCubeBox::CCubeBox(const char* meshPath)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load(meshPath);
}

CCubeBox::~CCubeBox()
{
}

void CCubeBox::Draw()
{
    Object3D::Draw();
}
