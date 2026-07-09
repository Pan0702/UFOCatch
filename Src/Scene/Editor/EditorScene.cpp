#include "EditorScene.h"
#include "../../MapEditor/BackGraund.h"
#include "../../MapEditor/Buttom.h"
#include "../../MapEditor/Controller.h"
#include "../../MapEditor/EditorUI.h"
#include "../../MapEditor/TRSObject/TRS.h"
#include "../../MapEditor/StageData.h"
#include "../../Stage/CubeBox.h"
#include "../../Stage/SkyBox.h"

CEditorScene::CEditorScene()
{
    StageData* stage_data = Instantiate<StageData>();
    stage_data->Import("data/Stage/easy.json");
    Instantiate<Button>();
    Instantiate<TRS>();
    Instantiate<Controller>();
    Instantiate<EditorUI>(); // Button/TRS/Controller の後に生成する
    Instantiate<CSkyBox>("data/Ground/SkyBox.mesh");
}

CEditorScene::~CEditorScene()
{
}
