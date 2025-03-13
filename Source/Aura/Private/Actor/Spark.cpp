#include "Actor/Spark.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

ASpark::ASpark()
{
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ASpark::BeginPlay()
{
	Super::BeginPlay();
}

void ASpark::OnHit()
{
	Super::OnHit();

	ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ASpark::OnProjectileBounce);
}

void ASpark::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	if (!bHit)
	{
		OnHit();
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if (UntargetableActors.Contains(TargetASC->GetAvatarActor())) return;

			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;

			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			HandlePierceBehaviour();
		}
	}
}

void ASpark::HandlePierceBehaviour()
{
	Super::HandlePierceBehaviour();
}

void ASpark::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	FVector NewVelocity = ImpactVelocity;
	NewVelocity.Z = 0.0f;

	ProjectileMovement->Velocity = NewVelocity;
}
