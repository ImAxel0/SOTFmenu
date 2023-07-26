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
#include "hooks.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "imgui/imgui_stdlib.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

int teleport_image_width = 0;
int teleport_image_height = 0;
ID3D11ShaderResourceView* lake_texture = NULL;
ID3D11ShaderResourceView* shotgun_texture = NULL;
ID3D11ShaderResourceView* pistol_texture = NULL;
ID3D11ShaderResourceView* hang_glider_texture = NULL;
ID3D11ShaderResourceView* knightV_texture = NULL;
ID3D11ShaderResourceView* top_of_mountain_texture = NULL;
ID3D11ShaderResourceView* rebreather_stungun_texture = NULL;
ID3D11ShaderResourceView* flashlight_texture = NULL;
ID3D11ShaderResourceView* modern_axe_texture = NULL;
ID3D11ShaderResourceView* machete_texture = NULL;
ID3D11ShaderResourceView* stun_baton_texture = NULL;
ID3D11ShaderResourceView* putter_texture = NULL;
ID3D11ShaderResourceView* binocular_texture = NULL;
ID3D11ShaderResourceView* revolver_texture = NULL;
ID3D11ShaderResourceView* rope_gun_texture = NULL;
ID3D11ShaderResourceView* shovel_texture = NULL;
ID3D11ShaderResourceView* moder_bow_texture = NULL;
ID3D11ShaderResourceView* chainsaw_texture = NULL;
ID3D11ShaderResourceView* fire_axe_texture = NULL;
ID3D11ShaderResourceView* crossbow_texture = NULL;
ID3D11ShaderResourceView* endgame_texture = NULL;

ID3D11ShaderResourceView* map_texture = NULL;

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
	ImColor itemsText(255, 255, 255);
	ImColor environmentText(255, 255, 255);
	ImColor addItemsText(255, 255, 255);
	ImColor spawnCharacters(255, 255, 255);
	ImColor multiplayerText(255, 255, 255);
	ImColor hoveredText(88, 127, 255);
}

bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
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

	// Getting Documents/SOTF/images location
	std::filesystem::path path;
	PWSTR path_tmp;
	auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path_tmp);
	// Error check
	if (get_folder_path_ret != S_OK) {
		CoTaskMemFree(path_tmp);
	}
	path = path_tmp;
	CoTaskMemFree(path_tmp);
	path += "\\SOTF\\images\\";
	
	// Getting single images paths
	std::string path_str = path.string();
	std::string lake_path = path_str.append("lake.png");
	path_str = path.string();
	std::string shotgun_path = path_str.append("shotgun.png");
	path_str = path.string();
	std::string pistol_path = path_str.append("pistol.png");
	path_str = path.string();
	std::string hang_glider_path = path_str.append("hang_glider.png");
	path_str = path.string();
	std::string knightV_path = path_str.append("knightV.png");
	path_str = path.string();
	std::string top_of_mountain_path = path_str.append("top_of_mountain.png");
	path_str = path.string();
	std::string rebreather_stungun_path = path_str.append("rebreather_stungun.png");
	path_str = path.string();
	std::string flashlight_path = path_str.append("flashlight.png");
	path_str = path.string();
	std::string modern_axe_path = path_str.append("modern_axe.png");
	path_str = path.string();
	std::string machete_path = path_str.append("machete.png");
	path_str = path.string();
	std::string stun_baton_path = path_str.append("stun_baton.png");
	path_str = path.string();
	std::string putter_path = path_str.append("putter.png");
	path_str = path.string();
	std::string binocular_path = path_str.append("binocular.png");
	path_str = path.string();
	std::string revolver_path = path_str.append("revolver.png");
	path_str = path.string();
	std::string rope_gun_path = path_str.append("rope_gun.png");
	path_str = path.string();
	std::string shovel_path = path_str.append("shovel.png");
	path_str = path.string();
	std::string modern_bow_path = path_str.append("modern_bow.png");
	path_str = path.string();
	std::string chainsaw_path = path_str.append("chainsaw.png");
	path_str = path.string();
	std::string fire_axe_path = path_str.append("fire_axe.png");
	path_str = path.string();
	std::string crossbow_path = path_str.append("crossbow.png");
	path_str = path.string();
	std::string endgame_path = path_str.append("endgame.png");

	path_str = path.string();
	std::string map_path = path_str.append("map\\map.png");

	bool ret1 = LoadTextureFromFile(lake_path.c_str(), &lake_texture, &teleport_image_width, &teleport_image_height);
	bool ret2 = LoadTextureFromFile(shotgun_path.c_str(), &shotgun_texture, &teleport_image_width, &teleport_image_height);
	bool ret3 = LoadTextureFromFile(pistol_path.c_str(), &pistol_texture, &teleport_image_width, &teleport_image_height);
	bool ret4 = LoadTextureFromFile(hang_glider_path.c_str(), &hang_glider_texture, &teleport_image_width, &teleport_image_height);
	bool ret5 = LoadTextureFromFile(knightV_path.c_str(), &knightV_texture, &teleport_image_width, &teleport_image_height);
	bool ret6 = LoadTextureFromFile(top_of_mountain_path.c_str(), &top_of_mountain_texture, &teleport_image_width, &teleport_image_height);
	bool ret7 = LoadTextureFromFile(rebreather_stungun_path.c_str(), &rebreather_stungun_texture, &teleport_image_width, &teleport_image_height);
	bool ret8 = LoadTextureFromFile(flashlight_path.c_str(), &flashlight_texture, &teleport_image_width, &teleport_image_height);
	bool ret9 = LoadTextureFromFile(modern_axe_path.c_str(), &modern_axe_texture, &teleport_image_width, &teleport_image_height);
	bool ret10 = LoadTextureFromFile(machete_path.c_str(), &machete_texture, &teleport_image_width, &teleport_image_height);
	bool ret21 = LoadTextureFromFile(stun_baton_path.c_str(), &stun_baton_texture, &teleport_image_width, &teleport_image_height);
	bool ret11 = LoadTextureFromFile(putter_path.c_str(), &putter_texture, &teleport_image_width, &teleport_image_height);
	bool ret12 = LoadTextureFromFile(binocular_path.c_str(), &binocular_texture, &teleport_image_width, &teleport_image_height);
	bool ret13 = LoadTextureFromFile(revolver_path.c_str(), &revolver_texture, &teleport_image_width, &teleport_image_height);
	bool ret14 = LoadTextureFromFile(rope_gun_path.c_str(), &rope_gun_texture, &teleport_image_width, &teleport_image_height);
	bool ret15 = LoadTextureFromFile(shovel_path.c_str(), &shovel_texture, &teleport_image_width, &teleport_image_height);
	bool ret16 = LoadTextureFromFile(modern_bow_path.c_str(), &moder_bow_texture, &teleport_image_width, &teleport_image_height);
	bool ret17 = LoadTextureFromFile(chainsaw_path.c_str(), &chainsaw_texture, &teleport_image_width, &teleport_image_height);
	bool ret18 = LoadTextureFromFile(fire_axe_path.c_str(), &fire_axe_texture, &teleport_image_width, &teleport_image_height);
	bool ret19 = LoadTextureFromFile(crossbow_path.c_str(), &crossbow_texture, &teleport_image_width, &teleport_image_height);
	bool ret20 = LoadTextureFromFile(endgame_path.c_str(), &endgame_texture, &teleport_image_width, &teleport_image_height);

	bool map = LoadTextureFromFile(map_path.c_str(), &map_texture, &teleport_image_width, &teleport_image_height);

	printf("[+] Images loaded\n");

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

