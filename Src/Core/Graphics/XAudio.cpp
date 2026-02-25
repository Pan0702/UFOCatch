// =========================================================================================
//
//  波形オーディオ管理
//                                                  ver 3.3        2024.3.23
// =========================================================================================

// ヘッダーファイルのインクルード
#include "XAudio.h"
#include "../Game/GameMain.h"


//------------------------------------------------------------------------
//
//  XAudio2 コンストラクタ 
//
//------------------------------------------------------------------------
CXAudio::CXAudio()
{
    ZeroMemory(this, sizeof(CXAudio));
}
//------------------------------------------------------------------------
//
//  XAudio2 デストラクタ   
//
//------------------------------------------------------------------------
CXAudio::~CXAudio()
{
    SAFE_RELEASE(m_pXAudio2);
}

//------------------------------------------------------------------------
//
//   XAudio2 の初期化    
//
//  HWND hWnd         ウィンドウハンドル
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = 異常
//
//------------------------------------------------------------------------
HRESULT CXAudio::InitAudio(HWND hWnd)
{
    UINT32 flags = 0;

    m_hWnd = hWnd;

    // XAudio2 エンジンの作成
    if (FAILED(XAudio2Create( &m_pXAudio2, flags)))
    {
       // -- 2018.7.8 XAudio2のDLLがない等の理由で初期化できない場合でも、
       // MCI（mciSendString）で再生を継続できるように、エラーにはせず続行させる。
       OutputDebugString(_T("警告：XAudio2 の初期化に失敗しました。MCI（mciSendString）で再生を試みます。\n"));
       SAFE_RELEASE(m_pXAudio2);
       return S_OK;
    }

    // マスターボイス（最終出力先）の作成
    if (FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
    {
       MessageBox(0, _T("XAudio2 マスターボイスの作成に失敗しました"), 0, MB_OK);
       return E_FAIL;
    }
    return S_OK;
}

//------------------------------------------------------------------------
//
//  XAudioSource コンストラクタ    
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
//  XAudioSource デストラクタ  
//
//------------------------------------------------------------------------
CXAudioSource::~CXAudioSource()
{
    if (m_bWav) {
       for (DWORD i = 0; i < m_dwSourceNum; i++)  // すべてのオーディオソースを破棄
       {
          if (m_pSourceVoice[i]) m_pSourceVoice[i]->DestroyVoice();
          SAFE_DELETE(m_pWavBuffer[i]);
       }
    }
    else {
       // MCIで開いたファイルを閉じる
       TCHAR AllStr[512];
       _tcscpy_s(AllStr, _T("close "));
       _tcscat_s(AllStr, m_szAliasName);
       mciSendString(AllStr, nullptr, 0, nullptr);
    }
}

//------------------------------------------------------------------------
//
//  サウンドの読み込み    
//
//  拡張子が .wav なら XAudio2、それ以外（.mp3, .mid等）なら MCI を使用する。
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::Load(const TCHAR* szFileName, DWORD dwNum)
{
    TCHAR ext[50];

    // ファイル名から拡張子を取得
    _tsplitpath_s(szFileName, NULL, 0, NULL, 0, NULL, 0, ext, sizeof(ext)/sizeof(TCHAR));

    if ( m_pXAudio->XAudio2() && ( _tcscmp(ext, _T(".wav")) == 0 || _tcscmp(ext, _T(".WAV")) == 0 ) )
    {
       m_bWav = true; // WAVファイルなので XAudio2 を使用
       LoadAudio(szFileName, dwNum);
    }
    else {
       m_bWav = false; // それ以外（BGM等）は MCI を使用
       LoadMci(szFileName);
    }
    return S_OK;
}

//------------------------------------------------------------------------
//
//  XAudio（WAV）の読み込み  
//  同じ音を複数読み込むことで、重なり再生（ショット音など）を可能にする。
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
//  XAudio 読み込みサブルーチン（WAV解析）
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadAudioSub(const TCHAR* szFileName, DWORD dwIndex)
{
    HMMIO         hMmio = nullptr;  // WindowsマルチメディアAPIのハンドル
    DWORD         dwWavSize = 0;    // 波形データのサイズ
    WAVEFORMATEX* pwfex;            // WAVフォーマット構造体
    MMCKINFO      ckInfo;           // 子チャンク情報
    MMCKINFO      riffckInfo = { 0 }; // 親チャンク（RIFF）情報
    PCMWAVEFORMAT pcmWaveForm;
    MMIOINFO      mmioInfo;

    TCHAR FName[256];
    _tcscpy_s(FName, szFileName);

    // mmioOpen で WAVファイルを開く
    ZeroMemory(&mmioInfo, sizeof(MMIOINFO));
    hMmio = mmioOpen(FName, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ);
    if ( hMmio == nullptr )
    {
       MessageBox(0, _T("オーディオファイルの読み込みに失敗しました"), FName, MB_OK);
       return E_FAIL;
    }

    // RIFFチャンクへ降りる
    mmioDescend(hMmio, &riffckInfo, nullptr, 0);

    // 'fmt ' チャンクを探して読み込む
    ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

    // フォーマット情報の読み込み
    mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
    pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
    memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
    pwfex->cbSize = 0;
    mmioAscend(hMmio, &ckInfo, 0); // チャンクを上る

    // 'data' チャンクを探して波形データを読み込む
    ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);
    dwWavSize = ckInfo.cksize;
    m_pWavBuffer[dwIndex] = new BYTE[dwWavSize];
    mmioRead(hMmio, (HPSTR)m_pWavBuffer[dwIndex], dwWavSize);

    // ソースボイスの作成
    if (FAILED(m_pXAudio->XAudio2()->CreateSourceVoice(&m_pSourceVoice[dwIndex], pwfex)))
    {
       MessageBox(0, _T("XAudio2 ソースボイスの作成に失敗しました"), 0, MB_OK);
       return E_FAIL;
    }
    m_dwWavSize[dwIndex] = dwWavSize;

    SAFE_DELETE_ARRAY(pwfex);
    mmioClose(hMmio, 0);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  MCI の読み込み (MP3, MIDI 等)
//
//------------------------------------------------------------------------
HRESULT CXAudioSource::LoadMci(const TCHAR* szFileName)
{
    TCHAR AllStr[512];
    TCHAR exe[256];

    // ファイル名（拡張子なし）をエイリアスとして使用
    _tsplitpath_s(szFileName, NULL, 0, NULL, 0, m_szAliasName, sizeof(m_szAliasName)/sizeof(TCHAR), exe, 256);

    // MCIコマンド文字列の生成： "open ファイル名 type mpegvideo alias 別名"
    _tcscpy_s( AllStr, _T("open "));
    _tcscat_s( AllStr, szFileName);
    _tcscat_s( AllStr, _T(" type mpegvideo alias "));
    _tcscat_s( AllStr, m_szAliasName);

    mciSendString( AllStr, nullptr, 0, nullptr);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  再生処理（WAV / MCI 分岐）
//
//------------------------------------------------------------------------
void CXAudioSource::Play(int loop)
{
    if (m_bWav) PlayAudio(loop);
    else PlayMci(loop);
}

//------------------------------------------------------------------------
//
//  XAudio2 再生 (効果音用)
//
//------------------------------------------------------------------------
void CXAudioSource::PlayAudio(int loop)
{
    // 再生中の場合は一旦停止してバッファをフラッシュ（頭出し）
    m_pSourceVoice[m_dwSourceIndex]->Stop(0, 0);
    m_pSourceVoice[m_dwSourceIndex]->FlushSourceBuffers();

    XAUDIO2_BUFFER buffer;
    ZeroMemory( &buffer, sizeof(XAUDIO2_BUFFER));

    buffer.pAudioData = m_pWavBuffer[m_dwSourceIndex];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = m_dwWavSize[m_dwSourceIndex];
    
    // ループ設定
    if (loop == AUDIO_LOOP) buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    else buffer.LoopCount = 0;

    // バッファのサブミットと再生開始
    if (FAILED(m_pSourceVoice[m_dwSourceIndex]->SubmitSourceBuffer(&buffer)))
    {
       MessageBox(0, _T("XAudio2 バッファの転送に失敗しました"), 0, MB_OK);
       return;
    }
    m_pSourceVoice[m_dwSourceIndex]->Start(0, XAUDIO2_COMMIT_NOW);

    // 次に使うソースインデックスを更新（重なり再生用）
    m_dwSourceIndex++;
    if (m_dwSourceIndex >= m_dwSourceNum) m_dwSourceIndex = 0;
}

//------------------------------------------------------------------------
//
//  MCI 再生 (BGM用)
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
//  停止処理
//
//------------------------------------------------------------------------
void CXAudioSource::Stop()
{
    if (m_bWav) StopAudio();
    else StopMci();
}

void CXAudioSource::StopAudio()
{
    // 重なり再生している全ソースを停止
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
//  音量設定
//
//------------------------------------------------------------------------
void CXAudioSource::Volume(float fVol)
{
    if (m_bWav) VolumeAudio(fVol);
    else VolumeMci((int)(fVol * 1000)); // MCIは0-1000で指定
}

void CXAudioSource::VolumeAudio(float fVol)
{
    // 全ソースボイスの音量を設定 (1.0fが基準)
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