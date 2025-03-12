#include "AbilitySystem/Abilities/SparkAbility.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

void USparkAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USparkAbility::SpawnSparks(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)
		Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = 5;
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		FVector SpawnLocation;
		if (!StickToGroundLocation(SocketLocation, SpawnLocation, 50.f))
		{
			SpawnLocation = SocketLocation;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(AuraProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Projectile->FinishSpawning(SpawnTransform);
	}
}

bool USparkAbility::StickToGroundLocation(const FVector& Location, FVector& OutGroundLocation, float Offset) const
{
	const FVector Start = Location + FVector(0.f, 0.f, 500.f);
	const FVector End = Location + FVector(0.f, 0.f, -500.f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		OutGroundLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, Offset);
		return true;
	}

	return false;
}