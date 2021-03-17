// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

#include "CubeWorld.h"
#include "Macros.h"
#include "Kismet/GameplayStatics.h"

ACustomPlayerController::ACustomPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACustomPlayerController::BeginPlay()
{
	InputComponent->BindAction("Left click", IE_Pressed, this, &ACustomPlayerController::TryBuildBlock);
	InputComponent->BindAction("Right click", IE_Pressed, this, &ACustomPlayerController::TryRemoveBlock);
}

bool ACustomPlayerController::PerformLineTrace(FHitResult* Out)
{
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	FHitResult LineTraceHit;
	FVector StartTrace = camManager->GetCameraLocation() + camManager->GetActorForwardVector() * 100.f;
	FVector EndTrace = camManager->GetActorForwardVector() * 300.f + StartTrace;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(LineTraceHit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, CQP);

	AActor* HitActor = LineTraceHit.GetActor();
	if (HitActor != nullptr && Cast<ACubeWorld>(HitActor) != nullptr)
	{
		*Out = LineTraceHit;
		return true;
	}

	return false;
}

void ACustomPlayerController::TryBuildBlock()
{
	FHitResult hitResult;
	if (PerformLineTrace(&hitResult))
	{
		ACubeWorld* Chunk = Cast<ACubeWorld>(hitResult.GetActor());
		if (Chunk != nullptr)
		{
			Chunk->AddCube(hitResult.Location + hitResult.ImpactNormal * 50.f, 1);
		}
	}
}

void ACustomPlayerController::TryRemoveBlock()
{
	FHitResult hitResult;
	if (PerformLineTrace(&hitResult))
	{
		ACubeWorld* Chunk = Cast<ACubeWorld>(hitResult.GetActor());
		if (Chunk != nullptr)
		{
			Chunk->DeleteCube(hitResult.Location - hitResult.ImpactNormal * 10.f);
		}
	}
}
