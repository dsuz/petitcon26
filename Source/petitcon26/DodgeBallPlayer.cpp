#include "DodgeBallPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

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
	StaticDummyDodgeBall = CreateDefaultSubobject<UStaticMeshComponent>("StaticDummyDodgeBall");
	StaticDummyDodgeBall->SetupAttachment(this->GetMesh(), SocketNameForBallHandling);
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
	LimitAimAngle();
}

void ADodgeBallPlayer::LimitAimAngle()
{
	auto PlayerController = this->GetController();
	auto Rotation = PlayerController->GetControlRotation();
	auto ClampedPitchAngle = UKismetMathLibrary::ClampAngle(Rotation.Pitch, AimLimitMin, AimLimitMax);
	Rotation.Pitch = ClampedPitchAngle;
	PlayerController->SetControlRotation(Rotation);
}


void ADodgeBallPlayer::Shoot(FVector Location)
{
	auto DodgeBall = GetWorld()->SpawnActor<ADodgeBall>(DodgeBallActor, Location, GetControlRotation());
	ShowDummyBall(false);
	DodgeBall->Shoot();
	
	// ディレイ
	FTimerHandle Handle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this, DodgeBall]()
	{
		// 数秒後の処理
		DodgeBall->Destroy();
		ShowDummyBall(true);
	});
	
	GetWorldTimerManager().SetTimer(Handle, TimerDel, 2.0f, false);
}

void ADodgeBallPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADodgeBallPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// 手に持ったボールの表示/非表示を切り替える（flg = true => 表示する, flg = false => 消す）
void ADodgeBallPlayer::ShowDummyBall(bool flg)
{
	StaticDummyDodgeBall->SetVisibility(flg);
}