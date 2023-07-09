#include "Globals.h"
#include "config.h"
#include <thread>
#include <chrono>
#include <iostream>

#define PTRCAST(t, a) reinterpret_cast<t>(a)

void findInMemory()
{
	SetConsoleTextAttribute(Globals::Gui::hConsole, 15);
	switch (Globals::Gui::dot)
	{
		case 1:
			printf("Searching Required Things.\n");
			++Globals::Gui::dot;
			break;
		case 2:
			printf("Searching Required Things..\n");
			++Globals::Gui::dot;
			break;
		case 3:
			printf("Searching Required Things...\n");
			Globals::Gui::dot = 1;
			break;
	}
	
	// GameAssembly.dll
	Globals::GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");

	if (Globals::GameAssembly == 0)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find GameAssembly\n");
		return;
	}

	SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
	std::cout << "[+] Found GameAssembly at: " << std::hex << Globals::GameAssembly << '\n';

	// Local Player
	Globals::Classes::LocalPlayerClass = IL2CPP::Class::Find("TheForest.Utils.LocalPlayer");

	Globals::LocalPlayer = Unity::GameObject::Find("LocalPlayer");
	
	if (!Globals::LocalPlayer)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find LocalPlayer\n");
		return;
	}

	SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
	std::cout << "[+] Found LocalPlayer at: " << Globals::LocalPlayer << '\n';

	Globals::ItemInstanceManager = Globals::LocalPlayer->GetComponent("ItemInstanceManager");
	Globals::Methods::TryAddItems = Globals::ItemInstanceManager->GetMethodPointer("TryAddItems");

	// FirstPersonCharacter
	Globals::FirstPersonCharacter = Globals::LocalPlayer->GetComponent("FirstPersonCharacter");

	if (!Globals::FirstPersonCharacter)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find FirstPersonCharacter\n");
		return;
	}

	SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
	std::cout << "[+] Found FirstPersonCharacter at: " << Globals::FirstPersonCharacter << '\n';

	// Vitals
	Globals::Vitals = Globals::LocalPlayer->GetComponent("Vitals");

	if (!Globals::Vitals)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find Vitals component\n");
		return;
	}

	SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
	std::cout << "[+] Found Vitals component at: " << Globals::Vitals << "\n";

	// PlayerHangGliderAction Component
	Globals::PlayerHangGliderAction = Globals::LocalPlayer->GetComponentInChildren("Sons.Gameplay.PlayerHangGliderAction");

	if (!Globals::PlayerHangGliderAction)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find PlayerHangGliderAction component\n");
	}
	else
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found PlayerHangGliderAction component at: " << Globals::PlayerHangGliderAction << "\n";
	}

	// KnightVControlDefinition Component
	Globals::PlayerKnightVAction = Globals::LocalPlayer->GetComponentInChildren("Sons.Gameplay.PlayerKnightVAction");

	if (!Globals::PlayerKnightVAction)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find PlayerKnightVAction component\n");
	}
	else
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found PlayerKnightVAction component at: " << Globals::PlayerKnightVAction << "\n";

		Globals::KnightVControlDefinition = Globals::PlayerKnightVAction->GetMemberValue<Unity::CComponent*>("_controlDefinition");

		if (!Globals::KnightVControlDefinition)
		{
			SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
			printf("[-] Cannot find KnightVControlDefinition component\n");
		}
		else
		{
			SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
			std::cout << "	-> Found KnightVControlDefinition component: " << Globals::KnightVControlDefinition << '\n';
		}
	}

	// Atmosphere CGameObject
	Globals::Atmosphere = Unity::GameObject::Find("Atmosphere");

	if (!Globals::Atmosphere) {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find Atmosphere GameObject\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found Atmosphere GameObject: " << Globals::Atmosphere << '\n';
	}

	if (Globals::Atmosphere) {
		// SunControl
		Globals::SunLightManager = Globals::Atmosphere->GetComponentInChildren("Sons.Atmosphere.SunLightManager");

		if (!Globals::SunLightManager) {
			SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
			printf("[-] Cannot find SunLightManager component\n");
		}
		else {
			SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
			std::cout << "	-> Found SunLightManager component: " << Globals::SunLightManager << '\n';
		}

		if (Globals::SunLightManager) {
			Globals::SunLight = Globals::SunLightManager->GetMemberValue<Unity::CComponent*>("_sunLight");

			if (!Globals::SunLight) {
				SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
				printf("[-] Cannot find SunLight component\n");
				return;
			}
			else {
				SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
				std::cout << "		-> Found SunLight component: " << Globals::SunLight << '\n';
			}
		}
		// WindControl
		Globals::WindManager = Globals::Atmosphere->GetComponentInChildren("Sons.Atmosphere.WindManager");

		if (!Globals::WindManager) {
			SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
			printf("[-] Cannot find WindManager component\n");
		}
		else {
			SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
			std::cout << "[+] Found WindManager component : " << Globals::WindManager << '\n';
		}
	}

	// GameManagers CGameObject
	Globals::GameManagers = Unity::GameObject::Find("GameManagers");

	if (!Globals::GameManagers) {

		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find GameManagers GameObject\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found GameManagers GameObject: " << Globals::GameManagers << '\n';
	}

	if (Globals::GameManagers) {

		Globals::TimeOfDayHolder = Globals::GameManagers->GetComponentInChildren("Sons.Environment.TimeOfDayHolder");
		if (!Globals::TimeOfDayHolder) {

			SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
			printf("[-] Cannot find TimeOfDayHolder component\n");
		}
		else {
			SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
			std::cout << "[+] Found TimeOfDayHolder component: " << Globals::TimeOfDayHolder << '\n';
		}
	}

	// StructureCrafingSystem CGameObject
	Globals::StructureCraftingSystem = Unity::GameObject::Find("StructureCraftingSystem");

	if (!Globals::StructureCraftingSystem) {

		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find StructureCraftingSystem GameObject\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found StructureCraftingSystem GameObject: " << Globals::StructureCraftingSystem << '\n';
	}

	if (Globals::StructureCraftingSystem) {

		Globals::StructureCraftingSystem_component = Globals::StructureCraftingSystem->GetComponentInChildren("Sons.Crafting.Structures.StructureCraftingSystem");
		if (!Globals::StructureCraftingSystem_component) {

			SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
			printf("[-] Cannot find StructureCraftingSystem component\n");
		}
		else {
			SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
			std::cout << "[+] Found StructureCraftingSystem component: " << Globals::StructureCraftingSystem_component << '\n';
		}
	}

	// DebugConsole CGameObject
	Globals::DebugConsole = Unity::GameObject::Find("DebugConsole");

	if (!Globals::DebugConsole) {

		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find DebugConsole GameObject\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found DebugConsole GameObject: " << Globals::DebugConsole << '\n';
	}

	Globals::Methods::_spawnitem = IL2CPP::Class::Utils::GetMethodPointer("TheForest.DebugConsole", "_spawnitem");

	// CharacterManager GameObject
	Globals::CharacterManager = Unity::GameObject::Find("GameManagers/CharacterManager");

	if (!Globals::CharacterManager) {

		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find CharacterManager GameObject\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found CharacterManager GameObject: " << Globals::CharacterManager << '\n';
	}

	Globals::CharacterManager_component = Globals::CharacterManager->GetComponent("CharacterManager");

	Globals::Methods::DebugAddCharacter = IL2CPP::Class::Utils::GetMethodPointer("Sons.Characters.CharacterManager", "DebugAddCharacter");

	// TreeRegrowChecker Component
	Globals::TreeRegrowChecker = Globals::GameManagers->GetComponentInChildren("Sons.Gameplay.TreeRegrowChecker");

	if (!Globals::TreeRegrowChecker) {

		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find TreeRegrowChecker Component\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found TreeRegrowChecker Component: " << Globals::TreeRegrowChecker << '\n';
	}

	// SeasonsManager Component
	Globals::SeasonsManager = Globals::Atmosphere->GetComponentInChildren("Sons.Atmosphere.SeasonsManager");

	if (!Globals::SeasonsManager) {

		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find SeasonsManager Component\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found SeasonsManager Component: " << Globals::SeasonsManager << '\n';
	}

	Globals::Methods::get_ActiveSeason = Globals::SeasonsManager->GetMethodPointer("get_ActiveSeason");

	SetConsoleTextAttribute(Globals::Gui::hConsole, 9);
	printf("Required Things Found!\n\n");

	SetConsoleTextAttribute(Globals::Gui::hConsole, 15);
	printf("Searching Values...\n");
	
	// Fullness
	Globals::Stats::Fullness = Globals::Vitals->GetPropertyValue<Unity::CObject*>("Fullness");

	if (!Globals::Stats::Fullness)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find Fullness value\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found Fullness value: " << Globals::Stats::Fullness << '\n';
	}

	// Hydration
	Globals::Stats::Hydration = Globals::Vitals->GetPropertyValue<Unity::CObject*>("Hydration");

	if (!Globals::Stats::Hydration)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find Hydration value\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found Hydration value: " << Globals::Stats::Hydration << '\n';
	}

	// Rested
	Globals::Stats::Rest = Globals::Vitals->GetPropertyValue<Unity::CObject*>("Rested");

	if (!Globals::Stats::Rest)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find Sleep value\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found Sleep value: " << Globals::Stats::Rest << '\n';
	}

	SetConsoleTextAttribute(Globals::Gui::hConsole, 9);
	printf("Values Search Finished!\n\n");
}

