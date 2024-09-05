// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchaGameMode.generated.h"

UCLASS(minimalapi)
class AMatchaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMatchaGameMode();

	virtual void BeginPlay() override;

};



