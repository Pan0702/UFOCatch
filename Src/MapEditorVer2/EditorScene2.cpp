#include "EditorScene2.h"

#include "BackGraund.h"
#include "Buttom.h"
#include "Controller.h"
#include "RandomPlacer.h"
#include "TRSObject/TRS.h"
#include "StageData.h"

EditorScene2::EditorScene2()
{
    Instantiate<Camera>();
    Instantiate<StageData>();
    Instantiate<Button>();
    Instantiate<TRS>();
    Instantiate<Controller>();
    Instantiate<BackGraund>();
}

