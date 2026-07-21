#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DodgeBall.generated.h"

UCLASS()
class PETITCON26_API ADodgeBall : public AActor
{
	GENERATED_BODY()
	
public:	
	ADodgeBall();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Gameplay */
	UFUNCTION(BlueprintCallable)
	void Shoot();
};
