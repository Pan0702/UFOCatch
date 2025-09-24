#include "MainControl.h"
#include <assert.h>
#include <string>
#include "GameMain.h"

namespace {
    float refreshTimer = 0.0f;
};

void MainControl::UseRefreshMessage()
{
    refreshTimer = 0.0f;
}

void MainControl::UseFrameTimer(float time)
{
    assert(time > 0.0f);
    refreshTimer = time;
}

void MainControl::SetWindowName(const char* name)
{
    SetWindowText(GameDevice()->m_pMain->m_hWnd, name);
}

float MainControl::RefreshTimer()
{
    return refreshTimer;
}
