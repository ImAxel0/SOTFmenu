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

	SetConsoleTextAttribute(Globals::Gui::hConsole, 9);
	printf("Required Things Found!\n\n");

	SetConsoleTextAttribute(Globals::Gui::hConsole, 15);
	printf("Searching Values...\n");

	// Health
	Globals::Stats::Health = Globals::Vitals->GetPropertyValue<Unity::CObject*>("Health");
	
	if (!Globals::Stats::Health)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find Health value\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found Health value: " << Globals::Stats::Health << '\n';
	}

	// Stamina
	Globals::Stats::Stamina = Globals::Vitals->GetPropertyValue<Unity::CObject*>("Stamina");

	if (!Globals::Stats::Stamina)
	{
		SetConsoleTextAttribute(Globals::Gui::hConsole, 4);
		printf("[-] Cannot find Stamina value\n");
	}
	else {
		SetConsoleTextAttribute(Globals::Gui::hConsole, 2);
		std::cout << "[+] Found Stamina value: " << Globals::Stats::Stamina << '\n';
	}
	
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

void setMemory()
{
	//God Mode
	if (Config::bHealth)
	{
		if (!Globals::Stats::Health) {
			return;
		}

		Globals::Stats::Health->SetMemberValue<float>("_currentValue", Config::Value::health);
		Globals::ToggleStats::toggleHealth = false;
	}
	else if (!Globals::ToggleStats::toggleHealth)
	{
		Globals::ToggleStats::toggleHealth = true;
		Globals::Stats::Health->SetMemberValue<float>("_currentValue", 100.0f);
	}
	
	// Stamina
	if (Config::bStamina)
	{
		if (!Globals::Stats::Stamina) {
			return;
		}

		Globals::Stats::Stamina->SetMemberValue<float>("_currentValue", Config::Value::stamina);
		Globals::ToggleStats::toggleStamina = false;
	}
	else if (!Globals::ToggleStats::toggleStamina)
	{
		Globals::ToggleStats::toggleStamina = true;
		Globals::Stats::Stamina->SetMemberValue<float>("_currentValue", 90.0f);
	}
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
}