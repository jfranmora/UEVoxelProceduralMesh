// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleProceduralMeshTest.h"

// Sets default values
ATriangleProceduralMeshTest::ATriangleProceduralMeshTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	CustomMesh->bUseAsyncCooking = true;

	SetRootComponent(CustomMesh);
}

// Called when the game starts or when spawned
void ATriangleProceduralMeshTest::BeginPlay()
{
	Super::BeginPlay();

	// Set material
	CustomMesh->SetMaterial(0, Material);

	FVector Offset = FVector::ZeroVector;
	
	// Create initial triangle
	FVector v2 = FVector(0, 0, 0);
	FVector v0 = v2 + FVector(0, 100, 0);
	FVector v1 = v2 + FVector(100, 0, 0);

	// AddTriangle(a, b, c);
	AddTriangle(v0, v1, v2);
	
	RefreshMesh();
}

// 1
// - -
// 2 - 0
void ATriangleProceduralMeshTest::AddTriangle(FVector v1, FVector v2, FVector v3)
{
	const int VertexIndex = Vertices.Num();

	Vertices.Add(v1);
	Vertices.Add(v2);
	Vertices.Add(v3);

	Triangles.Add(VertexIndex);
	Triangles.Add(VertexIndex + 1);
	Triangles.Add(VertexIndex + 2);
}

// 2 - 1
// -   -
// 3 - 0
void ATriangleProceduralMeshTest::AddQuad(FVector v0, FVector v1, FVector v2, FVector v3)
{
	const int VertexIndex = Vertices.Num();

	Vertices.Add(v0);
	Vertices.Add(v1);
	Vertices.Add(v2);
	Vertices.Add(v3);

	// A - (0, 1, 2)
	Triangles.Add(VertexIndex + 0);
	Triangles.Add(VertexIndex + 1);
	Triangles.Add(VertexIndex + 2);

	// B - (0, 2, 3)
	Triangles.Add(VertexIndex + 0);
	Triangles.Add(VertexIndex + 2);
	Triangles.Add(VertexIndex + 3);
}

void ATriangleProceduralMeshTest::RefreshMesh()
{
	if (CustomMesh->GetProcMeshSection(0) != nullptr)
	{
		CustomMesh->ClearMeshSection(0);
	}

	CustomMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(),
	                              TArray<FProcMeshTangent>(), false);
}

// Called every frame
void ATriangleProceduralMeshTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
