#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "SparkAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API USparkAbility : public UAuraProjectileSpell
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	UFUNCTION(BlueprintCallable)
	void SpawnSparks(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float MaxNumProjectiles = 5.f;

};
