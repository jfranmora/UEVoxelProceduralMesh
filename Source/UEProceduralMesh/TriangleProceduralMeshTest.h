// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

#include "TriangleProceduralMeshTest.generated.h"

UCLASS()
class UEPROCEDURALMESH_API ATriangleProceduralMeshTest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATriangleProceduralMeshTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UMaterial* Material;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* CustomMesh;

	/* The vertices of the mesh */
	TArray<FVector> Vertices;

	/* The triangles of the mesh */
	TArray<int32> Triangles;

	void AddTriangle(FVector v1, FVector v2, FVector v3);
	void AddQuad(FVector v0, FVector v1, FVector v2, FVector v3);
	void RefreshMesh();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
