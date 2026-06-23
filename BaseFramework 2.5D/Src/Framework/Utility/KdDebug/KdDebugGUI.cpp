#include "../../../Application/main.h"

#include "KdDebugGUI.h"

//仮置き
#include "../../../Application/Camera/CameraManager.h"

KdDebugGUI::KdDebugGUI()
{}
KdDebugGUI::~KdDebugGUI()
{ 
	GuiRelease(); 
}

void KdDebugGUI::GuiInit(int w, int h)
{
	// 初期化済みなら動作させない
	if (m_uqLog) return;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style
	// ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();
	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(Application::Instance().GetWindowHandle(), ImVec2(w,h));
	ImGui_ImplDX11_Init(KdDirect3D::Instance().WorkDev(), KdDirect3D::Instance().WorkDevContext());

#include "imgui/ja_glyph_ranges.h"
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	// 日本語対応
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);
	m_uqLog = std::make_unique<ImGuiAppLog>();
}

void KdDebugGUI::GuiProcess()
{
	// 初期化されてないなら動作させない
	if (!m_uqLog) return;

	//===========================================================
	// ImGui開始
	//===========================================================
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//===========================================================
	// 以下にImGui描画処理を記述
	//===========================================================

	// デバッグウィンドウ(日本語を表示したい場合はこう書く)
	if (ImGui::Begin(U8("えふぴぃえす")))
	{
		// FPS
		ImGui::Text("FPS : %d", Application::Instance().GetNowFPS());

		//カメラ
		if (ImGui::Button("Save", ImVec2(40.0f, 20.0f)))CAMERAMGR.Save();
		ImGui::SliderFloat("CameraPosX", &CAMERAMGR.GetCameraData().m_cameraPos.x, -5.0f, 5.0f, "%.2f");
		ImGui::SliderFloat("CameraPosY", &CAMERAMGR.GetCameraData().m_cameraPos.y, -1.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("CameraPosZ", &CAMERAMGR.GetCameraData().m_cameraPos.z, -18.0f, -8.0f, "%.2f");
		ImGui::SliderFloat("CameraDegX", &CAMERAMGR.GetCameraData().m_cameraDeg.x, 0.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("CameraDegY", &CAMERAMGR.GetCameraData().m_cameraDeg.y, -40.0f, 40.0f, "%.1f");
 	}	
	ImGui::End();

	// ログウィンドウ
	//m_uqLog->Draw("Log Window");
	//KdDebugGUI::Instance().ClearLog();

	//=====================================================
	// ログ出力 ・・・ AddLog("～") で追加
	//=====================================================

//	m_uqLog->AddLog("hello world\n");

	//=====================================================
	// 別ソースファイルからログを出力する場合
	//=====================================================

//	KdDebugGUI::Instance().AddLog("TestLog\n");

	//===========================================================
	// ここより上にImGuiの描画はする事
	//===========================================================
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void KdDebugGUI::AddLog(const char* fmt,...)
{
	// 初期化されてないなら動作させない
	if (!m_uqLog) return;

	char tmpStr[128] = {};
	va_list args;
	va_start(args, fmt);
	vsprintf_s(tmpStr, fmt, args);
	m_uqLog->AddLog(tmpStr);
	va_end(args);
}

void KdDebugGUI::ClearLog()
{
	// 初期化されてないなら動作させない
	if (!m_uqLog) return;

	m_uqLog->Clear();
}

void KdDebugGUI::GuiRelease()
{
	// 初期化されてないなら動作させない
	if (!m_uqLog) return;

	m_uqLog = nullptr;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
