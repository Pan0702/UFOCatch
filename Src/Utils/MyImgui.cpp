//=============================================================================
//
//	ImGui���g���₷�����邽�߂̃��C�u����                Ver 2.0     2020.11.15
//
//	���̃\�[�X�t�@�C���ł́A./Libs/Imgui �t�H���_�ɓ����Ă��� imgui�V�X�e����
//	�w�b�_�ƃ\�[�X���g�p���Ă���
//	�܂��Aimgui��imgui_impl_win32.h/imgui_impl_win32.cpp�̓J�X�^�}�C�Y���Ă���B
//
//																	MyImgui.cpp
//=============================================================================
#include  "MyImgui.h"

// convert_utf8            // -- 2020.8.7
#include <codecvt>


// ============================================================================
// MyImgui�֐�
// namespace MyImgui
// ============================================================================
namespace MyImgui
{
    //=============================================================================
    //
    // MyImgui�̏�����
    //
    // ����
    //   HWND       hWnd      �E�B���h�E�n���h��
    //   CDirect3D* pD3D      Direct3D
    //   int        WidthIn   �E�B���h�E�̕�
    //   int        HeightIn  �E�B���h�E�̍���
    //
    //-----------------------------------------------------------------------------
    void ImguiInit(HWND hWnd, CDirect3D* pD3D, int WidthIn, int HeightIn)
    {
        // imgui�̏�����
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // �E�B���h�E�̐F�̐ݒ�
        //ImGui::StyleColorsLight();    // ���邢�D�F
        ImGui::StyleColorsClassic(); // ���̔�����
        //ImGui::StyleColorsDark();     // ��

        // �i���jImGui_ImplWin32_Init()�́A�I���W�i���̊֐����J�X�^�}�C�Y���Ă���
        if (!ImGui_ImplWin32_Init(hWnd, WidthIn, HeightIn)) // -- 2020.8.7
        {
            MessageBox(0, _T("imgui���������o���܂���"), nullptr, MB_OK);
        }
        if (!ImGui_ImplDX11_Init(pD3D->m_pDevice, pD3D->m_pDeviceContext))
        {
            MessageBox(0, _T("imgui���������o���܂���"), nullptr, MB_OK);
        }

        // ini�t�@�C���𐶐����Ȃ��悤��
        io.IniFilename = nullptr;

        // �t�H���g�̃p�X���擾
        char FontPath[MAX_PATH]; // �p�X�擾�o�b�t�@
        SHGetSpecialFolderPathA(nullptr, FontPath, CSIDL_FONTS, 0);
        strcat_s(FontPath, "\\meiryo.ttc");

        // ���{��t�H���g�ɑΉ�
        //io.Fonts->AddFontFromFileTTF(FontPath, 18.0f, nullptrptr, io.Fonts->GetGlyphRangesJapanese());

        // ���{��t�H���g�ɑΉ�(�������A���p������Default�̂܂ܕ\��)
        ImFontConfig imgui_config;
        imgui_config.MergeMode = true;
        io.Fonts->AddFontDefault();
        io.Fonts->AddFontFromFileTTF(FontPath, 18.0f, &imgui_config, io.Fonts->GetGlyphRangesJapanese());
        io.Fonts->AddFontFromFileTTF("C:/WINDOWS/FONTS/BIZ-UDGOTHICR.TTC",
                             18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        io.ImeWindowHandle = ImmGetDefaultIMEWnd(hWnd); // ���{����͑Ή����H 
        //io.ImeWindowHandle = hWnd;                        // ���{����͑Ή����B���̂Q�H
    }

    //-----------------------------------------------------------------------------
    //
    // MyImgui�̏I������
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
    // MyImgui�̕`��O����
    //
    //-----------------------------------------------------------------------------
    void ImguiNewFrame()
    {
        // �`��O����
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();
    }

    //-----------------------------------------------------------------------------
    //
    // MyImgui�̎��`�揈��
    //
    //-----------------------------------------------------------------------------
    void ImguiRender()
    {
        // MyImgui�̍X�V
        //Update();              // �f���E�B���h�E�̕\��

        // �`��㏈���i���`��j
        ImGui::Render();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    //-----------------------------------------------------------------------------
    //
    // MyImgui�S�̂̍X�V����
    //
    //-----------------------------------------------------------------------------
    void Update()
    {
        ImGuiIO& io = ImGui::GetIO();

        // �f���E�B���h�E1�\������
        // ���̃E�B���h�EImGui::ShowDemoWindow()�́A�V�X�e���ɍŏ�����p�ӂ���Ă���f���E�B���h�E�ł���
        static bool p_open = false;
        if (io.KeysDown[VK_F8] == 1)
        {
            p_open = true;
        }
        if (p_open) ImGui::ShowDemoWindow(&p_open);

        // �f���E�B���h�E2�\������
        // ���̃E�B���h�E�́AMyImgui�ō쐬�����f���E�B���h�E�ł���
        static bool p_open2 = false;
        if (io.KeysDown[VK_F9] == 1)
        {
            p_open2 = true;
        }
        if (p_open2) ShowDemoWindow2(&p_open2);
    }


    //-----------------------------------------------------------------------------
    //
    // �f���p�E�B���h�E(DemoWindow2)�̕\��
    //
    // ����
    //		bool* p_open : �I��(����{�^��)�t���O  false�ŏI��
    //
    //-----------------------------------------------------------------------------
    void ShowDemoWindow2(bool* p_open)
    {
        // �w�i�ƃ^�C�g���o�[�̐F�̐ݒ�
        // �i���jPushStyleColor()��1:1�ŏI������PopStyleColor()���K�v
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.2f, 0.2f, 0.7f)); // �ʏ펞�̔w�i�F
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.5f, 0.1f, 0.5f)); // �ʏ펞�i�t�H�[�J�X���Ȃ��Ƃ��j�̃^�C�g���F
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 0.8f));
        // �A�N�e�B�u���i�t�H�[�J�X���������Ă���Ƃ��j�̃^�C�g���F

        // �E�B���h�E�ʒu�ƃT�C�Y��ݒ肵�܂��B
        //   ImGuiCond_Once �ɂ��A����̂ݐݒ肳��܂��B
        //   ImGuiCond_Always �ŁA��ɐݒ肷�邱�Ƃ��ł��܂��B
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_Once);

        // �e�t���[���̘g��
        //ImGui::GetStyle().FrameRounding = 3.0f;      // �t���[���̃R�[�i�[��
        ImGui::GetStyle().FrameBorderSize = 1.0f; // �g���̑���
        //ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 1);    // �g���̐F
        //ImGui::GetStyle().Colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 1);    // �g���̉e�̐F

        // �E�B���h�E��`�̐ݒ�
        // �EBegin�ŃE�C���h�E�̖��O�ݒ�B�Ȃ��A���O�̓E�C���h�E���ƂɃ��j�[�N�ɂȂ�悤�ɂ��邱��
        // �Ep_open�́A����{�^�����������Ƃ�false���Ԃ�
        // �Eflag�́AImGuiWindowFlags_MenuBar, ImGuiWindowFlags_NoResize �Ȃ�
        if (ImGui::Begin(u8"ImGui �f���E�C���h�E�Q�̃^�C�g���o�[", p_open))
        {
            // �ŏ������傫����ݒ�   �s�v
            //ImGui::SetWindowSize(ImVec2(400, 600), ImGuiCond_::ImGuiCond_FirstUseEver);

            // -----------------------------------------------
            // �e�L�X�g�\���B�܂��ASameLine()�œ���s�ɂQ���\������
            ImGui::Text(u8"�e�L�X�g�R���e���c��\���B");
            ImGui::SameLine();
            ImGui::Text(u8"�܂��A�����s�ɒǉ����邱�Ƃ��ł���");

            // ��؂��
            ImGui::Separator();

            // --------------------------------------------------
            // �`�F�b�N�{�b�N�X
            static bool chkboxflag = false;
            ImGui::Checkbox(u8"�`�F�b�N�{�b�N�X��\������", &chkboxflag);

            ImGui::Separator();

            // �J���[�s�b�J�[ -----------------------------------

            ImGui::PushItemWidth(220); // ���ڂ̕���220�ɂ���B�K��PopItemWidth()�Ƒ΂ɂ���

            // �E�z�C�[���^�C�v�E�����x�o�[��\���BColorEditFlag���w�肵�Ȃ��ƃ{�[�h�^�C�v�B���̂Ƃ��́A�E�N���b�N�Ō`����I���ł���
            static float Colorpick[4] = {0};
            ImGui::ColorPicker4(u8"�J���[�s�b�J�[", Colorpick,
                                ImGuiColorEditFlags_::ImGuiColorEditFlags_PickerHueWheel |
                                ImGuiColorEditFlags_::ImGuiColorEditFlags_AlphaBar);
            //ImGui::ColorPicker4(u8"�J���[�s�b�J�[", Colorpick);

            ImGui::PopItemWidth(); // ���ڂ̕������ɖ߂�

            ImGui::Separator();

            // �e�L�X�g�E�������͂̂��낢�� -------------------------

            // ���p�p�������̓���
            static char str[256] = {""}; // char�^
            //ImGui::InputTextMultiline(u8"���p�p�������̓���", str, 256, ImVec2(200,50));   // �����s����
            ImGui::InputText(u8"���p�p�������̓���", str, 256); // ��s����


            // ���{��e�L�X�g�̓���
            // �ETCHAR�^����UUTF-8(char)�^�ɕϊ����ē��͂���
            // �E�܂�A�@TCHAR�^��UTF-8(char)�^���i���́j��UTF-8(char)�^��TCHAR�^
            static TCHAR tstr[256] = {_T("")}; // TCHAR�^
            char u8str[256]; // UTF-8(char)�^

            // TCHAR�^��UTF-8�^�ɕϊ�
            MyImgui::ConvertTCHARToUTF8(tstr, u8str);

            // ���{��e�L�X�g����
            //ImGui::InputTextMultiline(u8"���{��̓���", u8str, 256, ImVec2(200,50));   // �����s����
            ImGui::InputText(u8"���{��̓���", u8str, 256); // ��s����

            // UTF-8�^��TCHAR�^�ɕϊ�
            MyImgui::ConvertUTF8ToTCHAR(u8str, tstr);


            ImGui::Separator();


            // �����̓���
            static int su1 = 0;
            //ImGui::DragInt(u8"�����̓���", &su1);
            ImGui::InputInt(u8"�����̓���", &su1);


            // 4�����̓���
            static int su2[4] = {0};
            ImGui::SliderInt4(u8"�S���l�̓���", su2, 0, 255);

            ImGui::Separator();


            // �R���{�{�b�N�X�̂��낢�� ------------------------

            // �@ �R���{�{�b�N�X1
            // �z��items�̓��e���P���ڂÂR���{�{�b�N�X�ɐݒ肵�ĕ\������
            // �N���b�N�����ʒu�̕�����|�C���^�[���I���|�C���^�[item_current�ɐݒ肳���
            const char* items[] = {
                "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL",
                "MMMM", "OOOOOOO"
            };
            static const char* item_current = items[0]; // ���ڃ��X�g�̐擪�ʒu�������I���|�C���^�[�Ƃ���.
            if (ImGui::BeginCombo("Combo 1", item_current, 0))
            // 2�Ԗڂ̃p�����[�^�́A�R���{���J���O�ɑI�����ꂽ�|�C���^�[�ł���.
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    bool is_selected = (item_current == items[n]);
                    // ���݂̃��X�g�ʒu���A�I���|�C���^�[�ʒu�ƈ�v���Ă��邩���f����
                    if (ImGui::Selectable(items[n], is_selected))
                        // ���X�g���R���{�{�b�N�X�ɒǉ�����B�I���ʒu�ƈ�v���Ă�����true��Ԃ�
                        item_current = items[n]; // �I���ʒu��I���|�C���^�[�ɐݒ肷��
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                    // �R���{���J���Ƃ��̏����t�H�[�J�X��ݒ肷�� (scrolling + for keyboard navigation support in the upcoming navigation branch)
                }
                ImGui::EndCombo();
            }

            // �A �R���{�{�b�N�X2
            // �\�����������s(single constant string)�Œ��ڐݒ肵�ĕ\������
            // �N���b�N�����ʒu���I���|�C���^�[item_current_2�ɐݒ肳���
            static int item_current_2 = 1; // �����J�[�\���ʒu���P�Ƃ���
            ImGui::Combo("Combo 2", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

            // �B �R���{�{�b�N�X3
            // �\���������z��i��L�A�z��items�j���g�p���ĕ\������
            // �N���b�N�����ʒu���I���|�C���^�[item_current_2�ɐݒ肳���
            static int item_current_3 = -1; // �����J�[�\���ʒu��I�����Ȃ��Ƃ���
            ImGui::Combo("Combo 3", &item_current_3, items, IM_ARRAYSIZE(items));

            ImGui::Separator();


            // ���X�g�{�b�N�X�̂��낢�� ---------------------------

            ImGui::PushItemWidth(100); // ���ڂ̕���100�ɂ���B�K��PopItemWidth()�Ƒ΂ɂ���

            // �@ ���X�g�{�b�N�X�P
            // �z��list_item�̓��e���P���ڂÂ��X�g�{�b�N�X�ɐݒ肵�ĕ\������
            // �N���b�N�����ʒu���I���|�C���^�[item_count�ɐݒ肳���
            const char* list_item[] = {"TTTTT", "UUUUU", "VVVVV", "WWWWW", "XXXXX", "YYYYY", "ZZZZZ"};
            static int item_count = 0; // ���ڂ̑I���|�C���^�[�i�I���ʒu�j

            if (ImGui::ListBoxHeader(u8"ListBox 1", item_count, 3))
            // 2�Ԗڂ̃p�����[�^�́A�I�����ꂽ�|�C���^�[�ł���B3�Ԗڂ̃p�����[�^�́A�{�b�N�X�ɕ\�����鍀�ڐ��ł���B
            {
                for (int n = 0; n < IM_ARRAYSIZE(list_item); n++)
                {
                    bool is_selected = (item_count == n); // ���݂̃��X�g�ʒu���A�I���|�C���^�[�ʒu�ƈ�v���Ă��邩���f����
                    ImGui::PushID(n);
                    if (ImGui::Selectable(list_item[n], is_selected))
                    // ���X�g�����X�g�{�b�N�X�ɒǉ�����B�I���ʒu�ƈ�v���Ă�����true��Ԃ�
                    {
                        item_count = n; // �I���ʒu��I���|�C���^�[�ɐݒ肷��
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus(); // ���X�g���J���Ƃ��̏����t�H�[�J�X��ݒ肷��
                    ImGui::PopID();
                }
                ImGui::ListBoxFooter();
            }

            ImGui::SameLine();

            // �A ���X�g�{�b�N�X�Q
            // �z��list_item2�̓��e���g�p���ă��X�g�{�b�N�X��\������
            // �N���b�N�����ʒu���I���|�C���^�[item_count2�ɐݒ肳���
            const char* list_item2[] = {"MMMMM", "NNNNN", "OOOOO", "PPPPP", "QQQQQ", "RRRRR", "SSSSS"};
            static int item_count2 = 0; // ���ڂ̑I���|�C���^�[�i�I���ʒu�j

            ImGui::ListBox(u8"ListBox 2", &item_count2, list_item2, IM_ARRAYSIZE(list_item2), 3);
            // 5�Ԗڂ̃p�����[�^�́A�{�b�N�X�ɕ\�����鍀�ڐ��ł���B

            ImGui::PopItemWidth(); // ���ڂ̕������ɖ߂�

            // ��؂��
            ImGui::Separator();

            ImGui::BeginChild(u8"�`���C���h�E�B���h�E", ImVec2(260, 50), true);

            ImGui::Text(u8"�m���D�P");
            ImGui::Text(u8"�m���D�Q");
            ImGui::Text(u8"�m���D�R");
            ImGui::Text(u8"�m���D�S");
            ImGui::Text(u8"�m���D�T");
            ImGui::Text(u8"�m���D�U");
            ImGui::Text(u8"�m���D�V");
            ImGui::Text(u8"�m���D�W");

            ImGui::EndChild();
            ImGui::Separator();

            // -------------------------------------------------
            // �{�^������
            ImGui::Text("                                      ");
            ImGui::SameLine();
            if (ImGui::Button(u8"�I��", ImVec2(60, 20)))
            {
                // �{�^�����������Ƃ��A*p_open��false��Ԃ�
                *p_open = false;
            }
        }

        ImGui::End();

        // �F�ݒ�(�w�i�ƃ^�C�g���o�[)�̌�n���B
        // ��LPushStyleColor()��1:1��popStyleColor()���K�v
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }

    //-----------------------------------------------------------------------------
    //
    // UTF-8����TCHAR�ւ̕ϊ��֐�
    //
    // ����
    //   char*  charIn        ���͂���UTF-8������
    //   TCHAR* tcharOut      �ϊ����TCHAR������(Out)
    //
    //-----------------------------------------------------------------------------
    void ConvertUTF8ToTCHAR(char* charIn, TCHAR* tcharOut)
    {
#if _UNICODE
        // unicode�̏ꍇ
        // UTF-8����UTF-16�ւ̕ϊ�
        ConvertU8ToU16(charIn, tcharOut);

#else
        // �}���`�o�C�g�����̏ꍇ
        // �@ UTF-8����UTF-16�ւ̕ϊ�
        WCHAR wstr[512];
        ConvertU8ToU16(charIn, wstr);

        // �A Unicode �����R�[�h(WCHAR)��������Ŏw�肵�������R�[�h�ɕϊ�����( CP_ACP �͓��{��Windows�ł̓V�t�gJIS�R�[�h )
        char mstr[512];
        WideCharToMultiByte(CP_ACP, 0, wstr, -1, mstr, 512, nullptr, nullptr);
        strcpy_s(tcharOut, strlen(mstr) + 1, mstr);

#endif
    }

    //-----------------------------------------------------------------------------
    //
    // TCHAR����UTF-8�ւ̕ϊ��֐�
    //
    // ����
    //   TCHAR* tcharIn       ���͂���TCHAR������
    //   char*  charOut       �ϊ����UTF-8������(Out)
    //
    //-----------------------------------------------------------------------------
    void ConvertTCHARToUTF8(TCHAR* tcharIn, char* charOut)
    {
#if _UNICODE
        // unicode�̏ꍇ
        // UTF-16����UTF-8�ւ̕ϊ�
        ConvertU16ToU8(tcharIn, charOut);

#else
        // �}���`�o�C�g�����̏ꍇ
        // �@ ���͕�����̃}���`�o�C�g(char)�^��WCHAR�^�ɕϊ�
        WCHAR wstr[512] = {L'\0'};
        MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, tcharIn, -1, wstr, 512);
        // �A WCHAR�^��UTF-8�ɕϊ�����
        ConvertU16ToU8(wstr, charOut);

#endif
    }


    //-----------------------------------------------------------------------------
    //
    // UTF-8����UTF-16�ւ̕ϊ��֐�
    //
    // ����
    //   char*  charIn        ���͂���UTF-8������
    //   WCHAR* wcharOut      �ϊ����UTF-16������(Out)
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
    // UTF-16����UTF-8�ւ̕ϊ��֐�
    //
    // ����
    //   WCHAR* wcharIn       ���͂���UTF-16������
    //   char*  charOut       �ϊ����UTF-8������(Out)
    //
    //-----------------------------------------------------------------------------
    void ConvertU16ToU8(WCHAR* wcharIn, char* charOut)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convt;
        std::string ch = convt.to_bytes(wcharIn);

        strcpy_s(charOut, ch.length() + 1, ch.c_str());
    }
}
