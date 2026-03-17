#include "EditorScene2.h"

#include "BackGraund.h"
#include "Buttom.h"
#include "Controller.h"
#include "RandomPlacer.h"
#include "TRSObject/TRS.h"
#include "StageData.h"

EditorScene2::EditorScene2()
{
    new Camera();
    new StageData();
    new Button();
    new TRS();
    new Controller();
    new BackGraund();
}
