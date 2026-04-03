#include "EditorScene.h"
#include "../../MapEditor/BackGraund.h"
#include "../../MapEditor/Buttom.h"
#include "../../MapEditor/Controller.h"
#include "../../MapEditor/EditorUI.h"
#include "../../MapEditor/TRSObject/TRS.h"
#include "../../MapEditor/StageData.h"
CEditorScene::CEditorScene()
{
    Instantiate<StageData>();
    Instantiate<Button>();
    Instantiate<TRS>();
    Instantiate<Controller>();
    Instantiate<EditorUI>(); // Button/TRS/Controller の後に生成する
    Instantiate<BackGraund>();
}

CEditorScene::~CEditorScene()
{
}
