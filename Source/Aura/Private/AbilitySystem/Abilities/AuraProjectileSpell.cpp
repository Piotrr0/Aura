#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Actor/AuraProjectile.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthority(&ActivationInfo)) return;
}

void UAuraProjectileSpell::SpawnProjectile()
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(AuraProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->FinishSpawning(SpawnTransform);
	}
}
