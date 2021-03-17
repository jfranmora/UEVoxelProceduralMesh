// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeWorldGameMode.h"

#include "CustomPlayerController.h"

ACubeWorldGameMode::ACubeWorldGameMode()
{
	PlayerControllerClass = ACustomPlayerController::StaticClass();
}
