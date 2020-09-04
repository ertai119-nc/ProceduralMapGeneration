// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMapGenActor.h"

// Sets default values
ARandomMapGenActor::ARandomMapGenActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARandomMapGenActor::BeginPlay()
{
	Super::BeginPlay();
	
}

#if WITH_EDITOR
void ARandomMapGenActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.MemberProperty)
    {
        const FName MemberPropName = PropertyChangedEvent.MemberProperty->GetFName();

        if (MemberPropName == GET_MEMBER_NAME_CHECKED(ARandomMapGenActor, Generate))
        {
            if (Generate)
            {
                InitializeMap();
                DrawCells();
            }
            else
            {
                DeInitializeMap();
            }
        }

        if (MemberPropName == GET_MEMBER_NAME_CHECKED(ARandomMapGenActor, Simulate))
        {
            if (Simulate)
            {
                SimulateMap();
                DrawCells();
            }
        }
    }

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called every frame
void ARandomMapGenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ARandomMapGenActor::countAliveNeighbours(int x, int y)
{
    int count = 0;
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            int neighbour_x = x + i;
            int neighbour_y = y + j;

            if (i == 0 && j == 0)
            {
                //do nothing as its our target cell
            }
            //if out of bounds, obviously don't check it, but add one anyway as it counts as an alive neighbour
            else if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= maps_[0].Num() || neighbour_y >= maps_.Num())
            {
                count += 1;
            }
            else if (maps_[neighbour_y][neighbour_x] == 1)
            {
                count += 1;
            }
        }
    }
    return count;
}

void ARandomMapGenActor::SimulateMap()
{
    TArray<TArray<int32> > newMap;
    //newMap.resize(height, std::vector<int>(width));

    int birthLimit = 4;
    int deathLimit = 3;

    //iterates through every tile in the map and decides if needs to be born, die, or remain unchanged
    for (int h = 0; h < maps_.Num(); h++)
    {
        newMap.Add(TArray<int32>());

        for (int w = 0; w < maps_[0].Num(); w++)
        {
            if (h == 0 || w == 0 || h == Height - 1 || w == Width - 1)
            {
                newMap[h].Add(0);
                continue;
            }

            int newVal = countAliveNeighbours(w, h);
            if (maps_[h][w])
            {
                if (newVal < DeathCount)
                {
                    newMap[h].Add(0);// [w] = 0;
                }
                else
                {
                    newMap[h].Add(1);// [w] = 1;
                }
            }
            else
            {
                if (newVal > BirthCount)
                {
                    newMap[h].Add(1);// [w] = 1;
                }
                else
                {
                    newMap[h].Add(0);// [w] = 0;
                }
            }
        }
    }

    maps_ = newMap;
    //return newMap;
}

void ARandomMapGenActor::InitializeMap()
{
    const auto& originLoc = GetActorLocation();

    for (int32 y = 0 ; y < Height; y++)
    {
        for (int32 x = 0; x < Width; x++)
        {
            FString name = FString::Format(TEXT("cell_{0}"), { y * Width + x });
            auto meshComp = NewObject<UStaticMeshComponent>(this, *name);
            if (meshComp)
            {
                meshComp->RegisterComponentWithWorld(GetWorld());
                
                FVector loc(originLoc.X + (x * 100), originLoc.Y + (y * 100), originLoc.Z);
                meshComp->SetWorldLocation(loc);

                meshs_.Add(meshComp);
            }
        }
    }

    TArray<TArray<int32> > map; //create map to return
    
    for (int h = 0; h < Height; h++)
    {
        map.Add(TArray<int32>());

        for (int w = 0; w < Width; w++) //initialising values here
        {
            if (h == 0 || w == 0 || h == Height - 1 || w == Width - 1)
            {
                map[h].Add(0);// [w] = 1;
            }
            else if (FMath::RandRange(0.f, 100.f) < SpawnProbability) //it has a chance of being a wall or not
            {
                map[h].Add(1);// [w] = 1;
            }
            else
            {
                map[h].Add(0);// [w] = 0;
            }
        }
    }
    //return our new map
    maps_ = map;
}

void ARandomMapGenActor::DeInitializeMap()
{
    for (auto& mesh : meshs_)
    {
        if (mesh)
        {
            mesh->DestroyComponent();
        }
    }

    meshs_.Empty();
}

void ARandomMapGenActor::DrawCells()
{
    for (int h = 0; h < Height; h++)
    {
        for (int w = 0; w < Width; w++) //initialising values here
        {
            if (maps_[h][w])
            {
                meshs_[h * Width + w]->SetStaticMesh(FloorMesh);
            }
            else
            {
                meshs_[h * Width + w]->SetStaticMesh(WallMesh);
            }
        }
    }
}
