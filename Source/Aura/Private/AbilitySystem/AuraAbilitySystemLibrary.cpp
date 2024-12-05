#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Game/AuraGameMode.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass ChracterClass, UAbilitySystemComponent* ASC, float Level)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	const FCharacterClassDefaultInfo ClassDefaultsInfo = ClassInfo->GetClassDefaultInfo(ChracterClass);

	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryEffectSpec = ASC->MakeOutgoingSpec(ClassDefaultsInfo.PrimaryAttributes, Level, PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpec.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpec.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalEffectSpec = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpec.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	if (const AAuraGameMode* AuraGameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);

		for (auto& Ability : ClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
			ASC->GiveAbility(Ability);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameMode* AuraGameMode = Cast<AAuraGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}