#include "includes.h"
#include "Globals.h"
#include "config.h"
#include <chrono>
#include <thread>
#include "memory.h"
#include "font.h"
#include "iconfont.h"
#include <filesystem>
#include <shlobj_core.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

namespace Colors
{
	ImColor black(0, 0, 0);
	ImColor white(255, 255, 255);
	ImColor mainTheme(183, 64, 64);
	ImColor* color = &mainTheme;
	ImColor dynamicText0(255, 255, 255);
	ImColor dynamicText1(255, 255, 255);
	ImColor dynamicText2(255, 255, 255);
	ImColor dynamicText3(255, 255, 255);
	ImColor dynamicText4(255, 255, 255);
	ImColor dynamicAbout(255, 255, 255);
	ImColor hoveredText(88, 127, 255);
}

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
	//io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.Fonts->AddFontFromMemoryTTF(&OpenSans_Regular_ttf, 1, 18.0);
	float baseFontSize = 10.0f;
	float iconFontSize = baseFontSize * 2.0f / 3.0f;
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 10.0f;
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	io.Fonts->AddFontFromMemoryTTF(&icon_font_ttf, sizeof icon_font_ttf, 10.0f, &config, icon_ranges);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (Globals::Gui::showMenu) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled(ICON_FA_QUESTION);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

