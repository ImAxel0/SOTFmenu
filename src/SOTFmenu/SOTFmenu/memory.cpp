#include "Globals.h"
#include "config.h"
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
	Globals::LocalPlayer = Unity::GameObject::Find("LocalPlayer");
	
	if (!Globals::LocalPlayer)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find LocalPlayer\n");
		return;
	}

	SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
	std::cout << "[+] Found LocalPlayer at: " << Globals::LocalPlayer << '\n';

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
			printf("[-] Cannot find KnightVControlDefinition\n");
		}
		else
		{
			SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
			std::cout << "	-> Found KnightVControlDefinition: " << Globals::KnightVControlDefinition << '\n';
		}
	}

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
}