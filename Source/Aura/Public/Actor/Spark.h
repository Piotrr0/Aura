#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "Spark.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ASpark : public AAuraProjectile
{
	GENERATED_BODY()

public:
	ASpark();

protected:

	virtual void BeginPlay() override;
	virtual void OnHit() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:


};
