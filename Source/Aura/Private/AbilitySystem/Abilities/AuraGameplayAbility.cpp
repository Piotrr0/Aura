#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString();
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString();
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString();
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const 
{
	float ManaCost = 0;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (auto Mod : CostEffect->Modifiers)
		{
			if(Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}

	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		Cooldown = CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}

	return Cooldown;
}
