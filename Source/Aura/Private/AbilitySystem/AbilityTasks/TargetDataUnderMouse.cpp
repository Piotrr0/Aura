#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	if (const bool bIsLocallyController = Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Target, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;

	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
	EndTask();
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
