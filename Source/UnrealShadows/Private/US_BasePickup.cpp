#include "US_BasePickup.h"
#include "US_Character.h"
#include "Components/SphereComponent.h"

AUS_BasePickup::AUS_BasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	RootComponent = SphereCollision;
	
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(200.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
void AUS_BasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUS_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUS_BasePickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AUS_BasePickup::Pickup_Implementation(class AUS_Character* OwningCharacter)
{
}