bool findPistol()
{
	Globals::CompactPistolWeaponController = Globals::LocalPlayer->GetComponentInChildren("Sons.Weapon.CompactPistolWeaponController");
	if (!Globals::CompactPistolWeaponController) {
		return FALSE;
	}
	return TRUE;
}

bool findRevolver()
{
	Globals::RevolverWeaponController = Globals::LocalPlayer->GetComponentInChildren("Sons.Weapon.RevolverWeaponController");
	if (!Globals::RevolverWeaponController) {
		return FALSE;
	}

	if (Config::Value::Revolver::FireWeapon == nullptr)
	{
		Config::Value::Revolver::FireWeapon = IL2CPP::Class::Utils::GetMethodPointer("Sons.Weapon.RevolverWeaponController", "FireWeapon");
	}
	return TRUE;
}

bool findShotgun()
{
	Globals::ShotgunWeaponController = Globals::LocalPlayer->GetComponentInChildren("Sons.Weapon.ShotgunWeaponController");
	if (!Globals::ShotgunWeaponController) {
		return FALSE;
	}

	if (Config::Value::Shotgun::FireWeapon == nullptr)
	{
		Config::Value::Shotgun::FireWeapon = IL2CPP::Class::Utils::GetMethodPointer("Sons.Weapon.ShotgunWeaponController", "FireWeapon");
	}
	return TRUE;
}

