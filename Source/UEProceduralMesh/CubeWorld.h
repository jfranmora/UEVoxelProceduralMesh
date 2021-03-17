// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "CubeWorld.generated.h"

struct IndexedPosition
{
	int x;
	int y;
	int z;
};

UCLASS()
class UEPROCEDURALMESH_API ACubeWorld : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	ACubeWorld();

	/* Regenerates the mesh using the world data */
	void RegenerateMesh();

	/* Clear the current mesh data */
	void ClearMeshData();

	/* Try add cube using as input the world position */
	void AddCube(const FVector WorldPosition, const int index);

	/* Delete a cube using the world position of the impact */
	void DeleteCube(const FVector WorldPosition);
	
protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	/* ------------------ */
	/* Exposed Properties */
	/* ------------------ */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* Material;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ProceduralMesh;

	/* ------------- */
	/* Internal Data */
	/* ------------- */
	int World[8][8][8];

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	/* ------------- */
	/* Map Generator */
	/* ------------- */

	float GetHeightForPosition(int x, int y) const;
	
	/* -------------- */
	/* Mesh Generator */
	/* -------------- */
	
	/* Generate cube using index */
	void AddCubeToMesh(int x, int y, int z, int index);

	/* Add triangle with vertex index */
	void AddTriangleIndexes(int v0, int v1, int v2, int offset = 0);
	void AddFace(FVector v0, FVector v1, FVector v2, FVector v3, FVector normal);

	IndexedPosition WorldPositionToIndexed(FVector WorldPosition) const;
};
