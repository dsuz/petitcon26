#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy.generated.h"

UCLASS()
class PETITCON26_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	AEnemy();

protected:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> LeftShoulder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> RightShoulder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> LeftFoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> RightFoot;
	UFUNCTION(BlueprintCallable)
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	UFUNCTION(BlueprintCallable)
	void ActivateRagdoll();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
