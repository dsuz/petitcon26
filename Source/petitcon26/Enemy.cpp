#include "Enemy.h"

#include "GameFramework/PawnMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCollisionProfileName("BlockAllDynamic");
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	LeftShoulder = CreateDefaultSubobject<USphereComponent>("LeftShoulder");
	LeftShoulder->SetupAttachment(SkeletalMeshComponent);
	RightShoulder = CreateDefaultSubobject<USphereComponent>("RightShoulder");
	RightShoulder->SetupAttachment(SkeletalMeshComponent);
	LeftFoot = CreateDefaultSubobject<USphereComponent>("LeftFoot");
	LeftFoot->SetupAttachment(SkeletalMeshComponent);
	RightFoot = CreateDefaultSubobject<USphereComponent>("RightFoot");
	RightFoot->SetupAttachment(SkeletalMeshComponent);
}

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("Enemy hit by %s"), *(OtherActor->GetName()));
	ActivateRagdoll();
	BlowAwayRagdoll(this->GetActorForwardVector() * -1, 1000);
}

void AEnemy::ActivateRagdoll()
{	
	if (!SkeletalMeshComponent || !SkeletalMeshComponent->GetPhysicsAsset())
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh or PhysicsAsset missing!"));
		return;
	}

	// Disable character movement and controller possession 
	//GetCharacterMovement()->DisableMovement();
	if (UPawnMovementComponent* MovementComp = FindComponentByClass<UPawnMovementComponent>())
	{
		MovementComp->StopMovementImmediately();
		MovementComp->SetComponentTickEnabled(false);
		MovementComp->SetUpdatedComponent(nullptr); // Rootの更新対象から外す
	}
	DetachFromControllerPendingDestroy();

	// Enable physics simulation on all bodies to start ragdoll
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
	SkeletalMeshComponent->SetAllBodiesSimulatePhysics(true);
	SkeletalMeshComponent->SetSimulatePhysics(true);
	SkeletalMeshComponent->WakeAllRigidBodies();
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::BlowAwayRagdoll(FVector Direction, float Strength)
{
	if (!SkeletalMeshComponent || !SkeletalMeshComponent->IsSimulatingPhysics())
	{
		return;
	}

	const FVector Impulse = Direction.GetSafeNormal() * Strength;

	// ルートボーンだけでなく全ボディに適用する
	for (FBodyInstance* Body : SkeletalMeshComponent->Bodies)
	{
		if (Body && Body->IsInstanceSimulatingPhysics())
		{
			Body->AddImpulse(Impulse, /*bVelChange=*/true);
		}
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
