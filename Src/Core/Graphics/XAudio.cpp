// =========================================================================================
//
//  豕｢蠖｢繧ｪ繝ｼ繝・ぅ繧ｪ邂｡逅・
//                                                  ver 3.3        2024.3.23
// =========================================================================================

// 繝倥ャ繝繝ｼ繝輔ぃ繧､繝ｫ縺ｮ繧､繝ｳ繧ｯ繝ｫ繝ｼ繝・
#include "XAudio.h"
#include "../Game/GameMain.h"


//------------------------------------------------------------------------
//
//  XAudio2 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ 
//
//------------------------------------------------------------------------
CXAudio::CXAudio()
{
    ZeroMemory(this, sizeof(CXAudio));
}
//------------------------------------------------------------------------
//
//  XAudio2 繝・せ繝医Λ繧ｯ繧ｿ   
//
//------------------------------------------------------------------------
CXAudio::~CXAudio()
{
    SAFE_RELEASE(m_pXAudio2);
}

//------------------------------------------------------------------------
//
//   XAudio2 縺ｮ蛻晄悄蛹・   
//
//  HWND hWnd         繧ｦ繧｣繝ｳ繝峨え繝上Φ繝峨Ν
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CXAudio::InitAudio(HWND hWnd)
{
    UINT32 flags = 0;

    m_hWnd = hWnd;

    // XAudio2 繧ｨ繝ｳ繧ｸ繝ｳ縺ｮ菴懈・
    if (FAILED(XAudio2Create( &m_pXAudio2, flags)))
    {
       // -- 2018.7.8 XAudio2縺ｮDLL縺後↑縺・ｭ峨・逅・罰縺ｧ蛻晄悄蛹悶〒縺阪↑縺・ｴ蜷医〒繧ゅ・
       // MCI・・ciSendString・峨〒蜀咲函繧堤ｶ咏ｶ壹〒縺阪ｋ繧医≧縺ｫ縲√お繝ｩ繝ｼ縺ｫ縺ｯ縺帙★邯夊｡後＆縺帙ｋ縲・
       OutputDebugString(_T("隴ｦ蜻奇ｼ唸Audio2 縺ｮ蛻晄悄蛹悶↓螟ｱ謨励＠縺ｾ縺励◆縲・CI・・ciSendString・峨〒蜀咲函繧定ｩｦ縺ｿ縺ｾ縺吶・n"));
       SAFE_RELEASE(m_pXAudio2);
       return S_OK;
    }

    // 繝槭せ繧ｿ繝ｼ繝懊う繧ｹ・域怙邨ょ・蜉帛・・峨・菴懈・
    if (FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
    {
       MessageBox(0, _T("XAudio2 繝槭せ繧ｿ繝ｼ繝懊う繧ｹ縺ｮ菴懈・縺ｫ螟ｱ謨励＠縺ｾ縺励◆"), 0, MB_OK);
       return E_FAIL;
    }
    return S_OK;
}

//------------------------------------------------------------------------
//
//  XAudioSource 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ    
//
//------------------------------------------------------------------------
CXAudioSource::CXAudioSource() : CXAudioSource(GameDevice()->m_pXAudio) {}
CXAudioSource::CXAudioSource(const TCHAR* szFileName, DWORD dwNum) : CXAudioSource(GameDevice()->m_pXAudio, szFileName, dwNum) {}

CXAudioSource::CXAudioSource(CXAudio* pXAudio)
{
    ZeroMemory(this, sizeof(CXAudioSource));
    m_pXAudio = pXAudio;
}

CXAudioSource::CXAudioSource(CXAudio* pXAudio, const TCHAR* szFileName, DWORD dwNum)
{
    ZeroMemory(this, sizeof(CXAudioSource));
    m_pXAudio = pXAudio;
    Load(szFileName, dwNum);
}

//------------------------------------------------------------------------
//
//  XAudioSource 繝・せ繝医Λ繧ｯ繧ｿ  
//
//------------------------------------------------------------------------
CXAudioSource::~CXAudioSource()
{
    if (m_bWav) {
       for (DWORD i = 0; i < m_dwSourceNum; i++)  // 縺吶∋縺ｦ縺ｮ繧ｪ繝ｼ繝・ぅ繧ｪ繧ｽ繝ｼ繧ｹ繧堤ｴ譽・
       {
          if (m_pSourceVoice[i]) m_pSourceVoice[i]->DestroyVoice();
          SAFE_DELETE(m_pWavBuffer[i]);
       }
    }
    else {
       // MCI縺ｧ髢九＞縺溘ヵ繧｡繧､繝ｫ繧帝哩縺倥ｋ
       TCHAR AllStr[512];
       _tcscpy_s(AllStr, _T("close "));
       _tcscat_s(AllStr, m_szAliasName);
       mciSendString(AllStr, nullptr, 0, nullptr);
    }
}

//------------------------------------------------------------------------
//
//  繧ｵ繧ｦ繝ｳ繝峨・隱ｭ縺ｿ霎ｼ縺ｿ    
//
//  諡｡蠑ｵ蟄舌′ .wav 縺ｪ繧・XAudio2縲√◎繧御ｻ･螟厄ｼ・mp3, .mid遲会ｼ峨↑繧・MCI 繧剃ｽｿ逕ｨ縺吶ｋ縲・
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::Load(const TCHAR* szFileName, DWORD dwNum)
{
    TCHAR ext[50];

    // 繝輔ぃ繧､繝ｫ蜷阪°繧画僑蠑ｵ蟄舌ｒ蜿門ｾ・
    _tsplitpath_s(szFileName, NULL, 0, NULL, 0, NULL, 0, ext, sizeof(ext)/sizeof(TCHAR));

    if ( m_pXAudio->XAudio2() && ( _tcscmp(ext, _T(".wav")) == 0 || _tcscmp(ext, _T(".WAV")) == 0 ) )
    {
       m_bWav = true; // WAV繝輔ぃ繧､繝ｫ縺ｪ縺ｮ縺ｧ XAudio2 繧剃ｽｿ逕ｨ
       LoadAudio(szFileName, dwNum);
    }
    else {
       m_bWav = false; // 縺昴ｌ莉･螟厄ｼ・GM遲会ｼ峨・ MCI 繧剃ｽｿ逕ｨ
       LoadMci(szFileName);
    }
    return S_OK;
}

//------------------------------------------------------------------------
//
//  XAudio・・AV・峨・隱ｭ縺ｿ霎ｼ縺ｿ  
//  蜷後§髻ｳ繧定､・焚隱ｭ縺ｿ霎ｼ繧縺薙→縺ｧ縲・㍾縺ｪ繧雁・逕滂ｼ医す繝ｧ繝・ヨ髻ｳ縺ｪ縺ｩ・峨ｒ蜿ｯ閭ｽ縺ｫ縺吶ｋ縲・
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadAudio(const TCHAR* szFileName, DWORD dwSourceNum)
{
    m_dwSourceNum = dwSourceNum;
    if (m_dwSourceNum < 1) m_dwSourceNum = 1;
    if (m_dwSourceNum > AUDIO_SOURCE_MAX) m_dwSourceNum = AUDIO_SOURCE_MAX;

    for (DWORD i = 0; i < m_dwSourceNum; i++) 
    {
       LoadAudioSub( szFileName, i ); 
    }
    m_dwSourceIndex = 0;
    return S_OK;
}

//------------------------------------------------------------------------
//
//  XAudio 隱ｭ縺ｿ霎ｼ縺ｿ繧ｵ繝悶Ν繝ｼ繝√Φ・・AV隗｣譫撰ｼ・
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadAudioSub(const TCHAR* szFileName, DWORD dwIndex)
{
    HMMIO         hMmio = nullptr;  // Windows繝槭Ν繝√Γ繝・ぅ繧｢API縺ｮ繝上Φ繝峨Ν
    DWORD         dwWavSize = 0;    // 豕｢蠖｢繝・・繧ｿ縺ｮ繧ｵ繧､繧ｺ
    WAVEFORMATEX* pwfex;            // WAV繝輔か繝ｼ繝槭ャ繝域ｧ矩菴・
    MMCKINFO      ckInfo;           // 蟄舌メ繝｣繝ｳ繧ｯ諠・ｱ
    MMCKINFO      riffckInfo = { 0 }; // 隕ｪ繝√Ε繝ｳ繧ｯ・・IFF・画ュ蝣ｱ
    PCMWAVEFORMAT pcmWaveForm;
    MMIOINFO      mmioInfo;

    TCHAR FName[256];
    _tcscpy_s(FName, szFileName);

    // mmioOpen 縺ｧ WAV繝輔ぃ繧､繝ｫ繧帝幕縺・
    ZeroMemory(&mmioInfo, sizeof(MMIOINFO));
    hMmio = mmioOpen(FName, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);
    if ( hMmio == nullptr )
    {
       MessageBox(0, _T("繧ｪ繝ｼ繝・ぅ繧ｪ繝輔ぃ繧､繝ｫ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺ｫ螟ｱ謨励＠縺ｾ縺励◆"), FName, MB_OK);
       return E_FAIL;
    }

    // RIFF繝√Ε繝ｳ繧ｯ縺ｸ髯阪ｊ繧・
    mmioDescend(hMmio, &riffckInfo, nullptr, 0);

    // 'fmt ' 繝√Ε繝ｳ繧ｯ繧呈爾縺励※隱ｭ縺ｿ霎ｼ繧
    ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

    // 繝輔か繝ｼ繝槭ャ繝域ュ蝣ｱ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
    mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
    pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
    memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
    pwfex->cbSize = 0;
    mmioAscend(hMmio, &ckInfo, 0); // 繝√Ε繝ｳ繧ｯ繧剃ｸ翫ｋ

    // 'data' 繝√Ε繝ｳ繧ｯ繧呈爾縺励※豕｢蠖｢繝・・繧ｿ繧定ｪｭ縺ｿ霎ｼ繧
    ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);
    dwWavSize = ckInfo.cksize;
    m_pWavBuffer[dwIndex] = new BYTE[dwWavSize];
    mmioRead(hMmio, (HPSTR)m_pWavBuffer[dwIndex], dwWavSize);

    // 繧ｽ繝ｼ繧ｹ繝懊う繧ｹ縺ｮ菴懈・
    if (FAILED(m_pXAudio->XAudio2()->CreateSourceVoice(&m_pSourceVoice[dwIndex], pwfex)))
    {
       MessageBox(0, _T("XAudio2 繧ｽ繝ｼ繧ｹ繝懊う繧ｹ縺ｮ菴懈・縺ｫ螟ｱ謨励＠縺ｾ縺励◆"), 0, MB_OK);
       return E_FAIL;
    }
    m_dwWavSize[dwIndex] = dwWavSize;

    SAFE_DELETE_ARRAY(pwfex);
    mmioClose(hMmio, 0);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  MCI 縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ (MP3, MIDI 遲・
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadMci(const TCHAR* szFileName)
{
    TCHAR AllStr[512];
    TCHAR exe[256];

    // 繝輔ぃ繧､繝ｫ蜷搾ｼ域僑蠑ｵ蟄舌↑縺暦ｼ峨ｒ繧ｨ繧､繝ｪ繧｢繧ｹ縺ｨ縺励※菴ｿ逕ｨ
    _tsplitpath_s(szFileName, NULL, 0, NULL, 0, m_szAliasName, sizeof(m_szAliasName)/sizeof(TCHAR), exe, 256);

    // MCI繧ｳ繝槭Φ繝画枚蟄怜・縺ｮ逕滓・・・"open 繝輔ぃ繧､繝ｫ蜷・type mpegvideo alias 蛻･蜷・
    _tcscpy_s( AllStr, _T("open "));
    _tcscat_s( AllStr, szFileName);
    _tcscat_s( AllStr, _T(" type mpegvideo alias "));
    _tcscat_s( AllStr, m_szAliasName);

    mciSendString( AllStr, nullptr, 0, nullptr);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  蜀咲函蜃ｦ逅・ｼ・AV / MCI 蛻・ｲ撰ｼ・
//
//------------------------------------------------------------------------
void CXAudioSource::Play(int loop)
{
    if (m_bWav) PlayAudio(loop);
    else PlayMci(loop);
}

//------------------------------------------------------------------------
//
//  XAudio2 蜀咲函 (蜉ｹ譫憺浹逕ｨ)
//
//------------------------------------------------------------------------
void CXAudioSource::PlayAudio(int loop)
{
    // 蜀咲函荳ｭ縺ｮ蝣ｴ蜷医・荳譌ｦ蛛懈ｭ｢縺励※繝舌ャ繝輔ぃ繧偵ヵ繝ｩ繝・す繝･・磯ｭ蜃ｺ縺暦ｼ・
    m_pSourceVoice[m_dwSourceIndex]->Stop(0, 0);
    m_pSourceVoice[m_dwSourceIndex]->FlushSourceBuffers();

    XAUDIO2_BUFFER buffer;
    ZeroMemory( &buffer, sizeof(XAUDIO2_BUFFER));

    buffer.pAudioData = m_pWavBuffer[m_dwSourceIndex];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = m_dwWavSize[m_dwSourceIndex];
    
    // 繝ｫ繝ｼ繝苓ｨｭ螳・
    if (loop == AUDIO_LOOP) buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    else buffer.LoopCount = 0;

    // 繝舌ャ繝輔ぃ縺ｮ繧ｵ繝悶Α繝・ヨ縺ｨ蜀咲函髢句ｧ・
    if (FAILED(m_pSourceVoice[m_dwSourceIndex]->SubmitSourceBuffer(&buffer)))
    {
       MessageBox(0, _T("XAudio2 繝舌ャ繝輔ぃ縺ｮ霆｢騾√↓螟ｱ謨励＠縺ｾ縺励◆"), 0, MB_OK);
       return;
    }
    m_pSourceVoice[m_dwSourceIndex]->Start(0, XAUDIO2_COMMIT_NOW);

    // 谺｡縺ｫ菴ｿ縺・た繝ｼ繧ｹ繧､繝ｳ繝・ャ繧ｯ繧ｹ繧呈峩譁ｰ・磯㍾縺ｪ繧雁・逕溽畑・・
    m_dwSourceIndex++;
    if (m_dwSourceIndex >= m_dwSourceNum) m_dwSourceIndex = 0;
}

//------------------------------------------------------------------------
//
//  MCI 蜀咲函 (BGM逕ｨ)
//
//------------------------------------------------------------------------
void CXAudioSource::PlayMci(int loop)
{
    TCHAR AllStr[512];

    _tcscpy_s(AllStr, _T("play "));
    _tcscat_s(AllStr, m_szAliasName);
    _tcscat_s(AllStr, _T(" from 0"));

    if (loop == AUDIO_LOOP) _tcscat_s(AllStr, _T(" repeat"));

    mciSendString(AllStr, nullptr, 0, nullptr);
}

//------------------------------------------------------------------------
//
//  蛛懈ｭ｢蜃ｦ逅・
//
//------------------------------------------------------------------------
void CXAudioSource::Stop()
{
    if (m_bWav) StopAudio();
    else StopMci();
}

void CXAudioSource::StopAudio()
{
    // 驥阪↑繧雁・逕溘＠縺ｦ縺・ｋ蜈ｨ繧ｽ繝ｼ繧ｹ繧貞●豁｢
    for (DWORD i = 0; i < m_dwSourceNum; i++) {
       m_pSourceVoice[i]->Stop(0, 0);
       m_pSourceVoice[i]->FlushSourceBuffers();
    }
    m_dwSourceIndex = 0;
}

void CXAudioSource::StopMci()
{
    TCHAR AllStr[512];
    _tcscpy_s(AllStr, _T("stop "));
    _tcscat_s(AllStr, m_szAliasName);
    mciSendString(AllStr, nullptr, 0, nullptr);
}

//------------------------------------------------------------------------
//
//  髻ｳ驥剰ｨｭ螳・
//
//------------------------------------------------------------------------
void CXAudioSource::Volume(float fVol)
{
    if (m_bWav) VolumeAudio(fVol);
    else VolumeMci((int)(fVol * 1000)); // MCI縺ｯ0-1000縺ｧ謖・ｮ・
}

void CXAudioSource::VolumeAudio(float fVol)
{
    // 蜈ｨ繧ｽ繝ｼ繧ｹ繝懊う繧ｹ縺ｮ髻ｳ驥上ｒ險ｭ螳・(1.0f縺悟渕貅・
    for (DWORD i = 0; i < m_dwSourceNum; i++) {
       m_pSourceVoice[i]->SetVolume(fVol);
    }
}

void CXAudioSource::VolumeMci(int nVol)
{
    TCHAR str[512];
    _stprintf_s(str, _T("setaudio %s volume to %d"), m_szAliasName, nVol);
    mciSendString(str, nullptr, 0, nullptr);
}