// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UEPROCEDURALMESH_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACustomPlayerController();

protected:
	virtual void BeginPlay() override;
	
	bool PerformLineTrace(FHitResult* Out);
	void TryBuildBlock();
	void TryRemoveBlock();
};
