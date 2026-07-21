#include "DodgeBall.h"

#include "Components/StaticMeshComponent.h"

ADodgeBall::ADodgeBall()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = MeshComponent;
}

void ADodgeBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADodgeBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADodgeBall::Shoot()
{
	UE_LOG(LogTemp, Log, TEXT("Shoot"));
	// TODO: 固定値なのであとで変える
	MeshComponent->AddImpulse(this->GetActorForwardVector() * 30000);
}

