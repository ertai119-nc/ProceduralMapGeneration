// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomMapGenActor.generated.h"

USTRUCT()
struct FCellInfo
{
    GENERATED_BODY()

    FVector2D pos;
    bool dead;

    UPROPERTY()
    UStaticMeshComponent* meshComp;
};

UCLASS()
class TESTMAP_API ARandomMapGenActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomMapGenActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

    void InitializeMap();
    void DeInitializeMap();

    void SimulateMap();

    void DrawCells();

    int32 countAliveNeighbours(int x, int y);

    UPROPERTY(EditAnywhere)
        UStaticMesh* FloorMesh;

    UPROPERTY(EditAnywhere)
        UStaticMesh* WallMesh;

    UPROPERTY(EditAnywhere)
    bool Generate;

    UPROPERTY(EditAnywhere)
        bool Simulate;

	UPROPERTY(EditAnywhere)
	int32 Width = 30;

    UPROPERTY(EditAnywhere)
        int32 Height = 20;

    UPROPERTY(EditAnywhere)
        float SpawnProbability = 30.f;

    UPROPERTY(EditAnywhere)
        int32 DeathCount = 3;

    UPROPERTY(EditAnywhere)
        int32 BirthCount = 4;

protected:

    UPROPERTY()
    TArray<UStaticMeshComponent*> meshs_;
    UPROPERTY()
        TArray<UStaticMeshComponent*> borderMeshs_;

    TArray<TArray<int32>> maps_;
};
