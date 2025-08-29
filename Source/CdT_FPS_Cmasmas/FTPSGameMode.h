// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FTPSGameMode.generated.h"

class AFTPSCharcter;

UCLASS()
class CDT_FPS_CMASMAS_API AFTPSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void GameOver(AFTPSCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(AFTPSCharacter* Character);
};