void DisplayTeleportImage(ImTextureID texture)
{
	auto pos = ImGui::GetWindowPos();
	ImGui::SetNextWindowSize(ImVec2(720, 550), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(pos.x + 322, pos.y), ImGuiCond_Always);
	if (ImGui::Begin("Preview", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::Image((void*)texture, ImVec2(720, 550));
		ImGui::End();
	}
}

DWORD WINAPI SetOnGround(LPVOID lpReserved)
{
	using namespace std::chrono;

	std::this_thread::sleep_for(2s);

	int x = Globals::LocalPlayer->GetTransform()->GetPosition().x;
	int z = Globals::LocalPlayer->GetTransform()->GetPosition().z;
	float y = 750;

	while (!Globals::FirstPersonCharacter->GetMemberValue<bool>("_isGrounded"))
	{
		Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(x, y, z));
		if (Globals::FirstPersonCharacter->GetMemberValue<bool>("_isGrounded")) {
			break;
		}
		y -= 0.1f;
	}
	ExitThread(0);
}

void DisplayMap(ImTextureID texture)
{
	ImGui::SetNextWindowSize(ImVec2(723, 723), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	if (ImGui::Begin("##", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		//std::cout << "X: " << ImGui::GetMousePos().x << '\n';
		//std::cout << "Y: " << ImGui::GetMousePos().y << '\n';

		// In game x increases going right
		// In game z increases going up

		int x = (ImGui::GetMousePos().x - 363) * 5.5f; // if mouse cursor is at the center of the map than ImGui::GetMousePos().x = 363
		int z = (ImGui::GetMousePos().y - 363) * 5.5f; // if mouse cursor is at the center of the map than ImGui::GetMousePos().y = 363

		ImGui::Image((void*)texture, ImVec2(723, 723));
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			Globals::LocalPlayer->GetTransform()->SetPosition(Unity::Vector3(x, 750, -z));
			//CreateThread(nullptr, 0, SetOnGround, nullptr, 0, nullptr);
			Config::bFallDamage = true;
		}
		ImGui::SetCursorPos(ImVec2((Globals::LocalPlayer->GetTransform()->GetPosition().x / 5.5f) + 358, -(Globals::LocalPlayer->GetTransform()->GetPosition().z / 5.5f) + 353));
		ImGui::Text(ICON_FA_CIRCLE_DOT);
		ImGui::SetCursorPos(ImVec2(9, 9));
		ImGui::Text(std::to_string(Globals::LocalPlayer->GetTransform()->GetPosition().x).c_str());
		ImGui::Text(std::to_string(Globals::LocalPlayer->GetTransform()->GetPosition().y).c_str());
		ImGui::Text(std::to_string(Globals::LocalPlayer->GetTransform()->GetPosition().z).c_str());
		ImGui::End();
	}
}

void SetWindow(std::string window_string)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			Globals::Gui::window = window_string;
		}
	}
}

void TeleportTo(Unity::Vector3 xyz, ID3D11ShaderResourceView* image)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			Globals::LocalPlayer->GetTransform()->SetPosition(xyz);
		}
		DisplayTeleportImage(image);
	}
}