DWORD WINAPI Rainbow(LPVOID lpReserved)
{
	using namespace std::chrono;

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::duration<float>(Globals::Gui::RainbowDelay));
		*Colors::color = ImColor(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
		if (!Globals::Gui::RainbowThread)
		{
			Globals::Gui::RainbowThread = true;
			ExitThread(0);
			break;
		}
	}
	return TRUE;
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return Globals::Gui::oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetMouseCursor();
	ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	ImGui::GetIO().WantCaptureMouse = Globals::Gui::showMenu;
	ImGui::GetIO().MouseDrawCursor = Globals::Gui::showMenu;

	if (GetAsyncKeyState(Globals::Gui::OpenKey) & 1)
	{
		Globals::Gui::showMenu = !Globals::Gui::showMenu;
	}

	if (Globals::Gui::showMenu)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowTitleAlign = ImVec2(0.5, 0.5);
		style->Colors[ImGuiCol_Border] = *Colors::color;
		style->Colors[ImGuiCol_TitleBg] = *Colors::color;
		style->Colors[ImGuiCol_TitleBgActive] = *Colors::color;
		style->Colors[ImGuiCol_Button] = ImColor(24, 24, 24);
		style->Colors[ImGuiCol_ButtonHovered] = ImColor(45, 45, 45);
		style->Colors[ImGuiCol_FrameBg] = ImColor(36, 36, 36);
		style->Colors[ImGuiCol_FrameBgActive] = ImColor(36, 36, 36);
		style->Colors[ImGuiCol_FrameBgHovered] = ImColor(45, 45, 45);
		style->Colors[ImGuiCol_Separator] = *Colors::color;
		style->Colors[ImGuiCol_SeparatorActive] = *Colors::color;
		style->Colors[ImGuiCol_SeparatorHovered] = *Colors::color;
		style->ItemSpacing = ImVec2(NULL, 8);
		ImGui::SetNextWindowSize(ImVec2(320, 550));
		style->WindowBorderSize = 3.0f;
		//style->WindowRounding = 0.0f;
		style->Colors[ImGuiCol_Text] = Colors::black;
		ImGui::Begin("SOFT Mod Menu " ICON_FA_TREE, &Globals::Gui::isOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
		style->Colors[ImGuiCol_Text] = Colors::white;

		ImGui::BeginMenuBar();

		style->Colors[ImGuiCol_Text] = Colors::dynamicAbout;
		ImGui::Text("About");
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			Colors::dynamicAbout = Colors::hoveredText;
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				Globals::Gui::showAbout = !Globals::Gui::showAbout;
			}
		}
		else {
			Colors::dynamicAbout = Colors::white;
		}
		style->Colors[ImGuiCol_Text] = Colors::white;

		ImGui::EndMenuBar();

		// Home window
		if (Globals::Gui::window == "home")
		{
			// Health
			if (ImGui::Checkbox(ICON_FA_HEART " God Mode", &Globals::Gui::isGodMode))
			{
				if (Globals::Gui::isGodMode) {
					printf("God Mode ON\n");
					Config::bHealth = true;
					Config::bFallDamage = true;
				}
				else {
					printf("God Mode OFF\n");
					Config::bHealth = false;
				}
			}
			// Stamina
			if (ImGui::Checkbox(ICON_FA_HAND_FIST " Infinite Stamina", &Globals::Gui::isStamina))
			{
				if (Globals::Gui::isStamina) {
					printf("Infinite Stamina ON\n");
					Config::bStamina = true;
				}
				else {
					printf("Infinite Stamina OFF\n");
					Config::bStamina = false;
				}
			}
			// Fullness
			if (ImGui::Checkbox(ICON_FA_BOWL_FOOD " No Hungry", &Globals::Gui::isFullness))
			{
				if (Globals::Gui::isFullness) {
					printf("Fullness ON\n");
					Config::bFullness = true;
				}
				else {
					printf("Fullness OFF\n");
					Config::bFullness = false;
				}
			}
			// Hydration
			if (ImGui::Checkbox(ICON_FA_DROPLET " No Dehydration", &Globals::Gui::isHydration))
			{
				if (Globals::Gui::isHydration) {
					printf("No Dehydration ON\n");
					Config::bHydration = true;
				}
				else {
					printf("No Dehydration OFF\n");
					Config::bHydration = false;
				}
			}
			// Rest
			if (ImGui::Checkbox(ICON_FA_MOON " No Sleep", &Globals::Gui::isRest))
			{
				if (Globals::Gui::isRest) {
					printf("No Sleep ON\n");
					Config::bRest = true;
				}
				else {
					printf("No Sleep OFF\n");
					Config::bRest = false;
				}
			}
			// Speed
			ImGui::Checkbox(ICON_FA_PERSON_WALKING " Custom Movement/Swim Speed", &Config::bSpeed);
			if (Config::bSpeed)
			{
				ImGui::SliderFloat("Run Speed", &Config::Value::runSpeed, 5.4f, 50.0f, "%.1f");
				ImGui::SameLine(ImGui::GetContentRegionAvail().x), HelpMarker("Default is 5.4");
				ImGui::SliderFloat("Swim Speed", &Config::Value::swimSpeedMultiplier, 1.0f, 25.0f, "%.1f");
				ImGui::SameLine(ImGui::GetContentRegionAvail().x), HelpMarker("Don't know what the default value is");
			}
			// Super Jump
			ImGui::Checkbox(ICON_FA_UP_LONG " Super Jump", &Config::bJump);
			if (Config::bJump)
			{
				ImGui::SliderFloat("Jump height", &Config::Value::jumpHeight, 2.6f, 150.0f, "%.1f");
				ImGui::SameLine(ImGui::GetContentRegionAvail().x), HelpMarker("Default is 2.6");
				Config::bFallDamage = true;
			}
			// Fall Damage
			ImGui::Checkbox(ICON_FA_PERSON_FALLING " No Fall Damage", &Config::bFallDamage);
			// Disable Gravity
			ImGui::Checkbox(ICON_FA_SHUTTLE_SPACE " No Gravity", &Config::bGravity);

			ImGui::Separator();
			/*
			style->Colors[ImGuiCol_Text] = Colors::dynamicText1;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " SP Only " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::dynamicText1 = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					printf("sp clicked\n");
				}
			}
			else {
				Colors::dynamicText1 = Colors::white;
			}

			style->Colors[ImGuiCol_Text] = Colors::dynamicText2;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " MP Only " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::dynamicText2 = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					printf("mp clicked\n");
				}
			}
			else {
				Colors::dynamicText2 = Colors::white;
			}
			*/
			style->Colors[ImGuiCol_Text] = Colors::dynamicText0;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Teleport " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::dynamicText0 = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "teleport";
				}
			}
			else {
				Colors::dynamicText0 = Colors::white;
			}

			style->Colors[ImGuiCol_Text] = Colors::dynamicText3;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Menu Theme " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::dynamicText3 = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "theme";
				}
			}
			else {
				Colors::dynamicText3 = Colors::white;
			}

			style->Colors[ImGuiCol_Text] = Colors::dynamicText4;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Debug " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::dynamicText4 = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "debug";
				}
			}
			else {
				Colors::dynamicText4 = Colors::white;
			}
			style->Colors[ImGuiCol_Text] = Colors::white;
		}

		// Teleport window
		if (Globals::Gui::window == "teleport")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "home";
				}
			}

			ImGui::Text("My Base (Lake)");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1000, 120, -57));
				}
			}

			ImGui::Text("Shotgun Grave Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1340, 102, 1412));
				}
			}

			ImGui::Text("Pistol Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1797, 16, 578));
				}
			}

			ImGui::Text("Hang Glider 1st Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1307, 87, 1732));
				}
			}

			ImGui::Text("Knight V 1st Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1026, 231, -625));
				}
			}

			ImGui::Text("Top Of The Mountain");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(4, 716, -459));
				}
			}

			ImGui::Text("Rebreather + Stun Gun Cave Entrance");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-418, 19, 1532));
				}
			}

			ImGui::Text("Flashlight Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-630, 142, 391));
				}
			}

			ImGui::Text("Modern Axe Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-704, 108, 450));
				}
			}

			ImGui::Text("Machete Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-65, 20, 1458));
				}
			}

			ImGui::Text("Stun Baton Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1142, 134, -157));
				}
			}

			ImGui::Text("Putter Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(1024, 145, 1212));
				}
			}

			ImGui::Text("Binocular Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1109, 20, 1721));
				}
			}

			ImGui::Text("Revolver Loc. (Bunker Entrance?)");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(1111, 132, 1003));
				}
			}

			ImGui::Text("Cross + Rope Gun Cave Entrance");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1113, 132, -171));
				}
			}

			ImGui::Text("Slingshot + Shovel Cave Entrance");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-531, 200, 124));
				}
			}

			ImGui::Text("Compound Bow Bunker Entrance");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1136, 284, -1095));
				}
			}

			ImGui::Text("3D Printer + Guest KeyCard + Guitar + Chainsaw Bunker Entrance");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1188, 70, 133));
				}
			}

			ImGui::Text("FireAxe + Maintenance Card Bunker Entrance");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-474, 90, 710));
				}
			}

			ImGui::Text("VIP Keycard + Crossbow Bunker Entrance");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(-1014, 102, 1024));
				}
			}
			/*
			ImGui::Text("Golden Armor Loc.");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(180, 145, -667));
				}
			}
			*/
			ImGui::Text("End Game Bunker Entrance");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(1756, 45, 553));
				}
			}
		}

		// Theme window
		if (Globals::Gui::window == "theme")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "home";
				}
			}

			if (!Globals::Gui::RainbowTheme)
			{
				ImGui::Checkbox(ICON_FA_BRUSH " Custom Theme", &Globals::Gui::CustomTheme);
				if (Globals::Gui::CustomTheme) {
					ImGui::ColorEdit3("##", (float*)&Colors::color->Value.x);
				}
				else if (!Globals::Gui::CustomTheme) {
					*Colors::color = ImColor(183, 64, 64);
				}
				ImGui::SameLine(ImGui::GetContentRegionAvail().x), HelpMarker("Click the right colored Box to open the Color Picker");
			}

			if (!Globals::Gui::CustomTheme)
			{
				if (ImGui::Checkbox("Random Theme", &Globals::Gui::RainbowTheme))
				{
					if (Globals::Gui::RainbowTheme) {
						CreateThread(nullptr, 0, Rainbow, nullptr, 0, nullptr);
					}
					else {
						Globals::Gui::RainbowThread = false;
					}
				}
				ImGui::SameLine(ImGui::GetContentRegionAvail().x), HelpMarker("Experimental, if you encounter crashes or strange behaviours don't enable this");

				if (Globals::Gui::RainbowTheme) {
					ImGui::SliderFloat("Seconds Delay", &Globals::Gui::RainbowDelay, 0.1f, 10.0f, "%.1f");
				}
			}
			ImGui::ColorButton("Current Theme", *Colors::color);
		}

		// Debug window
		if (Globals::Gui::window == "debug")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "home";
				}
			}
			ImGui::Text("Player Coords");
			Unity::Vector3 playerLoc = Globals::LocalPlayer->GetTransform()->GetPosition();
			ImGui::InputFloat("X", &playerLoc.x, NULL, NULL, 0, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat("Y", &playerLoc.y, NULL, NULL, 0, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat("Z", &playerLoc.z, NULL, NULL, 0, ImGuiInputTextFlags_ReadOnly);
		}

		//About window
		if (Globals::Gui::showAbout)
		{
			ImGui::SetCursorPos(ImVec2(5, 235));
			style->Colors[ImGuiCol_ChildBg] = ImColor(36, 36, 36);
			style->ChildBorderSize = 2.0f;
			ImGui::BeginChild("about", ImVec2(310, 80), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::SetCursorPos(ImVec2(95, 30));
			ImGui::Text("Made by ImAxel0");
			ImGui::EndChild();
		}
		ImGui::End();

		if (!Globals::Gui::isOpen)
		{
			Globals::Gui::showMenu = false;
			Globals::Gui::isOpen = true;
		}
	}

	setMemory();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return Globals::Gui::oPresent(pSwapChain, SyncInterval, Flags);
}
