#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AuraGameMode.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

};
