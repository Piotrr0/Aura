#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual int32 GetPlayerLevel() override;

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults", meta = (AllowPrivateAccess = true))
	int32 Level = 1;

	
};
