#pragma once
#include "../Common/Object3D.h"

// 蝨ｰ髱｢縺ｮ繝｡繝・す繝･縺ｨ繧ｳ繝ｪ繧ｸ繝ｧ繝ｳ繧堤ｮ｡逅・☆繧九け繝ｩ繧ｹ //
class CGround : public Object3D
{
public:
    //.mesh縺ｮPath縺ｨmesh縺ｮ螟ｧ縺阪＆繧呈ｸ｡縺・/
    //縲蛻晄悄迥ｶ諷九・・托ｼ静暦ｼ托ｼ・荳ｭ蠢・せ縺ｯ5,0,5//
    CGround(const char* mesh, const VECTOR3& scale = VECTOR3(1.0f, 1.0f, 1.0f));
    ~CGround();
private:

   // bool Init(const char* filePath);
};

