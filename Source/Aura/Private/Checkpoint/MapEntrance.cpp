#include "Checkpoint/MapEntrance.h"
#include "Interaction/PlayerInterface.h"
#include "Game/AuraGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::HighlightActor_Implementation()
{
	Super::HighlightActor_Implementation();
}

void AMapEntrance::UnHighlightActor_Implementation()
{
	Super::UnHighlightActor_Implementation();
}

void AMapEntrance::LoadActor_Implementation()
{
	// Nothing
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (AAuraGameMode* AuraGM = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}

		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
