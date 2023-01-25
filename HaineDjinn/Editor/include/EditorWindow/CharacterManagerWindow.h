#pragma once
#include "ImGuiIncluder.h"


class CharacterManager
{
	public:
		
		static bool canOpenEntityFolder;
		static char buf[255];
		static std::string entityName;
		static bool isAPlayableCharacter;
		static bool isHeAgressive;
		static bool isItAtDistance;
		static  int life;
		static int damagePerAttack;
		static float hitPerSecond;
		static float detectionRange;
			   
		static void RunCharacterManagerWindow(const std::string& pathAssets, bool& isCreatingAEntityFile);
		static void CreateNewEntityFile(const std::string& pathAssets, bool& isCreatingAEntityFile);
};