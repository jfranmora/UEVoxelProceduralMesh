// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeWorld.h"

#include "Macros.h"

const float GUE_Units = 100.f;
const int GChunk_Size = 8;

// Sets default values
ACubeWorld::ACubeWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");
	ProceduralMesh->bUseAsyncCooking = true;

	SetRootComponent(ProceduralMesh);
}

void ACubeWorld::RegenerateMesh()
{
	ClearMeshData();

	for (int x = 0; x < GChunk_Size; x++)
	{
		for (int y = 0; y < GChunk_Size; y++)
		{
			for (int z = 0; z < GChunk_Size; z++)
			{
				if (World[x][y][z] != 0)
				{
					AddCubeToMesh(x, y, z, World[x][y][z]);
				}
			}
		}
	}

	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(),
	                                  TArray<FProcMeshTangent>(), true);
}

void ACubeWorld::ClearMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
	Normals.Empty();
}

void ACubeWorld::AddCube(const FVector WorldPosition, const int index)
{
	IndexedPosition pos = WorldPositionToIndexed(WorldPosition);

	World[pos.x][pos.y][pos.z] = index;
	RegenerateMesh();

	// DEBUG_MESSAGE("Add cube (%i): %s", index, *(WorldPosition.ToString()))
	// DEBUG_MESSAGE("Estimated position (%i, %i, %i)", pos.x, pos.y, pos.z);
}

void ACubeWorld::DeleteCube(const FVector WorldPosition)
{
	IndexedPosition pos = WorldPositionToIndexed(WorldPosition);

	World[pos.x][pos.y][pos.z] = 0;
	RegenerateMesh();

	// DEBUG_MESSAGE("Delete cube: %s", *(WorldPosition.ToString()))
	// DEBUG_MESSAGE("Estimated position (%i, %i, %i)", pos.x, pos.y, pos.z);
}

// Called when the game starts or when spawned
void ACubeWorld::BeginPlay()
{
	Super::BeginPlay();

	ProceduralMesh->SetMaterial(0, Material);

	// Generate world
	for (size_t x = 0; x < GChunk_Size; x++)
	{
		for (size_t y = 0; y < GChunk_Size; y++)
		{
			for (int z = 0; z < GChunk_Size; z++)
			{
				World[x][y][z] = z < GetHeightForPosition(x, y);
			}
		}
	}

	RegenerateMesh();
}

void ACubeWorld::Destroyed()
{
	ClearMeshData();
}

float ACubeWorld::GetHeightForPosition(int x, int y) const
{
	const float freqX = .13f;
	const float freqY = .12f;

	FVector Location = GetActorLocation();
	FVector2D NoiseLocation = FVector2D(x * freqX + Location.X, y * freqY + Location.Y);
	return (FMath::PerlinNoise2D(NoiseLocation) / 2.f + .5f) * GChunk_Size / 2.f;
}

void ACubeWorld::AddCubeToMesh(int32 x, int32 y, int32 z, int index)
{
	const bool bHideForwardFace = /*y == 0 || */ (y > 0 && World[x][y - 1][z] != 0);
	const bool bHideBackFace = /*y == (GChunk_Size - 1) || */ (y < GChunk_Size - 1 && World[x][y + 1][z] != 0);
	const bool bHideRightFace = /*x == (GChunk_Size - 1) || */ (x < GChunk_Size - 1 && World[x + 1][y][z] != 0);
	const bool bHideLeftFace = /*x == 0 ||*/ (x > 0 && World[x - 1][y][z] != 0);
	const bool bHideTopFace = /*z == (GChunk_Size - 1) || */ (z < GChunk_Size - 1 && World[x][y][z + 1] != 0);
	const bool bHideBottomFace = z == 0 || (z > 0 && World[x][y][z - 1] != 0);

	FVector v0 = GUE_Units * FVector(y, x, z);
	FVector v1 = v0 + GUE_Units * FVector::UpVector;
	FVector v2 = v1 + GUE_Units * FVector::RightVector;
	FVector v3 = v0 + GUE_Units * FVector::RightVector;
	FVector v4 = v0 + GUE_Units * FVector::ForwardVector;
	FVector v5 = v1 + GUE_Units * FVector::ForwardVector;
	FVector v6 = v2 + GUE_Units * FVector::ForwardVector;
	FVector v7 = v3 + GUE_Units * FVector::ForwardVector;

	if (!bHideForwardFace) AddFace(v3, v2, v1, v0, FVector::BackwardVector);
	if (!bHideBackFace) AddFace(v4, v5, v6, v7, FVector::ForwardVector);
	if (!bHideLeftFace) AddFace(v0, v1, v5, v4, FVector::LeftVector);
	if (!bHideRightFace) AddFace(v7, v6, v2, v3, FVector::RightVector);
	if (!bHideTopFace) AddFace(v2, v6, v5, v1, FVector::UpVector);
	if (!bHideBottomFace) AddFace(v7, v3, v0, v4, FVector::DownVector);
}

void ACubeWorld::AddTriangleIndexes(int v0, int v1, int v2, int offset)
{
	Triangles.Add(offset + v0);
	Triangles.Add(offset + v1);
	Triangles.Add(offset + v2);
}

void ACubeWorld::AddFace(FVector v0, FVector v1, FVector v2, FVector v3, FVector normal)
{
	int VertexOffset = Vertices.Num();

	Vertices.Add(v0);
	Vertices.Add(v1);
	Vertices.Add(v2);
	Vertices.Add(v3);

	Normals.Add(normal);
	Normals.Add(normal);
	Normals.Add(normal);
	Normals.Add(normal);

	UVs.Add(FVector2D(1.f, 0.f));
	UVs.Add(FVector2D(1.f, 1.f));
	UVs.Add(FVector2D(0.f, 1.f));
	UVs.Add(FVector2D(0.f, 0.f));

	AddTriangleIndexes(0, 1, 2, VertexOffset);
	AddTriangleIndexes(0, 2, 3, VertexOffset);
}

IndexedPosition ACubeWorld::WorldPositionToIndexed(FVector WorldPosition) const
{
	IndexedPosition Result;

	const FVector LocalLocation = WorldPosition - GetActorLocation();
	
	Result.x = LocalLocation.Y / 100.0f;
	Result.y = LocalLocation.X / 100.0f;
	Result.z = LocalLocation.Z / 100.0f;

	return Result;
}
