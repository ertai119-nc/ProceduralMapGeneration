// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSpriteComponent.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "WangTileActor.generated.h"

class Tile
{
private:
    TArray<char> sideCodes_;
	TWeakObjectPtr<UPaperSprite> sprite_;
public:
    static const int NORTH = 0;
    static const int EAST = 1;
    static const int SOUTH = 2;
    static const int WEST = 3;

	Tile(const Tile&);
	Tile(UPaperSprite* sprite);

    char getCodeAtSide(int);
    bool hasCodeAtSide(char, int);
	TWeakObjectPtr<UPaperSprite> GetImage() { return sprite_; }
    bool isSame(Tile*);
};

UCLASS()
class TESTMAP_API AWangTileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWangTileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	void InitializeMap();
	void UninitialzeMap();
	void GenerateMap();
	Tile GetRandom();
	void DrawTiles();

public:

    UPROPERTY(EditAnywhere)
    bool Generate;

	UPROPERTY(EditAnywhere)
	int32 Width = 10;

    UPROPERTY(EditAnywhere)
    int32 Height = 10;
	
	UPROPERTY(EditAnywhere)
	TArray<UPaperSprite*> tileSet_; 

	TArray<Tile> tiles_;
	TArray<TArray<Tile>> mapTiles_;

	UPROPERTY(EditAnywhere)
	TArray<class UPaperSpriteComponent*> spawnedSpriteComps_;

protected:
};
