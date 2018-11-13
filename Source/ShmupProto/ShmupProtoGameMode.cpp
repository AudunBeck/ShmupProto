// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ShmupProtoGameMode.h"
#include "ShmupProtoPlayerController.h"
#include "ShmupProtoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShmupProtoGameMode::AShmupProtoGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AShmupProtoPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}