void AddItem(int ItemID, int ItemQTY)
{
	Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::mainTheme;
	ImGui::PushID(ItemID);
	if (ImGui::SmallButton("Add " ICON_FA_ARROW_UP))
	{
		Globals::ItemInstanceManager->CallMethod<bool, int, int>(Globals::Methods::TryAddItems, ItemID, ItemQTY);
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	}
	ImGui::PopID();
	Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::white;
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

			IL2CPP::Thread::Attach(IL2CPP::Domain::Get());

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

	// May cause performance issues
	Config::Value::Pistol::IsPistolEquipped = findPistol();
	Config::Value::Revolver::IsRevolverEquipped = findRevolver();
	Config::Value::Shotgun::IsShotgunEquipped = findShotgun();
	Config::Value::FlashLight::IsFlashLightEquipped = findFlashLight();
	Config::Value::Lighter::IsLighterEquipped = findLighter();
	Config::Value::Rebreather::IsRebreatherEquipped = findRebreather();
	Config::Value::RopeGun::IsRopeGunEquipped = findRopeGun();

	if (Globals::Gui::showMenu)
	{
		Globals::Gui::style = &ImGui::GetStyle();
		Globals::Gui::style->WindowTitleAlign = ImVec2(0.5, 0.5);
		Globals::Gui::style->Colors[ImGuiCol_Border] = *Colors::color;
		Globals::Gui::style->Colors[ImGuiCol_TitleBg] = *Colors::color;
		Globals::Gui::style->Colors[ImGuiCol_TitleBgActive] = *Colors::color;
		Globals::Gui::style->Colors[ImGuiCol_Button] = ImColor(24, 24, 24);
		Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = ImColor(45, 45, 45);
		Globals::Gui::style->Colors[ImGuiCol_FrameBg] = ImColor(36, 36, 36);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = ImColor(36, 36, 36);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = ImColor(45, 45, 45);
		Globals::Gui::style->Colors[ImGuiCol_Separator] = *Colors::color;
		Globals::Gui::style->Colors[ImGuiCol_SeparatorActive] = *Colors::color;
		Globals::Gui::style->Colors[ImGuiCol_SeparatorHovered] = *Colors::color;
		Globals::Gui::style->ItemSpacing = ImVec2(NULL, 8);
		ImGui::SetNextWindowSize(ImVec2(320, 670));
		Globals::Gui::style->WindowBorderSize = 3.0f;
		//Globals::Gui::style->WindowRounding = 0.0f;
		Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::black;

		ImGui::Begin("SOTF Mod Menu " ICON_FA_TREE, &Globals::Gui::isOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
		Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::white;

		ImGui::BeginMenuBar();

		Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::dynamicAbout;
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
		Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::white;

		ImGui::EndMenuBar();

		// Home window
		if (Globals::Gui::window == "home")
		{	
			// Health
			ImGui::Checkbox(ICON_FA_HEART " God Mode", &Config::bHealth);
			// Stamina
			ImGui::Checkbox(ICON_FA_HAND_FIST " Infinite Stamina", &Config::bStamina);
		
			// Fullness
			if (ImGui::Checkbox(ICON_FA_BOWL_FOOD " No Hungry", &Globals::Gui::isFullness))
			{
				if (Globals::Gui::isFullness) {
					Config::bFullness = true;
				}
				else {
					Config::bFullness = false;
				}
			}
			// Hydration
			if (ImGui::Checkbox(ICON_FA_DROPLET " No Dehydration", &Globals::Gui::isHydration))
			{
				if (Globals::Gui::isHydration) {
					Config::bHydration = true;
				}
				else {
					Config::bHydration = false;
				}
			}
			// Rest
			if (ImGui::Checkbox(ICON_FA_MOON " No Sleep", &Globals::Gui::isRest))
			{
				if (Globals::Gui::isRest) {
					Config::bRest = true;
				}
				else {
					Config::bRest = false;
				}
			}		
			// Infinite Ammo
			ImGui::Checkbox(ICON_FA_GUN " Infinite Ammo (Beta)", &Config::bInfiniteAmmo);
			ImGui::SameLine(ImGui::GetContentRegionAvail().x), HelpMarker("Doesn't work with all weapons. Tested working on pistol and shotgun + stun gun but this one remain in a buggy state after every shot but still continue to shoot");
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
			// Instant build
			ImGui::Checkbox(ICON_FA_BUILDING " Instant Build mode (HOST only)", &Config::bInstantBuild);
 
			ImGui::Separator();
			
			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::dynamicText1;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Single Player Only " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::dynamicText1 = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "single";
				}
			}
			else {
				Colors::dynamicText1 = Colors::white;
			}
			
			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::multiplayerText;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Multi Player Only " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::multiplayerText = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "multiplayer";
				}
			}
			else {
				Colors::multiplayerText = Colors::white;
			}
			
			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::addItemsText;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Add Item to Inventory " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::addItemsText = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "additems";
				}
			}
			else {
				Colors::addItemsText = Colors::white;
			}

			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::spawnCharacters;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Spawn Characters (HOST only) " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::spawnCharacters = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "spawncharacters";
				}
			}
			else {
				Colors::spawnCharacters = Colors::white;
			}

			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::itemsText;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Modded Items " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::itemsText = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "items";
				}
			}
			else {
				Colors::itemsText = Colors::white;
			}

			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::environmentText;
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Environment " ICON_FA_ARROW_LEFT_LONG);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				Colors::environmentText = Colors::hoveredText;
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					Globals::Gui::window = "environment";
				}
			}
			else {
				Colors::environmentText = Colors::white;
			}
			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::white;

			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::dynamicText0;
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

			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::dynamicText3;
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

			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::dynamicText4;
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
			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::white;
		}

		// Single Player window
		if (Globals::Gui::window == "single")
		{
			// Create SinglePlayer Hooks
			if (!Globals::initSingleHooks)
			{
				CreateSinglePlayerHacksHooks();
				Globals::initSingleHooks = true;
			}

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("home");

			// GodMode SP
			if (ImGui::Checkbox(ICON_FA_HEART " God Mode SP", &Config::bHealthSP))
			{
				GodMode();
			}
			// Stamina SP
			if (ImGui::Checkbox(ICON_FA_HAND_FIST " Infinite Stamina SP", &Config::bStaminaSP))
			{
				InfiniteStamina();
			}
		}

		// Mutli-Player window
		if (Globals::Gui::window == "multiplayer")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Lobby Settings (HOST only) " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("lobby");
		}

		//Lobby Settings window
		if (Globals::Gui::window == "lobby")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("multiplayer");

			if (!Config::Lobby::gotLobbyInfo) {
				GetLobbyInfo();
				Config::Lobby::gotLobbyInfo = true;
			}

			ImGui::InputInt("Player Limit", &Config::Lobby::PlayerLimit, 1, NULL);

			if (ImGui::SmallButton("Set"))
			{
				Globals::CoopLobby->CallMethod<void>("SetMemberLimit", Config::Lobby::PlayerLimit);
			}
		}

		// Add Items window
		if (Globals::Gui::window == "additems")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("home");

			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::mainTheme;
			ImGui::Text("There is no inventory limit but the item number\nwill reset to the max capacity after closing the\ngame.");
			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::white;

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " General Items " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("general");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Armor " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("armor");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Attachments " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("attachments");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Food & Drink " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("food&drink");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Keycards " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("keycards");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Materials " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("materials");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Seeds & Plants " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("seeds&plants");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Throwables " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("throwables");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Tools " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("tools");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Weapons & Ammo " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("weapons&ammo");
		}

		// General Items window
		if (Globals::Gui::window == "general")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Album Cover");
			AddItem(Config::Items::AlbumCover, 1);

			ImGui::Text("Arm");
			AddItem(Config::Items::Arm, 1);

			ImGui::Text("Blazer");
			AddItem(Config::Items::Blazer, 1);

			ImGui::Text("Blue T-Shirt");
			AddItem(Config::Items::BlueTshirt, 1);

			ImGui::Text("Camouflage Suit");
			AddItem(Config::Items::CamouflageSuit, 1);

			ImGui::Text("Cash (x50)");
			AddItem(Config::Items::Cash, 50);

			ImGui::Text("Coins (x50)");
			AddItem(Config::Items::Coins, 50);

			ImGui::Text("DeepSleep Book");
			AddItem(Config::Items::DeepSleepBook, 1);

			ImGui::Text("Energy Mix");
			AddItem(Config::Items::EnergyMix, 1);

			ImGui::Text("Energy Mix +");
			AddItem(Config::Items::EnergyMixPlus, 1);

			ImGui::Text("Feather (x2)");
			AddItem(Config::Items::Feather, 2);

			ImGui::Text("Flippers");
			AddItem(Config::Items::Flippers, 1);

			ImGui::Text("Golf Ball");
			AddItem(Config::Items::GolfBall, 1);

			ImGui::Text("GPS Locator");
			AddItem(Config::Items::GpsLocator, 1);

			ImGui::Text("Head");
			AddItem(Config::Items::Head, 1);

			ImGui::Text("Healt Mix");
			AddItem(Config::Items::HealthMix, 1);

			ImGui::Text("Healt Mix +");
			AddItem(Config::Items::HealthMixPlus, 1);

			ImGui::Text("Hoodie");
			AddItem(Config::Items::Hoodie, 1);

			ImGui::Text("Leather Jacket");
			AddItem(Config::Items::LeatherJacket, 1);

			ImGui::Text("Leg");
			AddItem(Config::Items::Leg, 1);

			ImGui::Text("Lightbulb");
			AddItem(Config::Items::Lightbulb, 1);

			ImGui::Text("Meds");
			AddItem(Config::Items::Meds, 1);

			ImGui::Text("Pajamas");
			AddItem(Config::Items::Pajamas, 1);

			ImGui::Text("Paper Target");
			AddItem(Config::Items::PaperTarget, 1);

			ImGui::Text("Pennant Line book");
			AddItem(Config::Items::PennantLineBook, 1);

			ImGui::Text("Radio");
			AddItem(Config::Items::Radio, 1);

			ImGui::Text("Skin Pouch");
			AddItem(Config::Items::SkinPouch, 1);

			ImGui::Text("Solar Panel");
			AddItem(Config::Items::SolarPanel, 1);

			ImGui::Text("Swimsuit");
			AddItem(Config::Items::Swimsuit, 1);

			ImGui::Text("Tarp");
			AddItem(Config::Items::Tarp, 1);

			ImGui::Text("Turtle Egg");
			AddItem(Config::Items::TurtleEgg, 1);

			ImGui::Text("Wetsuit");
			AddItem(Config::Items::Wetsuit, 1);

			ImGui::Text("Winter Jacket");
			AddItem(Config::Items::WinterJacket, 1);
		}

		// Weapons & Ammo window
		if (Globals::Gui::window == "weapons&ammo")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Compound Bow");
			AddItem(Config::Items::CompoundBow, 1);

			ImGui::Text("Crafted Club");
			AddItem(Config::Items::CraftedClub, 1);

			ImGui::Text("Crafted Spear");
			AddItem(Config::Items::CraftedSpear, 1);

			ImGui::Text("Crossbow");
			AddItem(Config::Items::Crossbow, 1);

			ImGui::Text("Pistol");
			AddItem(Config::Items::Pistol, 1);

			ImGui::Text("Machete");
			AddItem(Config::Items::Machete, 1);

			ImGui::Text("Katana");
			AddItem(Config::Items::Katana, 1);

			ImGui::Text("Revolver");
			AddItem(Config::Items::Revolver, 1);

			ImGui::Text("Rifle");
			AddItem(Config::Items::Rifle, 1);

			ImGui::Text("Shotgun");
			AddItem(Config::Items::Shotgun, 1);

			ImGui::Text("Slingshot");
			AddItem(Config::Items::Slingshot, 1);

			ImGui::Text("Stun Baton");
			AddItem(Config::Items::StunBaton, 1);

			ImGui::Text("Taser");
			AddItem(Config::Items::Taser, 1);

			ImGui::Text("Pistol 9mm ammo (x50)");
			AddItem(Config::Items::PistolAmmo, 50);

			ImGui::Text("Shotgun buckshot ammo (x50)");
			AddItem(Config::Items::ShotgunBuckshotAmmo, 50);

			ImGui::Text("Shotgun slug ammo (x50)");
			AddItem(Config::Items::ShotgunSlugAmmo, 50);

			ImGui::Text("Stun gun ammo (x50)");
			AddItem(Config::Items::StunGunAmmo, 50);

			ImGui::Text("Crossbow Bolt (x10)");
			AddItem(Config::Items::CrossbowBolt, 10);

			ImGui::Text("3d printed arrow (x2)");
			AddItem(Config::Items::PrintedArrow, 2);

			ImGui::Text("Carbon fibre arrow (x2)");
			AddItem(Config::Items::CarbonFibreArrow, 2);

			ImGui::Text("Stone arrow (x2)");
			AddItem(Config::Items::StoneArrow, 2);

			ImGui::Text("Zipline rope (x5)");
			AddItem(Config::Items::ZiplineRope, 5);
		}

		// Tools window
		if (Globals::Gui::window == "tools")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Air Tank");
			AddItem(Config::Items::AirTank, 1);

			ImGui::Text("Binoculars");
			AddItem(Config::Items::Binocular, 1);

			ImGui::Text("Can Opener");
			AddItem(Config::Items::CanOpener, 1);

			ImGui::Text("Cross");
			AddItem(Config::Items::Cross, 1);

			ImGui::Text("Electric Chainsaw");
			AddItem(Config::Items::ElectricChainsaw, 1);

			ImGui::Text("Fireman's Axe");
			AddItem(Config::Items::FirefighterAxe, 1);

			ImGui::Text("Flashlight");
			AddItem(Config::Items::Flashlight, 1);

			ImGui::Text("Gold Mask");
			AddItem(Config::Items::GoldMask, 1);

			ImGui::Text("Guitar");
			AddItem(Config::Items::Guitar, 1);

			ImGui::Text("Modern Axe");
			AddItem(Config::Items::ModernAxe, 1);

			ImGui::Text("Printed Sled");
			AddItem(Config::Items::PrintedSled, 1);

			ImGui::Text("Putter");
			AddItem(Config::Items::Putter, 1);

			ImGui::Text("Rebreather");
			AddItem(Config::Items::Rebreather, 1);

			ImGui::Text("Red Mask");
			AddItem(Config::Items::RedMask, 1);

			ImGui::Text("Repair Tool");
			AddItem(Config::Items::RepairTool, 1);

			ImGui::Text("Rope Gun");
			AddItem(Config::Items::RopeGun, 1);

			ImGui::Text("Shovel");
			AddItem(Config::Items::Shovel, 1);

			ImGui::Text("Torch");
			AddItem(Config::Items::Torch, 1);

			ImGui::Text("Turtle Shell");
			AddItem(Config::Items::TurtleShell, 1);
		}

		// Throwables window
		if (Globals::Gui::window == "throwables")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Flare");
			AddItem(Config::Items::Flare, 1);

			ImGui::Text("Frag Grenade");
			AddItem(Config::Items::FragGrenade, 1);

			ImGui::Text("Molotov");
			AddItem(Config::Items::Molotov, 1);

			ImGui::Text("Timed Bomb");
			AddItem(Config::Items::TimedBomb, 1);
		}

		// Materials window
		if (Globals::Gui::window == "materials")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Animal Hide");
			AddItem(Config::Items::AnimalHide, 1);

			ImGui::Text("Batteries");
			AddItem(Config::Items::Batteries, 1);

			ImGui::Text("Bone");
			AddItem(Config::Items::Bone, 1);

			ImGui::Text("C4 Brick");
			AddItem(Config::Items::C4Brick, 1);

			ImGui::Text("Circuit Board");
			AddItem(Config::Items::CircuitBoard, 1);

			ImGui::Text("Cloth (x25)");
			AddItem(Config::Items::Cloth, 25);

			ImGui::Text("Duct Tape");
			AddItem(Config::Items::DuctTape, 1);

			ImGui::Text("Leaf (x50)");
			AddItem(Config::Items::Leaf, 50);

			ImGui::Text("Rock");
			AddItem(Config::Items::Rock, 1);

			ImGui::Text("Rope");
			AddItem(Config::Items::Rope, 1);

			ImGui::Text("Skull");
			AddItem(Config::Items::Skull, 1);

			ImGui::Text("Small Rocks (x2)");
			AddItem(Config::Items::SmallRocks, 2);

			ImGui::Text("Stick (x2)");
			AddItem(Config::Items::Stick, 2);

			ImGui::Text("Watch");
			AddItem(Config::Items::Watch, 1);

			ImGui::Text("Wire");
			AddItem(Config::Items::Wire, 1);
		}

		// Attachments window
		if (Globals::Gui::window == "attachments")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Laser Sight");
			AddItem(Config::Items::LaserSight, 1);

			ImGui::Text("Pistol Rail");
			AddItem(Config::Items::PistolRail, 1);

			ImGui::Text("Scope");
			AddItem(Config::Items::Scope, 1);

			ImGui::Text("Silencer");
			AddItem(Config::Items::PistolSilencer, 1);

			ImGui::Text("Shotgun Rail");
			AddItem(Config::Items::ShotgunRail, 1);

			ImGui::Text("Weapon Flashlight");
			AddItem(Config::Items::FlashlightAttachment, 1);
		}

		// Food & Drink window
		if (Globals::Gui::window == "food&drink")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Blackberries (x10)");
			AddItem(Config::Items::Blackberries, 10);

			ImGui::Text("Blueberries (x10)");
			AddItem(Config::Items::Blueberries, 10);

			ImGui::Text("CannedFoods");
			AddItem(Config::Items::CannedFoods, 1);

			ImGui::Text("Cat Food");
			AddItem(Config::Items::CatFood, 1);

			ImGui::Text("Cereal");
			AddItem(Config::Items::Cereal, 1);

			ImGui::Text("FlyAmanita");
			AddItem(Config::Items::FlyAmanita, 1);

			ImGui::Text("Guarana Berries (x10)");
			AddItem(Config::Items::GuaranaBerries, 10);

			ImGui::Text("HidnumRepandum");
			AddItem(Config::Items::HidnumRepandum, 1);

			ImGui::Text("MRE Pack");
			AddItem(Config::Items::MREPack, 1);

			ImGui::Text("Oyster (x5)");
			AddItem(Config::Items::Oyster, 5);

			ImGui::Text("Ramen Noodles");
			AddItem(Config::Items::RamenNoodles, 1);

			ImGui::Text("Raw Meat");
			AddItem(Config::Items::RawMeat, 1);

			ImGui::Text("Salmon Berries (x10)");
			AddItem(Config::Items::SalmonBerries, 10);

			ImGui::Text("Shiitake");
			AddItem(Config::Items::Shiitake, 1);

			ImGui::Text("Snow Berries (x10)");
			AddItem(Config::Items::Snowberries, 10);

			ImGui::Text("Soda");
			AddItem(Config::Items::Soda, 1);

			ImGui::Text("Steak Bite");
			AddItem(Config::Items::SteakBite, 1);

			ImGui::Text("Twin Berries (x10)");
			AddItem(Config::Items::Twinberries, 10);

			ImGui::Text("Yarrow");
			AddItem(Config::Items::Yarrow, 1);
		}

		// Armor window
		if (Globals::Gui::window == "armor")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Leaf Armor");
			AddItem(Config::Items::LeafArmor, 1);

			ImGui::Text("Hide Armor");
			AddItem(Config::Items::HideArmor, 1);

			ImGui::Text("Bone Armor");
			AddItem(Config::Items::BoneArmor, 1);

			ImGui::Text("Creepy Armor");
			AddItem(Config::Items::CreepyArmor, 1);

			ImGui::Text("Tech Armor");
			AddItem(Config::Items::TechArmor, 1);

			ImGui::Text("Golden Armor");
			AddItem(Config::Items::GoldenArmor, 1);
		}

		// Seeds & Plants window
		if (Globals::Gui::window == "seeds&plants")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Aloe Vera (x10)");
			AddItem(Config::Items::AloeVera, 10);

			ImGui::Text("Aloe Vera Seed (x10)");
			AddItem(Config::Items::AloreVeraSeed, 10);

			ImGui::Text("ArrowLeaf (x10)");
			AddItem(Config::Items::ArrowLeaf, 10);

			ImGui::Text("ArrowLeaf Seed (x10)");
			AddItem(Config::Items::ArrowLeafSeed, 10);

			ImGui::Text("Black Berry Seed (x10)");
			AddItem(Config::Items::BlackberrySeed, 10);

			ImGui::Text("Blue Berry Seed (x10)");
			AddItem(Config::Items::BlueberrySeed, 10);

			ImGui::Text("Chicory (x10)");
			AddItem(Config::Items::Chicory, 10);

			ImGui::Text("Chicory Seed (x10)");
			AddItem(Config::Items::ChicorySeed, 10);

			ImGui::Text("Devil's Club (x10)");
			AddItem(Config::Items::DevilsClub, 10);

			ImGui::Text("Devil's Club Seed (x10)");
			AddItem(Config::Items::DevilsClubSeed, 10);

			ImGui::Text("Fireweed (x10)");
			AddItem(Config::Items::Fireweed, 10);

			ImGui::Text("Fireweed Seed (x10)");
			AddItem(Config::Items::FireweedSeed, 10);

			ImGui::Text("Guarana Seed (x10)");
			AddItem(Config::Items::GuaranaSeed, 10);

			ImGui::Text("Horsetail (x10)");
			AddItem(Config::Items::Horsetail, 10);

			ImGui::Text("Horsetail Seed (x10)");
			AddItem(Config::Items::HorsetailSeed, 10);

			ImGui::Text("Salmon Berry Seed (x10)");
			AddItem(Config::Items::SalmonberrySeed, 10);

			ImGui::Text("Yarrow (x10)");
			AddItem(Config::Items::Yarrow, 10);

			ImGui::Text("Yarrow Seed (x10)");
			AddItem(Config::Items::YarrowSeed, 10);
		}

		// Keycards window
		if (Globals::Gui::window == "keycards")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("additems");

			ImGui::Text("Guest KeyCard");
			AddItem(Config::Items::GuestKeyCard, 1);

			ImGui::Text("Maintenance KeyCard");
			AddItem(Config::Items::MaintenanceKeyCard, 1);

			ImGui::Text("VIP KeyCard");
			AddItem(Config::Items::VIPKeyCard, 1);
		}

		// Spawn Characters window
		if (Globals::Gui::window == "spawncharacters")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("home");

			if (ImGui::Button("Spawn Cannibal", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "cannibal";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Red cannibal", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "fat";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Muddy male", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "muddymale";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Muddy female", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "muddyfemale";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Big Gold Mask cannibal", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "heavy";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Caterpillar", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "caterpillar";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Baby", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "baby";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Twin", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "twins";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Demon", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "demon";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Demon Boss", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "demonboss";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Finger", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "fingers";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn MissPuffy", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "misspuffy";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn MrPuffy", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "mrpuffy";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn MrPuffton", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "mrpuffton";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Shark", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "shark";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Sluggy", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "sluggy";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Kelvin", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "robby";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}

			if (ImGui::Button("Spawn Virginia", ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::DebugAddCharacter::character = "virginia";
				Config::MethodToggleCall::DebugAddDefinedCharacter = true;
			}
		}

		// Modded items window
		if (Globals::Gui::window == "items")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom HangGlider " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("glider");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom KnightV " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("knightV");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom Pistol " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("pistol");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom Revolver " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("revolver");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom Shotgun " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("shotgun");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom FlashLight " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("flashlight");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom Lighter " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("lighter");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom Rebreather " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("rebreather");

			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Custom RopeGun " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("ropegun");
		}

		// Custom HangGlider window
		if (Globals::Gui::window == "glider")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bGlider);
			ImGui::SliderFloat("Flight Speed", &Config::Value::Glider::_constantForwardForce, 10.0f, 1000.0f, "%.1f");
			ImGui::Checkbox("No DownForce", &Config::Value::Glider::noDownForce);
			if (ImGui::SmallButton("Default")) {
				Config::Value::Glider::_constantForwardForce = Config::Value::Glider::Def_constantForwardForce;
			}
		}

		// Custom KnightV window
		if (Globals::Gui::window == "knightV")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bKnightV);
			ImGui::SliderFloat("Max Speed", &Config::Value::KnightV::MaxVelocity, 10.0f, 100.0f, "%.0f");
			ImGui::SliderFloat("Jump Force", &Config::Value::KnightV::JumpForce, 4.0f, 500.0f, "%.0f");
			ImGui::SliderFloat("Max Lean", &Config::Value::KnightV::MinMaxLean, 1.0f, 100.0f, "%.0f");
			if (ImGui::SmallButton("Default")) {
				Config::Value::KnightV::MaxVelocity = Config::Value::KnightV::DefMaxVelocity;
				Config::Value::KnightV::JumpForce = Config::Value::KnightV::DefJumpForce;
				Config::Value::KnightV::MinMaxLean = Config::Value::KnightV::DefMinMaxLean;
			}
		}

		// Custom Pistol window
		if (Globals::Gui::window == "pistol")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bPistol);
			ImGui::Checkbox("Rapid Fire", &Config::Value::Pistol::RapidFire);
			ImGui::Text("Press Q to shoot in rapid fire mode");
		}

		// Custom Revolver window
		if (Globals::Gui::window == "revolver")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bRevolver);
			ImGui::Checkbox("Rapid Fire (Unstable)", &Config::Value::Revolver::RapidFire);
			ImGui::Text("Keep pressed Q to shoot in rapid fire mode");
		}

		// Custom Shotgun window
		if (Globals::Gui::window == "shotgun")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bShotgun);
			ImGui::Checkbox("Rapid Fire", &Config::Value::Shotgun::RapidFire);
			ImGui::Text("Keep pressed Q to shoot in rapid fire mode");
		}

		// Custom FalshLight window
		if (Globals::Gui::window == "flashlight")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bFlashLight);
			ImGui::SliderFloat("Light Strength", &Config::Value::FlashLight::_maxLightIntensity, 16.5f, 40.0f, "%.1f");
			ImGui::Checkbox("No Battery Drain", &Config::Value::FlashLight::NoDrain);
		}

		// Custom Lighter window
		if (Globals::Gui::window == "lighter")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bLighter);
			ImGui::SliderFloat("Light Strength", &Config::Value::Lighter::intensity, 512.0f, 1000000.0f, "%.0f");
			ImGui::Checkbox("Incremented Range", &Config::Value::Lighter::IncRange);
			ImGui::ColorEdit4("Light Color", (float*)&Globals::Gui::LighterColor);
		}

		// Custom Rebreather window
		if (Globals::Gui::window == "rebreather")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bRebreather);
			ImGui::SliderFloat("Light Strength", &Config::Value::Rebreather::_maxLightIntensity, 18.5f, 40.0f, "%.1f");
			ImGui::Checkbox("Unlimited Oxigen", &Config::Value::Rebreather::InfOxigen);
		}

		// Custom RopeGun window
		if (Globals::Gui::window == "ropegun")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("items");

			ImGui::Checkbox("Enable/Disable", &Config::bRopeGun);
			ImGui::Checkbox("Infinite Length", &Config::Value::RopeGun::InfLength);
		}

		// Environment window
		if (Globals::Gui::window == "environment")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("home");

			// Sun Control
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Sun Control " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("sun");

			// Wind Control
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Wind Control " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("wind");

			// DayTime Control
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " DayTime Control (Don't use in multi) " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("daytime");

			// Season Control
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Season Control " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("season");

			// Tree Regrow Control
			ImGui::Text(ICON_FA_ARROW_RIGHT_LONG " Tree Regrow Control (HOST only ?) " ICON_FA_ARROW_LEFT_LONG);
			SetWindow("regrow");
		}

		if (Globals::Gui::window == "sun")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("environment");

			ImGui::Checkbox("Enable/Disable", &Config::bSunControl);
			ImGui::SliderFloat("Light Intensity", &Config::Value::SunControl::Intensity, 0, 10000000.0f, "%.0f");
			ImGui::ColorEdit4("Sun Light Color", (float*)&Globals::Gui::SunLightColor);
			if (ImGui::SmallButton("Default"))
			{
				Globals::Gui::SunLightColor = ImVec4(1, 1, 1, 1);
				Config::Value::SunControl::Intensity = 100000.0f;
			}
		}

		if (Globals::Gui::window == "wind")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("environment");

			ImGui::Checkbox("Enable/Disable", &Config::bWindControl);
			ImGui::SliderFloat("Wind Intensity", &Config::Value::WindControl::Intensity, 0, 5.0f, "%.1f");
		}

		if (Globals::Gui::window == "daytime")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("environment");

			ImGui::Checkbox("Enable/Disable", &Config::bDayTime);
			ImGui::SliderFloat("DayTime Speed Multiplier", &Config::Value::DayTimeControl::_baseSpeedMultiplier, 0, 10000.0f, "%.0f");
			ImGui::Text("0 = freeze time\nCTRL + Left click to manual input the value");
		}

		if (Globals::Gui::window == "season")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("environment");

			int season = Globals::SeasonsManager->CallMethod<Config::SeasonsManager::Season>(Globals::Methods::get_ActiveSeason);

			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::mainTheme;
			switch (season)
			{
			case 0: 
				ImGui::Text(ICON_FA_ARROW_RIGHT " Spring " ICON_FA_ARROW_LEFT);
				break;
			case 1:
				ImGui::Text(ICON_FA_ARROW_RIGHT " Summer " ICON_FA_ARROW_LEFT);
				break;
			case 2:
				ImGui::Text(ICON_FA_ARROW_RIGHT " Fall " ICON_FA_ARROW_LEFT);
				break;
			case 3:
				ImGui::Text(ICON_FA_ARROW_RIGHT " Winter " ICON_FA_ARROW_LEFT);
				break;
			}
			Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::white;

			if (ImGui::Button("Spring " ICON_FA_SUN_PLANT_WILT, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::SetSeason::season = "spring";
				Config::MethodToggleCall::SetSeason = true;
			}

			if (ImGui::Button("Summer " ICON_FA_SUN, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::SetSeason::season = "summer";
				Config::MethodToggleCall::SetSeason = true;
			}

			if (ImGui::Button("Fall " ICON_FA_LEAF, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::SetSeason::season = "fall";
				Config::MethodToggleCall::SetSeason = true;
			}

			if (ImGui::Button("Winter " ICON_FA_SNOWFLAKE, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				Config::MethodToggleCall::Value::SetSeason::season = "winter";
				Config::MethodToggleCall::SetSeason = true;
			}
		}

		if (Globals::Gui::window == "regrow")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Home");
			SetWindow("home");

			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("environment");

			ImGui::Checkbox("Enable/Disable", &Config::bTreeRegrow);
			ImGui::Text("Tree Regrow Rate");
			ImGui::InputFloat("##", &Config::Value::TreeRegrow::_regrowthFactor, 0.1f, NULL, 1);
			if (ImGui::SmallButton("Default"))
			{
				Config::Value::TreeRegrow::_regrowthFactor = 0.1f;
			}
			ImGui::Text("1 seems like 100%% trees will regrow after sleeping");
		}

		// Teleport window
		if (Globals::Gui::window == "teleport")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("home");

			ImGui::Checkbox("Interactive Map", &Globals::Gui::showMap);
			if (Globals::Gui::showMap)
			{
				Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 0, 0);
				DisplayMap(map_texture);
				Globals::Gui::style->Colors[ImGuiCol_Text] = Colors::white;
			}

			ImGui::Text("My Base (Lake)");
			TeleportTo(Unity::Vector3(-1000, 120, -57), lake_texture);

			ImGui::Text("Shotgun Grave Loc.");
			TeleportTo(Unity::Vector3(-1340, 102, 1412), shotgun_texture);

			ImGui::Text("Pistol Loc.");
			TeleportTo(Unity::Vector3(-1797, 16, 578), pistol_texture);

			ImGui::Text("Hang Glider 1st Loc.");
			TeleportTo(Unity::Vector3(-1307, 87, 1732), hang_glider_texture);

			ImGui::Text("Knight V 1st Loc.");
			TeleportTo(Unity::Vector3(-1026, 231, -625), knightV_texture);

			ImGui::Text("Top Of The Mountain");
			TeleportTo(Unity::Vector3(4, 716, -459), top_of_mountain_texture);

			ImGui::Text("Rebreather + Stun Gun Cave Entrance");
			TeleportTo(Unity::Vector3(-418, 19, 1532), rebreather_stungun_texture);

			ImGui::Text("Flashlight Loc.");
			TeleportTo(Unity::Vector3(-630, 142, 391), flashlight_texture);

			ImGui::Text("Modern Axe Loc.");
			TeleportTo(Unity::Vector3(-704, 108, 450), modern_axe_texture);

			ImGui::Text("Machete Loc.");
			TeleportTo(Unity::Vector3(-65, 20, 1458), machete_texture);

			ImGui::Text("Stun Baton Loc.");
			TeleportTo(Unity::Vector3(-1142, 134, -157), stun_baton_texture);

			ImGui::Text("Putter Loc.");
			TeleportTo(Unity::Vector3(1024, 145, 1212), putter_texture);

			ImGui::Text("Binocular Loc.");
			TeleportTo(Unity::Vector3(-1109, 20, 1721), binocular_texture);

			ImGui::Text("Revolver Loc. (Bunker Entrance?)");
			TeleportTo(Unity::Vector3(1111, 132, 1003), revolver_texture);

			ImGui::Text("Cross + Rope Gun Cave Entrance");
			TeleportTo(Unity::Vector3(-1113, 132, -171), rope_gun_texture);

			ImGui::Text("Slingshot + Shovel Cave Entrance");
			TeleportTo(Unity::Vector3(-531, 200, 124), shovel_texture);

			ImGui::Text("Compound Bow Bunker Entrance");
			TeleportTo(Unity::Vector3(-1136, 284, -1095), moder_bow_texture);

			ImGui::Text("3D Printer + Guest KeyCard + Guitar + Chainsaw Bunker Entrance");
			TeleportTo(Unity::Vector3(-1188, 70, 133), chainsaw_texture);

			ImGui::Text("FireAxe + Maintenance Card Bunker Entrance");
			TeleportTo(Unity::Vector3(-474, 90, 710), fire_axe_texture);

			ImGui::Text("VIP Keycard + Crossbow Bunker Entrance");
			TeleportTo(Unity::Vector3(-1014, 102, 1024), crossbow_texture);

			ImGui::Text("End Game Bunker Entrance");
			TeleportTo(Unity::Vector3(1756, 45, 553), endgame_texture);
		}

		// Theme window
		if (Globals::Gui::window == "theme")
		{
			ImGui::Text(ICON_FA_ARROW_LEFT_LONG " Go Back");
			SetWindow("home");

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
			SetWindow("home");

			ImGui::Text("Player Coords");
			Unity::Vector3 playerLoc = Globals::LocalPlayer->GetTransform()->GetPosition();
			ImGui::InputFloat("X", &playerLoc.x, NULL, NULL, 0, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat("Y", &playerLoc.y, NULL, NULL, 0, ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat("Z", &playerLoc.z, NULL, NULL, 0, ImGuiInputTextFlags_ReadOnly);

			ImGui::Separator();

			ImGui::Text("Add Items");
			ImGui::InputInt("Item ID", &Config::Items::itemID, NULL, NULL, ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::InputInt("Item Qty", &Config::Items::itemQTY, 1, NULL, ImGuiInputTextFlags_EnterReturnsTrue);
			if (ImGui::SmallButton("Add Item"))
			{
				Globals::ItemInstanceManager->CallMethod<bool, int, int>(Globals::Methods::TryAddItems, Config::Items::itemID, Config::Items::itemQTY);
			}

			ImGui::Separator();

			ImGui::Text("Spawn Characters");
			ImGui::InputText("Chr Name", &Config::MethodToggleCall::Value::DebugAddCharacter::entity);

			if (ImGui::SmallButton("Spawn character"))
			{
				Config::MethodToggleCall::DebugAddCharacter = true;
			}
			/*
			ImGui::Separator();

			ImGui::Text("Spawn Items");
			ImGui::InputText("Item Name", &Config::MethodToggleCall::Value::SpawnItem::itemTxt);

			if (ImGui::SmallButton("Spawn item"))
			{
				Config::MethodToggleCall::SpawnItem = true;
			}
			*/
		}

		//About window
		if (Globals::Gui::showAbout)
		{
			ImGui::SetCursorPos(ImVec2(5, 235));
			Globals::Gui::style->Colors[ImGuiCol_ChildBg] = ImColor(36, 36, 36);
			Globals::Gui::style->ChildBorderSize = 2.0f;
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
