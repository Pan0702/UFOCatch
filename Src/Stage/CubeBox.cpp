#include "CubeBox.h"

////////////////////
// 繝｡繝・す繝･繝輔ぃ繧､繝ｫ縺ｮ繝代せ繧呈欠螳壹＠縺ｦ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌舌☆繧・
// @param meshPath 繝｡繝・す繝･繝輔ぃ繧､繝ｫ縺ｮ繝代せ //
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
