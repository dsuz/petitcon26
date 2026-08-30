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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundWave> AttackSound;	// ボールが当たった時の音
	UFUNCTION(BlueprintCallable)
	void ActivateRagdoll();
	void BlowAwayRagdoll(FVector Direction, float Strength);
	void BlowAwayRagdoll(FVector ExplosionOrigin, float Strength, float Radius);
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
