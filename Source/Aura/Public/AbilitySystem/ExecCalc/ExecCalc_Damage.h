#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Interaction/CombatInterface.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:

    float CalculateDamageByType(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        const FAggregatorEvaluateParameters& EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const;

    float HandleBlockChance(float Damage,
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        const FAggregatorEvaluateParameters& EvaluationParameters,
        FGameplayEffectContextHandle& EffectContextHandle) const;

    float ApplyArmorReduction(float Damage,
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        const FAggregatorEvaluateParameters& EvaluationParameters,
        const UAbilitySystemComponent* SourceASC,
        const int32 SourceLevel,
        const int32 TargetLevel) const;

    float HandleCriticalHit(float Damage,
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        const FAggregatorEvaluateParameters& EvaluationParameters,
        FGameplayEffectContextHandle& EffectContextHandle,
        const UAbilitySystemComponent* SourceASC,
        const int32 SourceLevel,
        const int32 TargetLevel) const;

};