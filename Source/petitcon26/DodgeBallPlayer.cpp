#include "DodgeBallPlayer.h"
#include "Kismet/KismetMathLibrary.h"

ADodgeBallPlayer::ADodgeBallPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Gameplay Camera Setup
	GameplayCameraComponent = CreateDefaultSubobject<UGameplayCameraComponent>("Camera");
	GameplayCameraComponent->SetupAttachment(RootComponent);
	
	// Gameplay
	Muzzle = CreateDefaultSubobject<USceneComponent>("Muzzle");
	Muzzle->SetupAttachment(RootComponent);
}

void ADodgeBallPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Initialize Enhanced Input Subsystem
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ADodgeBallPlayer::Look(const FVector2D Value)
{
	AddControllerPitchInput(Value.Y);
	AddControllerYawInput(Value.X);
	//LimitAimAngle();
}

void ADodgeBallPlayer::LimitAimAngle()
{
	auto PlayerController = this->GetController();
	auto Rotation = PlayerController->GetControlRotation();
	auto ClampedPitchAngle = UKismetMathLibrary::ClampAngle(Rotation.Pitch, AimLimitMin, AimLimitMax);
	Rotation.Pitch = ClampedPitchAngle;
	PlayerController->SetControlRotation(Rotation);
}

void ADodgeBallPlayer::Shoot()
{
	auto DodgeBall = GetWorld()->SpawnActor<ADodgeBall>(DodgeBallActor, Muzzle->GetComponentLocation(), Muzzle->GetComponentRotation());
	DodgeBall->Shoot();
}

void ADodgeBallPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADodgeBallPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
