#include "Input/AuraInputConfig.h"

UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& GameplayTag, bool bLogNotFound) const
{
	for (const auto& AuraInputAction : AbilityInputActions)
	{
		if (AuraInputAction.InputTag == GameplayTag)
		{
			return AuraInputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action not found for %s"), *GameplayTag.ToString())
	}
	return nullptr;
}
