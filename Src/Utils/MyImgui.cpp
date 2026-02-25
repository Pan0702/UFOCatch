//=============================================================================
//
//  ImGuiを使いやすくするためのライブラリ                Ver 2.0     2020.11.15
//
//  このソースファイルでは、./Libs/Imgui フォルダに入っている imguiシステム
//  ヘッダーとソースを使用している。
//  また、imguiの imgui_impl_win32.h/imgui_impl_win32.cpp をカスタマイズしている。
//
//                                                  MyImgui.cpp
//=============================================================================
#include  "MyImgui.h"

// convert_utf8            // -- 2020.8.7
#include <codecvt>


// ============================================================================
// MyImgui関数
// namespace MyImgui
// ============================================================================
namespace MyImgui
{
    //=============================================================================
    //
    // MyImguiの初期化
    //
    // 引数
    //   HWND       hWnd      ウィンドウハンドル
    //   CDirect3D* pD3D      Direct3D
    //   int        WidthIn   ウィンドウの幅
    //   int        HeightIn  ウィンドウの高さ
    //
    //-----------------------------------------------------------------------------
    void ImguiInit(HWND hWnd, CDirect3D* pD3D, int WidthIn, int HeightIn)
    {
        // imguiの初期化
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // キーボード操作を有効化
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // ゲームパッド操作を有効化

        // ウィンドウの色（スタイル）の設定
        //ImGui::StyleColorsLight();    // 明るい色
        ImGui::StyleColorsClassic();    // 以前の標準色
        //ImGui::StyleColorsDark();     // 暗い色（現在の標準）

        // 【注】ImGui_ImplWin32_Init()は、オリジナルの関数をカスタマイズしている
        if (!ImGui_ImplWin32_Init(hWnd, WidthIn, HeightIn)) // -- 2020.8.7
        {
            MessageBox(0, _T("imgui初期化に失敗しました"), nullptr, MB_OK);
        }
        if (!ImGui_ImplDX11_Init(pD3D->m_pDevice, pD3D->m_pDeviceContext))
        {
            MessageBox(0, _T("imgui初期化に失敗しました"), nullptr, MB_OK);
        }

        // iniファイルを生成しないように設定
        io.IniFilename = nullptr;

        // フォントのパスを取得
        char FontPath[MAX_PATH]; // パス取得バッファ
        SHGetSpecialFolderPathA(nullptr, FontPath, CSIDL_FONTS, 0);
        strcat_s(FontPath, "\\meiryo.ttc");

        // 日本語フォントに対応
        //io.Fonts->AddFontFromFileTTF(FontPath, 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

        // 日本語フォントに対応(英数字はDefaultのまま表示し、日本語のみマージする設定)
        ImFontConfig imgui_config;
        imgui_config.MergeMode = true;
        io.Fonts->AddFontDefault();
        io.Fonts->AddFontFromFileTTF(FontPath, 18.0f, &imgui_config, io.Fonts->GetGlyphRangesJapanese());
        io.Fonts->AddFontFromFileTTF("C:/WINDOWS/FONTS/BIZ-UDGOTHICR.TTC",
                             18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        
        io.ImeWindowHandle = ImmGetDefaultIMEWnd(hWnd); // 日本語入力に対応
    }

    //-----------------------------------------------------------------------------
    //
    // MyImguiの終了処理
    //
    //-----------------------------------------------------------------------------
    void ImguiQuit()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    //-----------------------------------------------------------------------------
    //
    // MyImguiの描画前処理
    //
    //-----------------------------------------------------------------------------
    void ImguiNewFrame()
    {
        // 描画前処理
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();
    }

    //-----------------------------------------------------------------------------
    //
    // MyImguiの実描画処理
    //
    //-----------------------------------------------------------------------------
    void ImguiRender()
    {
        // 描画後処理（レンダリング）
        ImGui::Render();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    //-----------------------------------------------------------------------------
    //
    // MyImgui全体の更新処理
    //
    //-----------------------------------------------------------------------------
    void Update()
    {
        ImGuiIO& io = ImGui::GetIO();

        // デモウィンドウ1を表示する
        // このウィンドウは ImGui::ShowDemoWindow() というシステムに標準で用意されているものです
        static bool p_open = false;
        if (io.KeysDown[VK_F8] == 1)
        {
            p_open = true;
        }
        if (p_open) ImGui::ShowDemoWindow(&p_open);

        // デモウィンドウ2を表示する
        // このウィンドウは MyImguiで独自作成したデモウィンドウです
        static bool p_open2 = false;
        if (io.KeysDown[VK_F9] == 1)
        {
            p_open2 = true;
        }
        if (p_open2) ShowDemoWindow2(&p_open2);
    }


    //-----------------------------------------------------------------------------
    //
    // デモ用ウィンドウ(DemoWindow2)の表示
    //
    // 引数
    //     bool* p_open : 終了(閉じるボタン)フラグ  falseで終了
    //
    //-----------------------------------------------------------------------------
    void ShowDemoWindow2(bool* p_open)
    {
        // 背景とタイトルバーの色を設定
        // 【注】PushStyleColor()は呼び出した回数分、最後でPopStyleColor()が必要
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.2f, 0.2f, 0.7f)); // 通常時の背景色
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.5f, 0.1f, 0.5f)); // 通常時（フォーカスがないとき）のタイトル色
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 0.8f)); // アクティブ時（フォーカスがあるとき）のタイトル色

        // ウィンドウ位置とサイズを設定します。
        //   ImGuiCond_Once により、初回のみ設定されます。
        //   ImGuiCond_Always で、常に固定することもできます。
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_Once);

        // 各フレームの枠線設定
        ImGui::GetStyle().FrameBorderSize = 1.0f; // 枠線の太さ

        // ウィンドウ描画の設定
        // ・Beginでウィンドウの名前設定。なお、名前はウィンドウごとにユニークにすること
        // ・p_openにより、閉じるボタンが押されたときにfalseを返す
        if (ImGui::Begin(u8"ImGui デモウィンドウ2のタイトルバー", p_open))
        {
            // -----------------------------------------------
            // テキスト表示。SameLine()で同一行に並べて表示
            ImGui::Text(u8"テキストコンテンツを表示。");
            ImGui::SameLine();
            ImGui::Text(u8"また、このように横に追加することもできる");

            // 区切り線
            ImGui::Separator();

            // --------------------------------------------------
            // チェックボックス
            static bool chkboxflag = false;
            ImGui::Checkbox(u8"チェックボックスを表示する", &chkboxflag);

            ImGui::Separator();

            // カラーピッカー -----------------------------------

            ImGui::PushItemWidth(220); // 項目の幅を220にする。必ずPopItemWidth()と対にする

            // ホイールタイプ・色のアルファバーを表示。PickerHueWheelを指定しないとボードタイプ
            static float Colorpick[4] = {0};
            ImGui::ColorPicker4(u8"カラーピッカー", Colorpick,
                                ImGuiColorEditFlags_PickerHueWheel |
                                ImGuiColorEditFlags_AlphaBar);

            ImGui::PopItemWidth(); // 項目の幅設定を戻す

            ImGui::Separator();

            // テキスト・数値入力のいろいろ -------------------------

            // 英数パスワード等の入力
            static char str[256] = {""}; // char型
            ImGui::InputText(u8"英数文字列の入力", str, 256); // 単行入力


            // 日本語テキストの入力
            // TCHAR型を一度UTF-8(char)型に変換して入力する
            // つまり、 TCHAR型 → UTF-8型変換 → 入力処理 → UTF-8型 → TCHAR型変換
            static TCHAR tstr[256] = {_T("")}; // TCHAR型
            char u8str[256]; // UTF-8(char)型

            // TCHAR型をUTF-8型に変換
            MyImgui::ConvertTCHARToUTF8(tstr, u8str);

            // 日本語テキスト入力
            ImGui::InputText(u8"日本語の入力", u8str, 256); // 単行入力

            // UTF-8型をTCHAR型に逆変換
            MyImgui::ConvertUTF8ToTCHAR(u8str, tstr);


            ImGui::Separator();


            // 整数の入力
            static int su1 = 0;
            ImGui::InputInt(u8"整数の入力", &su1);


            // 4つの整数の入力（スライダー形式）
            static int su2[4] = {0};
            ImGui::SliderInt4(u8"色の値(RGBA)の入力", su2, 0, 255);

            ImGui::Separator();


            // コンボボックスのいろいろ ------------------------

            // ① コンボボックス1 (ポインタ配列形式)
            const char* items[] = {
                "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL",
                "MMMM", "OOOOOOO"
            };
            static const char* item_current = items[0]; 
            if (ImGui::BeginCombo("Combo 1", item_current, 0))
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    bool is_selected = (item_current == items[n]);
                    if (ImGui::Selectable(items[n], is_selected))
                        item_current = items[n]; 
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // ② コンボボックス2 (NULL文字区切りの単一文字列)
            static int item_current_2 = 1; 
            ImGui::Combo("Combo 2", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

            // ③ コンボボックス3 (配列と要素数指定)
            static int item_current_3 = -1; 
            ImGui::Combo("Combo 3", &item_current_3, items, IM_ARRAYSIZE(items));

            ImGui::Separator();


            // リストボックスのいろいろ ---------------------------

            ImGui::PushItemWidth(100); 

            // ① リストボックス1
            const char* list_item[] = {"TTTTT", "UUUUU", "VVVVV", "WWWWW", "XXXXX", "YYYYY", "ZZZZZ"};
            static int item_count = 0; 

            if (ImGui::ListBoxHeader(u8"ListBox 1", item_count, 3))
            {
                for (int n = 0; n < IM_ARRAYSIZE(list_item); n++)
                {
                    bool is_selected = (item_count == n);
                    ImGui::PushID(n);
                    if (ImGui::Selectable(list_item[n], is_selected))
                    {
                        item_count = n; 
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    ImGui::PopID();
                }
                ImGui::ListBoxFooter();
            }

            ImGui::SameLine();

            // ② リストボックス2 (簡易版)
            const char* list_item2[] = {"MMMMM", "NNNNN", "OOOOO", "PPPPP", "QQQQQ", "RRRRR", "SSSSS"};
            static int item_count2 = 0; 

            ImGui::ListBox(u8"ListBox 2", &item_count2, list_item2, IM_ARRAYSIZE(list_item2), 3);

            ImGui::PopItemWidth(); 

            // 区切り線
            ImGui::Separator();

            // 子ウィンドウの作成
            ImGui::BeginChild(u8"子ウィンドウ(スクロール)", ImVec2(260, 50), true);

            ImGui::Text(u8"項目 1");
            ImGui::Text(u8"項目 2");
            ImGui::Text(u8"項目 3");
            ImGui::Text(u8"項目 4");
            ImGui::Text(u8"項目 5");
            ImGui::Text(u8"項目 6");
            ImGui::Text(u8"項目 7");
            ImGui::Text(u8"項目 8");

            ImGui::EndChild();
            ImGui::Separator();

            // -------------------------------------------------
            // 閉じるボタン
            ImGui::Text("                                      ");
            ImGui::SameLine();
            if (ImGui::Button(u8"閉じる", ImVec2(60, 20)))
            {
                *p_open = false;
            }
        }

        ImGui::End();

        // スタイル色の変更を元に戻す
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    //-----------------------------------------------------------------------------
    //
    // UTF-8からTCHARへの変換関数
    //
    //-----------------------------------------------------------------------------
    void ConvertUTF8ToTCHAR(char* charIn, TCHAR* tcharOut)
    {
#if _UNICODE
        // Unicode(UTF-16)の場合
        ConvertU8ToU16(charIn, tcharOut);
#else
        // マルチバイト(Shift-JIS)の場合
        WCHAR wstr[512];
        ConvertU8ToU16(charIn, wstr);
        char mstr[512];
        WideCharToMultiByte(CP_ACP, 0, wstr, -1, mstr, 512, nullptr, nullptr);
        strcpy_s(tcharOut, strlen(mstr) + 1, mstr);
#endif
    }

    //-----------------------------------------------------------------------------
    //
    // TCHARからUTF-8への変換関数
    //
    //-----------------------------------------------------------------------------
    void ConvertTCHARToUTF8(TCHAR* tcharIn, char* charOut)
    {
#if _UNICODE
        // Unicode(UTF-16)の場合
        ConvertU16ToU8(tcharIn, charOut);
#else
        // マルチバイト(Shift-JIS)の場合
        WCHAR wstr[512] = {L'\0'};
        MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, tcharIn, -1, wstr, 512);
        ConvertU16ToU8(wstr, charOut);
#endif
    }


    //-----------------------------------------------------------------------------
    //
    // UTF-8からUTF-16(WCHAR)への変換
    //
    //-----------------------------------------------------------------------------
    void ConvertU8ToU16(char* charIn, WCHAR* wcharOut)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convt;
        std::wstring wch = convt.from_bytes(charIn);
        wcscpy_s(wcharOut, wch.length() + 1, (WCHAR*)wch.c_str());
    }

    //-----------------------------------------------------------------------------
    //
    // UTF-16(WCHAR)からUTF-8への変換
    //
    //-----------------------------------------------------------------------------
    void ConvertU16ToU8(WCHAR* wcharIn, char* charOut)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convt;
        std::string ch = convt.to_bytes(wcharIn);
        strcpy_s(charOut, ch.length() + 1, ch.c_str());
    }
}