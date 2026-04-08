#include "PlayUI.h"
#include "../../Common/Constants.h"
#include "../../UI/ImageRegistry.h"
using namespace Constants;
PlayUI::PlayUI()
{
     m_pImage = ImageRegistry::LoadTexture(SceneName::PLAY, "data/PlayUIParts.png");
}

void PlayUI::InitButton()
{
}

void PlayUI::InitImage()
{
}

void PlayUI::InitBar()
{
}