bool findFlashLight()
{
	Globals::FlashlightController = Globals::LocalPlayer->GetComponentInChildren("Sons.Weapon.FlashlightController");
	if (!Globals::FlashlightController) {
		return FALSE;
	}
	return TRUE;
}

bool findLighter()
{
	Globals::PlasmaLighterController = Globals::LocalPlayer->GetComponentInChildren("Sons.Item.PlasmaLighterController");
	if (Globals::PlasmaLighterController) {
		Globals::LocalPlayerPlasmaLight = Globals::PlasmaLighterController->GetMemberValue<Unity::CComponent*>("_localPlayerLight");
		if (!Globals::LocalPlayerPlasmaLight) {
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

bool findRebreather()
{
	Globals::RebreatherController = Globals::LocalPlayer->GetComponentInChildren("Sons.Gameplay.RebreatherController");
	if (!Globals::RebreatherController) {
		return FALSE;
	}
	return TRUE;
}

bool findRopeGun()
{
	Globals::RopeGunController = Globals::LocalPlayer->GetComponentInChildren("Sons.Weapon.RopeGunController");
	if (!Globals::RopeGunController) {
		return FALSE;
	}
	return TRUE;
}

void setMemory()
{
	// Fullness
	if (Config::bFullness)
	{
		if (!Globals::Stats::Fullness) {
			return;
		}

		Globals::Stats::Fullness->SetMemberValue<float>("_currentValue", Config::Value::fullness);
		Globals::ToggleStats::toggleFullness = false;
	}
	else if (!Globals::ToggleStats::toggleFullness)
	{
		Globals::ToggleStats::toggleFullness = true;
		Globals::Stats::Fullness->SetMemberValue<float>("_currentValue", 90.0f);
	}
	// Hydration
	if (Config::bHydration)
	{
		if (!Globals::Stats::Hydration) {
			return;
		}

		Globals::Stats::Hydration->SetMemberValue<float>("_currentValue", Config::Value::hydration);
		Globals::ToggleStats::toggleHydration = false;
	}
	else if (!Globals::ToggleStats::toggleHydration)
	{
		Globals::ToggleStats::toggleHydration = true;
		Globals::Stats::Hydration->SetMemberValue<float>("_currentValue", 90.0f);
	}
	// Rested
	if (Config::bRest)
	{
		if (!Globals::Stats::Rest) {
			return;
		}

		Globals::Stats::Rest->SetMemberValue<float>("_currentValue", Config::Value::rest);
		Globals::ToggleStats::toggleRest = false;
	}
	else if (!Globals::ToggleStats::toggleRest)
	{
		Globals::ToggleStats::toggleRest = true;
		//float _defaultvalue = Globals::Stats::Stamina->GetMemberValue<float>("_defaultValue");
		Globals::Stats::Rest->SetMemberValue<float>("_currentValue", 90.0f);
	}
	// Speed
	if (Config::bSpeed)
	{
		if (!Globals::FirstPersonCharacter) {
			return;
		}

		Globals::FirstPersonCharacter->SetMemberValue<float>("_runSpeed", Config::Value::runSpeed);
		Globals::FirstPersonCharacter->SetMemberValue<float>("_swimSpeed", Config::Value::swimSpeedMultiplier);
		Globals::FirstPersonCharacter->SetMemberValue<float>("_swimSpeedMultiplier", Config::Value::swimSpeedMultiplier);
		Globals::ToggleStats::toggleSpeed = false;
	}
	else if (!Globals::ToggleStats::toggleSpeed)
	{
		Globals::ToggleStats::toggleSpeed = true;
		Globals::FirstPersonCharacter->SetMemberValue<float>("_runSpeed", 5.4f);
		Globals::FirstPersonCharacter->SetMemberValue<float>("_swimSpeed", 5.4f);
		Globals::FirstPersonCharacter->SetMemberValue<float>("_swimSpeedMultiplier", 1.0f);
	}
	// Super Jump
	if (Config::bJump)
	{
		if (!Globals::FirstPersonCharacter) {
			return;
		}

		Globals::FirstPersonCharacter->SetMemberValue<float>("_jumpHeight", Config::Value::jumpHeight);
		Globals::ToggleStats::toggleJumpHeight = false;
	}
	else if (!Globals::ToggleStats::toggleJumpHeight)
	{
		Globals::ToggleStats::toggleJumpHeight = true;
		Globals::FirstPersonCharacter->SetMemberValue<float>("_jumpHeight", 2.6f);
	}
	// Fall Damage
	if (Config::bFallDamage)
	{
		if (Globals::FirstPersonCharacter) {
			Globals::FirstPersonCharacter->SetMemberValue<bool>("_allowFallDamage", false);
		}
	}
	else {
		Globals::FirstPersonCharacter->SetMemberValue<bool>("_allowFallDamage", true);
	}

	// Disable Gravity
	if (Config::bGravity)
	{
		if (Globals::FirstPersonCharacter) {
			Globals::FirstPersonCharacter->SetMemberValue<bool>("_disableGravity", true);
		}
	}
	else {
		Globals::FirstPersonCharacter->SetMemberValue<bool>("_disableGravity", false);
	}
	// Instant Build
	if (Config::bInstantBuild)
	{
		if (Globals::StructureCraftingSystem_component) {
			Globals::StructureCraftingSystem_component->SetMemberValue<bool>("<InstantBuild>k__BackingField", true);
		}
	}
	else {
		if (Globals::StructureCraftingSystem_component)
		{
			Globals::StructureCraftingSystem_component->SetMemberValue<bool>("<InstantBuild>k__BackingField", false);
		}
	}
	// Custom HangGlider
	if (Config::bGlider)
	{
		if (Globals::PlayerHangGliderAction) {
			Globals::PlayerHangGliderAction->SetMemberValue<float>("_constantForwardForce", Config::Value::Glider::_constantForwardForce);

			if (Config::Value::Glider::noDownForce) {
				Globals::PlayerHangGliderAction->SetMemberValue<float>("_baseDownPitchForce", 0);
				Globals::PlayerHangGliderAction->SetMemberValue<float>("_upPitchDownForce", 0);
			}
			else if (!Config::Value::Glider::noDownForce) {
				Globals::PlayerHangGliderAction->SetMemberValue<float>("_baseDownPitchForce", 150.0f);
				Globals::PlayerHangGliderAction->SetMemberValue<float>("_upPitchDownForce", 3000.0f);
			}
		}
	}
	else {
		if (Globals::PlayerHangGliderAction) {
			Globals::PlayerHangGliderAction->SetMemberValue<float>("_constantForwardForce", Config::Value::Glider::Def_constantForwardForce);

			Globals::PlayerHangGliderAction->SetMemberValue<float>("_baseDownPitchForce", 150.0f);
			Globals::PlayerHangGliderAction->SetMemberValue<float>("_upPitchDownForce", 3000.0f);
		}
	}
	// Custom KnightV
	if (Config::bKnightV)
	{
		if (Globals::KnightVControlDefinition)
		{
			Globals::KnightVControlDefinition->SetMemberValue<float>("JumpForce", Config::Value::KnightV::JumpForce);
			Globals::KnightVControlDefinition->SetMemberValue<float>("MaxVelocity", Config::Value::KnightV::MaxVelocity);
			Globals::KnightVControlDefinition->SetMemberValue<float>("MinMaxLean", Config::Value::KnightV::MinMaxLean);
		}
	}
	else {
		if (Globals::KnightVControlDefinition)
		{
			Globals::KnightVControlDefinition->SetMemberValue<float>("JumpForce", Config::Value::KnightV::DefJumpForce);
			Globals::KnightVControlDefinition->SetMemberValue<float>("MaxVelocity", Config::Value::KnightV::DefMaxVelocity);
			Globals::KnightVControlDefinition->SetMemberValue<float>("MinMaxLean", Config::Value::KnightV::DefMinMaxLean);
		}
	}
	//Custom Pistol
	if (Config::bPistol)
	{
		if (Globals::CompactPistolWeaponController)
		{
			if (Config::Value::Pistol::RapidFire) {
				Globals::CompactPistolWeaponController->SetMemberValue<float>("_fireDelay", 0);

				INPUT input{ 0 };
				input.type = INPUT_MOUSE;

				if (GetAsyncKeyState(0x51)) // letter q
				{
					input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
					SendInput(1, &input, sizeof(INPUT));

					input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
					SendInput(1, &input, sizeof(INPUT));
				}
			}
			else if (!Config::Value::Pistol::RapidFire) {
				Globals::CompactPistolWeaponController->SetMemberValue<float>("_fireDelay", 0.2f);
			}
		}
	}
	else {
		if (Globals::CompactPistolWeaponController)
		{
			Globals::CompactPistolWeaponController->SetMemberValue<float>("_fireDelay", 0.2f);
		}
	}
	//Custom Flashlight
	if (Config::bFlashLight)
	{
		if (Globals::FlashlightController)
		{
			Globals::FlashlightController->SetMemberValue<float>("_maxLightIntensity", Config::Value::FlashLight::_maxLightIntensity);

			if (Config::Value::FlashLight::NoDrain) {
				Globals::FlashlightController->SetMemberValue<float>("_powerDrainRate", 0);
			}
			else if (!Config::Value::FlashLight::NoDrain) {
				Globals::FlashlightController->SetMemberValue<float>("_powerDrainRate", 0.1667f);
			}
		}
	}
	else {
		if (Globals::FlashlightController)
		{
			Globals::FlashlightController->SetMemberValue<float>("_maxLightIntensity", 16.5f);
			Globals::FlashlightController->SetMemberValue<float>("_powerDrainRate", 0.1667f);
		}
	}
	// Custom Lighter
	if (Config::bLighter)
	{
		if (Globals::PlasmaLighterController && Globals::LocalPlayerPlasmaLight)
		{
			Globals::LocalPlayerPlasmaLight->SetMemberValue<float>("intensity", Config::Value::Lighter::intensity);
			Globals::LocalPlayerPlasmaLight->SetMemberValue<ImVec4>("color", Globals::Gui::LighterColor);

			if (Config::Value::Lighter::IncRange) {
				Globals::LocalPlayerPlasmaLight->SetMemberValue<float>("range", 1000.0f);
			}
			else if (!Config::Value::Lighter::IncRange) {
				Globals::LocalPlayerPlasmaLight->SetMemberValue<float>("range", 15.0f);
			}
		}
	}
	else {
		if (Globals::PlasmaLighterController && Globals::LocalPlayerPlasmaLight) 
		{
			Globals::LocalPlayerPlasmaLight->SetMemberValue<float>("intensity", 512.0f);
			Globals::LocalPlayerPlasmaLight->SetMemberValue<ImVec4>("color", ImVec4(0.67840004f, 0, 1, 1));
			Globals::LocalPlayerPlasmaLight->SetMemberValue<float>("range", 15.0f);
		}
	}
	// Custom Rebreather
	if (Config::bRebreather)
	{
		if (Globals::RebreatherController)
		{
			Globals::RebreatherController->SetMemberValue<float>("_airConsumptionRate", 0);
			Globals::RebreatherController->SetMemberValue<float>("_maxLightIntensity", Config::Value::Rebreather::_maxLightIntensity);

			if (Config::Value::Rebreather::InfOxigen)
			{
				Globals::RebreatherController->SetMemberValue<float>("_airConsumptionRate", 0);
			}
			else if (!Config::Value::Rebreather::InfOxigen)
			{
				Globals::RebreatherController->SetMemberValue<float>("_airConsumptionRate", 2.0f);
			}
		}
	}
	else {
		if (Globals::RebreatherController)
		{
			Globals::RebreatherController->SetMemberValue<float>("_airConsumptionRate", 2.0f);
			Globals::RebreatherController->SetMemberValue<float>("_maxLightIntensity", 18.5f);
		}
	}
	// Custom RopeGun
	if (Config::bRopeGun)
	{
		if (Globals::RopeGunController)
		{
			if (Config::Value::RopeGun::InfLength)
			{
				Globals::RopeGunController->SetMemberValue<float>("_maxFiringRange", 50000000.0f);
				Globals::RopeGunController->SetMemberValue<float>("_maxRopeLength", 50000000.0f);
			}
			else if (!Config::Value::RopeGun::InfLength)
			{
				Globals::RopeGunController->SetMemberValue<float>("_maxFiringRange", 50.0f);
				Globals::RopeGunController->SetMemberValue<float>("_maxRopeLength", 150.0f);
			}
		}
	}
	else {
		if (Globals::RopeGunController)
		{
			Globals::RopeGunController->SetMemberValue<float>("_maxFiringRange", 50.0f);
			Globals::RopeGunController->SetMemberValue<float>("_maxRopeLength", 150.0f);
		}
	}
	// Sun Control
	if (Config::bSunControl)
	{
		if (Globals::SunLight) {
			Globals::SunLight->SetMemberValue<ImVec4>("color", Globals::Gui::SunLightColor);
			Globals::SunLight->SetMemberValue<float>("intensity", Config::Value::SunControl::Intensity);
		}
	}
	else {

		if (Globals::SunLight) {
			Globals::SunLight->SetMemberValue<ImVec4>("color", ImVec4(1, 1, 1, 1));
			Globals::SunLight->SetMemberValue<float>("intensity", 100000.0f);
		}
	}
	// Wind Control
	if (Config::bWindControl)
	{
		if (Globals::WindManager) {
			Globals::WindManager->SetMemberValue<float>("_appliedStormIntensityMultiplier", Config::Value::WindControl::Intensity);
		}
	}
	// DayTime Control
	if (Config::bDayTime)
	{
		if (Globals::TimeOfDayHolder) 
		{
			Globals::TimeOfDayHolder->SetMemberValue<float>("_baseSpeedMultiplier", Config::Value::DayTimeControl::_baseSpeedMultiplier);
		}
	}
	else {
		if (Globals::TimeOfDayHolder) {

			Globals::TimeOfDayHolder->SetMemberValue<float>("_baseSpeedMultiplier", 1.0f);
		}
	}
	// Tree Regrow Control
	if (Config::bTreeRegrow)
	{
		if (Globals::TreeRegrowChecker)
		{
			Globals::TreeRegrowChecker->SetMemberValue<float>("_regrowthFactor", Config::Value::TreeRegrow::_regrowthFactor);
		}
	}
	else {
		if (Globals::TreeRegrowChecker) {

			Globals::TreeRegrowChecker->SetMemberValue<float>("_regrowthFactor", 0.1f);
		}
	}
}

// Methods calling begins here
void DebugAddCharacter()
{
	if (Config::MethodToggleCall::DebugAddCharacter)
	{
		Globals::CharacterManager_component->CallMethod<bool>("DebugAddCharacter", IL2CPP::String::New(Config::MethodToggleCall::Value::DebugAddCharacter::entity), true);
		Config::MethodToggleCall::DebugAddCharacter = false;
	}

	if (Config::MethodToggleCall::DebugAddDefinedCharacter)
	{
		Globals::CharacterManager_component->CallMethod<bool>("DebugAddCharacter", IL2CPP::String::New(Config::MethodToggleCall::Value::DebugAddCharacter::character), true);
		Config::MethodToggleCall::DebugAddDefinedCharacter = false;
	}
}

void SetSeason()
{
	if (Config::MethodToggleCall::SetSeason)
	{
		void* _season = IL2CPP::Class::Utils::GetMethodPointer("TheForest.DebugConsole", "_season");
		Globals::DebugConsole->CallMethod<void>(_season, IL2CPP::String::New(Config::MethodToggleCall::Value::SetSeason::season));
		Config::MethodToggleCall::SetSeason = false;
	}
}

void SpawnItem()
{
	if (Config::MethodToggleCall::SpawnItem)
	{
		void* _spawnitem = IL2CPP::Class::Utils::GetMethodPointer("TheForest.DebugConsole", "_spawnitem");
		Globals::DebugConsole->CallMethod<void>(_spawnitem, IL2CPP::String::New(Config::MethodToggleCall::Value::SpawnItem::itemTxt));
		Config::MethodToggleCall::SpawnItem = false;
	}
}

// Custom Shotgun
void ShotgunFire()
{
	if (Config::bShotgun)
	{
		if (Globals::ShotgunWeaponController)
		{
			if (Config::Value::Shotgun::RapidFire)
			{
				if (GetAsyncKeyState(0x51) & 1) // letter q
				{
					Globals::ShotgunWeaponController->CallMethod<void>(Config::Value::Shotgun::FireWeapon);
				}
			}
		}
	}
}

// Custom Revolver
void RevolverFire()
{
	if (Config::bRevolver)
	{
		if (Globals::RevolverWeaponController)
		{
			if (Config::Value::Revolver::RapidFire) {

				if (GetAsyncKeyState(0x51) & 1) // letter q
				{
					Globals::RevolverWeaponController->CallMethod<void>(Config::Value::Revolver::FireWeapon);
				}
			}
		}
	}
}

void InstantiateMethods()
{
	IL2CPP::Callback::OnUpdate::Add(DebugAddCharacter);
	IL2CPP::Callback::OnUpdate::Add(SetSeason);
	IL2CPP::Callback::OnUpdate::Add(SpawnItem);
	IL2CPP::Callback::OnUpdate::Add(ShotgunFire);
	IL2CPP::Callback::OnUpdate::Add(RevolverFire);
}

void GetLobbyInfo()
{
	Globals::CoopLobbyManager = reinterpret_cast<Unity::CObject*>(IL2CPP::Class::Find("Sons.Multiplayer.CoopLobbyManager"));

	if (Globals::CoopLobbyManager)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] CoopLobbyManager object found: " << Globals::CoopLobbyManager << '\n';

		void* GetActiveInstance = IL2CPP::Class::Utils::GetMethodPointer("Sons.Multiplayer.CoopLobbyManager", "GetActiveInstance");
		Globals::CoopLobby = Globals::CoopLobbyManager->CallMethod<Unity::CObject*>(GetActiveInstance);

		if (Globals::CoopLobby)
		{
			std::cout << "[+] CoopLobby object found: " << Globals::CoopLobby << '\n';
			return;
		}
	}
	SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
	printf("Couldn't find lobby info");
}
