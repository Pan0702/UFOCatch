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
    Instantiate<StageData>();
    Instantiate<Button>();
    Instantiate<TRS>();
    Instantiate<Controller>();
    Instantiate<EditorUI>(); // Button/TRS/Controller の後に生成する
    Instantiate<CCubeBox>("data/Ground/SkyBox.mesh");
}

CEditorScene::~CEditorScene()
{
}
