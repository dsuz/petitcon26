// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/Character.h"
#include "DodgeBall.h"
#include "DodgeBallPlayer.generated.h"

UCLASS()
class PETITCON26_API ADodgeBallPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ADodgeBallPlayer();

protected:
	virtual void BeginPlay() override;

	/** Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MouseLookAction;
	UFUNCTION(BlueprintCallable)
	void Look(const FVector2D Value);
	void LimitAimAngle();
	
	/** Camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UGameplayCameraComponent> GameplayCameraComponent;
	
	/** Gameplay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADodgeBall> DodgeBallActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> Muzzle;
	UFUNCTION(BlueprintCallable)
	void Shoot();
	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimLimitMin = -45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimLimitMax = 45;
};
