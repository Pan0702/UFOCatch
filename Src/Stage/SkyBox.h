#pragma once
#include "../Common/Object3D.h"

class CSkyBox : public Object3D
{
public:
    CSkyBox(const char* meshPath);
    ~CSkyBox();

private:
    void Update() override;
    void Draw() override;

    ID3D11RasterizerState* m_pRStateCullNone;
};
