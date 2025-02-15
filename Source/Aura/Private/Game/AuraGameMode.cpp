#include "Game/AuraGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Game/AuraGameInstance.h"

void AAuraGameMode::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	DeleteSlot(LoadSlot->LoadSlotName, SlotIndex);
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	LoadScreenSaveGame->SaveSlotStatus = Taken;

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
}

void AAuraGameMode::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameMode::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->LoadSlotName;
	const int32 SlotIndex = Slot->SlotIndex;

	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* AAuraGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActors = Actors[0];
		for (AActor* Actor : Actors)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
			if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
			{
				SelectedActors = PlayerStart;
				break;
			}
		}
		return SelectedActors;
	}
	return nullptr;
}

void AAuraGameMode::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}

ULoadScreenSaveGame* AAuraGameMode::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}
