// Fill out your copyright notice in the Description page of Project Settings.


#include "WangTileActor.h"

Tile::Tile(const Tile& tile)
    : sideCodes_(tile.sideCodes_)
    , sprite_(tile.sprite_)
{
}

Tile::Tile(UPaperSprite* sprite)
{
    sprite_ = sprite;
    FString name = sprite_->GetName();

    sideCodes_.Add(name[0]);
    sideCodes_.Add(name[1]);
    sideCodes_.Add(name[2]);
    sideCodes_.Add(name[3]);
}

char Tile::getCodeAtSide(int32 index)
{
    if (sideCodes_.IsValidIndex(index))
    {
        return sideCodes_[index];
    }

    return '0';
}

bool Tile::hasCodeAtSide(char code, int index)
{
    return code == getCodeAtSide(index);
}

bool Tile::isSame(Tile* other)
{
    if (other == nullptr)
    {
        return false;
    }

    for (int i = 0; i < 4; i++)
    {
        if (other->getCodeAtSide(i) != getCodeAtSide(i))
        {
            return false;
        }
    }

    return true;
}


// Sets default values
AWangTileActor::AWangTileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWangTileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

#if WITH_EDITOR
void AWangTileActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.MemberProperty)
    {
        const FName MemberPropName = PropertyChangedEvent.MemberProperty->GetFName();

        if (MemberPropName == GET_MEMBER_NAME_CHECKED(AWangTileActor, Generate))
        {
            if (Generate)
            {
                InitializeMap();
                GenerateMap();
                DrawTiles();
            }
            else
            {
                UninitialzeMap();
            }
        }
    }

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called every frame
void AWangTileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWangTileActor::InitializeMap()
{
    const auto& originLoc = GetActorLocation();

    for (int32 i = 0; i < Height; i++)
    {
        for (int32 j = 0; j < Width; j++)
        {
            FString name = FString::Format(TEXT("name_{0}"), { i * Width + j });
            auto spireComp = NewObject<UPaperSpriteComponent>(this, *name);
            if (spireComp == nullptr)
            {
                continue;
            }

            spireComp->RegisterComponentWithWorld(GetWorld());
            spireComp->SetVisibility(true);

            FVector loc(originLoc.X + (j * 40), originLoc.Y, originLoc.Z - (i * 40));
            spireComp->SetWorldLocation(loc);

            spawnedSpriteComps_.Add(spireComp);
        }
    }

    for (const auto& sprite : tileSet_)
    {
        tiles_.Add(Tile(sprite));
    }
}

void AWangTileActor::UninitialzeMap()
{
    for (auto& comp : spawnedSpriteComps_)
    {
        comp->DestroyComponent();
    }
    spawnedSpriteComps_.Empty();
    tiles_.Empty();
    mapTiles_.Empty();
}

Tile AWangTileActor::GetRandom()
{
    return tileSet_[FMath::RandRange(0, tileSet_.Num() - 1)];
}

void AWangTileActor::DrawTiles()
{
    for (int32 i = 0 ; i < Height; i++)
    {
        for (int32 j = 0; j < Width; j++)
        {
            int32 spriteCompIndex = i * Width + j;
            spawnedSpriteComps_[spriteCompIndex]->SetSprite(mapTiles_[i][j].GetImage().Get());
        }
    }
}

void AWangTileActor::GenerateMap()
{
    for (int32 i = 0; i < Height; i++)
    {
        TArray<Tile> row;
        Tile* last = nullptr;

        for (int32 j = 0; j < Width; j++)
        {
            if (j != 0)
            {
                last = &row[j - 1];
            }
            
            if (i == 0 && j == 0)
            {
                row.Add(GetRandom());
            }
            else if (i == 0)
            {
                Tile t = GetRandom();

                while (!t.hasCodeAtSide(row[j - 1].getCodeAtSide(Tile::EAST), Tile::WEST) || t.isSame(last))
                {
                    t = GetRandom();
                }

                row.Add(t);
            }
            else
            {
                Tile t = GetRandom();

                if (j == 0)
                {
                    while (!t.hasCodeAtSide(mapTiles_[i - 1][j].getCodeAtSide(Tile::SOUTH), Tile::NORTH) || t.isSame(last))
                    {
                        t = GetRandom();
                    }
                }
                else
                {
                    int tries = 0;
                    while ((!t.hasCodeAtSide(row[j - 1].getCodeAtSide(Tile::EAST), Tile::WEST) || !t.hasCodeAtSide(mapTiles_[i - 1][j].getCodeAtSide(Tile::SOUTH), Tile::NORTH) || t.isSame(last)) && tries < 10)
                    {
                        t = GetRandom();
                        //                        tries++; // Fail safe to avoid endless cycle if location not tile-able
                    }
                }

                row.Add(t);
            }
        }

        mapTiles_.Add(row);
    }
}
