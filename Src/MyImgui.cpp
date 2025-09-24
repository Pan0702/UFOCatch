//=============================================================================
//
//	ImGuiを使いやすくするためのライブラリ                Ver 2.0     2020.11.15
//
//	このソースファイルでは、./Libs/Imgui フォルダに入っている imguiシステムの
//	ヘッダとソースを使用している
//	また、imguiのimgui_impl_win32.h/imgui_impl_win32.cppはカスタマイズしてある。
//
//																	MyImgui.cpp
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
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// ウィンドウの色の設定
		//ImGui::StyleColorsLight();    // 明るい灰色
		ImGui::StyleColorsClassic();    // 黒の半透明
		//ImGui::StyleColorsDark();     // 黒

		// （注）ImGui_ImplWin32_Init()は、オリジナルの関数をカスタマイズしてある
		if (!ImGui_ImplWin32_Init(hWnd, WidthIn, HeightIn))   // -- 2020.8.7
		{
			MessageBox(0, _T("imguiを初期化出来ません"), nullptr, MB_OK);
		}
		if (!ImGui_ImplDX11_Init(pD3D->m_pDevice, pD3D->m_pDeviceContext))
		{
			MessageBox(0, _T("imguiを初期化出来ません"), nullptr, MB_OK);
		}

		// iniファイルを生成しないように
		io.IniFilename = nullptr;

		// フォントのパスを取得
		char FontPath[MAX_PATH];  // パス取得バッファ
		SHGetSpecialFolderPathA(nullptr, FontPath, CSIDL_FONTS, 0);
		strcat_s(FontPath, "\\meiryo.ttc");

		// 日本語フォントに対応
		//io.Fonts->AddFontFromFileTTF(FontPath, 18.0f, nullptrptr, io.Fonts->GetGlyphRangesJapanese());

		// 日本語フォントに対応(ただし、半角文字はDefaultのまま表示)
		ImFontConfig imgui_config;
		imgui_config.MergeMode = true;
		io.Fonts->AddFontDefault();
		io.Fonts->AddFontFromFileTTF(FontPath, 18.0f, &imgui_config, io.Fonts->GetGlyphRangesJapanese());

		io.ImeWindowHandle = ImmGetDefaultIMEWnd(hWnd);   // 日本語入力対応か？ 
		//io.ImeWindowHandle = hWnd;                        // 日本語入力対応か。その２？

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

		// MyImguiの更新
		//Update();              // デモウィンドウの表示

		// 描画後処理（実描画）
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

		// デモウィンドウ1表示処理
		// このウィンドウImGui::ShowDemoWindow()は、システムに最初から用意されているデモウィンドウである
		static bool p_open = false;
		if (io.KeysDown[VK_F8] == 1)
		{
			p_open = true;
		}
		if( p_open ) ImGui::ShowDemoWindow(&p_open);

		// デモウィンドウ2表示処理
		// このウィンドウは、MyImguiで作成したデモウィンドウである
		static bool p_open2 = false;
		if (io.KeysDown[VK_F9] == 1)
		{
			p_open2 = true;
		}
		if( p_open2) ShowDemoWindow2(&p_open2);

	}


	//-----------------------------------------------------------------------------
	//
	// デモ用ウィンドウ(DemoWindow2)の表示
	//
	// 引数
	//		bool* p_open : 終了(閉じるボタン)フラグ  falseで終了
	//
	//-----------------------------------------------------------------------------
	void ShowDemoWindow2(bool* p_open)
	{

		// 背景とタイトルバーの色の設定
		// （注）PushStyleColor()と1:1で終了時のPopStyleColor()が必要
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.2f, 0.2f, 0.7f));          // 通常時の背景色
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.5f, 0.1f, 0.5f));           // 通常時（フォーカスがないとき）のタイトル色
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 0.8f));     // アクティブ時（フォーカスが当たっているとき）のタイトル色

		// ウィンドウ位置とサイズを設定します。
		//   ImGuiCond_Once により、初回のみ設定されます。
		//   ImGuiCond_Always で、常に設定することもできます。
		ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_Once);

		// 各フレームの枠線
		//ImGui::GetStyle().FrameRounding = 3.0f;      // フレームのコーナー丸
		ImGui::GetStyle().FrameBorderSize = 1.0f;    // 枠線の太さ
		//ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 1);    // 枠線の色
		//ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 1);    // 枠線の影の色

		// ウィンドウ定義の設定
		// ・Beginでウインドウの名前設定。なお、名前はウインドウごとにユニークになるようにすること
		// ・p_openは、閉じるボタンを押したときfalseが返る
		// ・flagは、ImGuiWindowFlags_MenuBar, ImGuiWindowFlags_NoResize など
		if (ImGui::Begin(u8"ImGui デモウインドウ２のタイトルバー", p_open))
		{
			// 最初だけ大きさを設定   不要
			//ImGui::SetWindowSize(ImVec2(400, 600), ImGuiCond_::ImGuiCond_FirstUseEver);

			// -----------------------------------------------
			// テキスト表示。また、SameLine()で同一行に２文表示も可
			ImGui::Text(u8"テキストコンテンツを表示。");
			ImGui::SameLine();
			ImGui::Text(u8"また、同じ行に追加することもできる");

			// 区切り線
			ImGui::Separator();

			// --------------------------------------------------
			// チェックボックス
			static bool chkboxflag = false;
			ImGui::Checkbox(u8"チェックボックスを表示する", &chkboxflag);

			ImGui::Separator();

			// カラーピッカー -----------------------------------

			ImGui::PushItemWidth(220);     // 項目の幅を220にする。必ずPopItemWidth()と対にする

			// ・ホイールタイプ・透明度バーを表示。ColorEditFlagを指定しないとボードタイプ。このときは、右クリックで形式を選択できる
			static float Colorpick[4] = { 0 };
			ImGui::ColorPicker4(u8"カラーピッカー", Colorpick,
				ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
			//ImGui::ColorPicker4(u8"カラーピッカー", Colorpick);
		
			ImGui::PopItemWidth();     // 項目の幅を元に戻す

			ImGui::Separator();

			// テキスト・数字入力のいろいろ -------------------------

			// 半角英数文字の入力
			static char str[256] = { "" };   // char型
			//ImGui::InputTextMultiline(u8"半角英数文字の入力", str, 256, ImVec2(200,50));   // 複数行入力
			ImGui::InputText(u8"半角英数文字の入力", str, 256);   // 一行入力



			// 日本語テキストの入力
			// ・TCHAR型を一旦UTF-8(char)型に変換して入力する
			// ・つまり、　TCHAR型→UTF-8(char)型→（入力）→UTF-8(char)型→TCHAR型
			static TCHAR tstr[256] = { _T("") };   // TCHAR型
			char u8str[256];                       // UTF-8(char)型

			// TCHAR型をUTF-8型に変換
			MyImgui::ConvertTCHARToUTF8(tstr, u8str);

			// 日本語テキスト入力
			//ImGui::InputTextMultiline(u8"日本語の入力", u8str, 256, ImVec2(200,50));   // 複数行入力
			ImGui::InputText(u8"日本語の入力", u8str, 256);   // 一行入力

			// UTF-8型をTCHAR型に変換
			MyImgui::ConvertUTF8ToTCHAR(u8str, tstr);


			ImGui::Separator();


			// 数字の入力
			static int su1 = 0;
			//ImGui::DragInt(u8"数字の入力", &su1);
			ImGui::InputInt(u8"数字の入力", &su1);


			// 4数字の入力
			static int su2[4] = { 0 };
			ImGui::SliderInt4(u8"４数値の入力", su2, 0, 255);

			ImGui::Separator();


			// コンボボックスのいろいろ ------------------------

			// ① コンボボックス1
			// 配列itemsの内容を１項目づつコンボボックスに設定して表示する
			// クリックした位置の文字列ポインターが選択ポインターitem_currentに設定される
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
			static const char* item_current = items[0];        // 項目リストの先頭位置を初期選択ポインターとする.
			if (ImGui::BeginCombo("Combo 1", item_current, 0)) // 2番目のパラメータは、コンボを開く前に選択されたポインターである.
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					bool is_selected = (item_current == items[n]);   // 現在のリスト位置が、選択ポインター位置と一致しているか判断する
					if (ImGui::Selectable(items[n], is_selected))    // リストをコンボボックスに追加する。選択位置と一致していたらtrueを返す
						item_current = items[n];                     // 選択位置を選択ポインターに設定する
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // コンボを開くときの初期フォーカスを設定する (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}
				ImGui::EndCombo();
			}

			// ② コンボボックス2
			// 表示文字列を一行(single constant string)で直接設定して表示する
			// クリックした位置が選択ポインターitem_current_2に設定される
			static int item_current_2 = 1;    // 初期カーソル位置を１とする
			ImGui::Combo("Combo 2", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

			// ③ コンボボックス3
			// 表示文字列を配列（上記、配列items）を使用して表示する
			// クリックした位置が選択ポインターitem_current_2に設定される
			static int item_current_3 = -1;    // 初期カーソル位置を選択しないとする
			ImGui::Combo("Combo 3", &item_current_3, items, IM_ARRAYSIZE(items));

			ImGui::Separator();


			// リストボックスのいろいろ ---------------------------

			ImGui::PushItemWidth(100);     // 項目の幅を100にする。必ずPopItemWidth()と対にする

			// ① リストボックス１
			// 配列list_itemの内容を１項目づつリストボックスに設定して表示する
			// クリックした位置が選択ポインターitem_countに設定される
			const char* list_item[] = { "TTTTT", "UUUUU", "VVVVV", "WWWWW", "XXXXX", "YYYYY", "ZZZZZ" };
			static int  item_count = 0;    // 項目の選択ポインター（選択位置）

			if (ImGui::ListBoxHeader(u8"ListBox 1", item_count, 3)) // 2番目のパラメータは、選択されたポインターである。3番目のパラメータは、ボックスに表示する項目数である。
			{
				for (int n = 0; n < IM_ARRAYSIZE(list_item); n++)
				{
					bool is_selected = (item_count == n);  // 現在のリスト位置が、選択ポインター位置と一致しているか判断する
					ImGui::PushID(n);
					if (ImGui::Selectable(list_item[n], is_selected))   // リストをリストボックスに追加する。選択位置と一致していたらtrueを返す
					{
						item_count = n;                    // 選択位置を選択ポインターに設定する
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // リストを開くときの初期フォーカスを設定する
					ImGui::PopID();
				}
				ImGui::ListBoxFooter();
			}

			ImGui::SameLine();

			// ② リストボックス２
			// 配列list_item2の内容を使用してリストボックスを表示する
			// クリックした位置が選択ポインターitem_count2に設定される
			const char* list_item2[] = { "MMMMM", "NNNNN", "OOOOO", "PPPPP", "QQQQQ", "RRRRR", "SSSSS" };
			static int  item_count2 = 0;    // 項目の選択ポインター（選択位置）

			ImGui::ListBox(u8"ListBox 2", &item_count2, list_item2, IM_ARRAYSIZE(list_item2), 3); // 5番目のパラメータは、ボックスに表示する項目数である。

			ImGui::PopItemWidth();     // 項目の幅を元に戻す

			// 区切り線
			ImGui::Separator();

			ImGui::BeginChild(u8"チャイルドウィンドウ", ImVec2(260, 50), true);

				ImGui::Text(u8"Ｎｏ．１");
				ImGui::Text(u8"Ｎｏ．２");
				ImGui::Text(u8"Ｎｏ．３");
				ImGui::Text(u8"Ｎｏ．４");
				ImGui::Text(u8"Ｎｏ．５");
				ImGui::Text(u8"Ｎｏ．６");
				ImGui::Text(u8"Ｎｏ．７");
				ImGui::Text(u8"Ｎｏ．８");

			ImGui::EndChild();
			ImGui::Separator();

			// -------------------------------------------------
			// ボタン入力
			ImGui::Text("                                      ");
			ImGui::SameLine();
			if (ImGui::Button(u8"終了", ImVec2(60, 20)))
			{
				// ボタンを押したとき、*p_openにfalseを返す
				*p_open = false;
			}
		}

		ImGui::End();

		// 色設定(背景とタイトルバー)の後始末。
		// 上記PushStyleColor()と1:1でpopStyleColor()が必要
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

	}

	//-----------------------------------------------------------------------------
	//
	// UTF-8からTCHARへの変換関数
	//
	// 引数
	//   char*  charIn        入力するUTF-8文字列
	//   TCHAR* tcharOut      変換後のTCHAR文字列(Out)
	//
	//-----------------------------------------------------------------------------
	void ConvertUTF8ToTCHAR(char* charIn, TCHAR* tcharOut)
	{

	#if _UNICODE
		// unicodeの場合
		// UTF-8からUTF-16への変換
		ConvertU8ToU16(charIn, tcharOut);

	#else
		// マルチバイト文字の場合
		// ① UTF-8からUTF-16への変換
		WCHAR wstr[512];
		ConvertU8ToU16(charIn, wstr);

		// ② Unicode 文字コード(WCHAR)を第一引数で指定した文字コードに変換する( CP_ACP は日本語WindowsではシフトJISコード )
		char  mstr[512];
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, mstr, 512, nullptr, nullptr);
		strcpy_s(tcharOut, strlen(mstr) + 1, mstr);

	#endif
	}
	//-----------------------------------------------------------------------------
	//
	// TCHARからUTF-8への変換関数
	//
	// 引数
	//   TCHAR* tcharIn       入力するTCHAR文字列
	//   char*  charOut       変換後のUTF-8文字列(Out)
	//
	//-----------------------------------------------------------------------------
	void ConvertTCHARToUTF8(TCHAR* tcharIn, char* charOut)
	{

	#if _UNICODE
		// unicodeの場合
		// UTF-16からUTF-8への変換
		ConvertU16ToU8(tcharIn, charOut);

	#else
		// マルチバイト文字の場合
		// ① 入力文字列のマルチバイト(char)型をWCHAR型に変換
		WCHAR wstr[512] = { L'\0' };
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, tcharIn, -1, wstr, 512);
		// ② WCHAR型をUTF-8に変換する
		ConvertU16ToU8(wstr, charOut);

	#endif

	}


	//-----------------------------------------------------------------------------
	//
	// UTF-8からUTF-16への変換関数
	//
	// 引数
	//   char*  charIn        入力するUTF-8文字列
	//   WCHAR* wcharOut      変換後のUTF-16文字列(Out)
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
	// UTF-16からUTF-8への変換関数
	//
	// 引数
	//   WCHAR* wcharIn       入力するUTF-16文字列
	//   char*  charOut       変換後のUTF-8文字列(Out)
	//
	//-----------------------------------------------------------------------------
	void ConvertU16ToU8(WCHAR* wcharIn, char* charOut)
	{

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convt;
		std::string ch = convt.to_bytes(wcharIn);

		strcpy_s(charOut, ch.length() + 1, ch.c_str());

	}

}