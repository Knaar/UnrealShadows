#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneDesc.h"
#include "GameFramework/Actor.h"
#include "US_BasePickup.generated.h"

UCLASS()
class UNREALSHADOWS_API AUS_BasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AUS_BasePickup();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",  meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

protected:
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup", meta=(DisplayName="Pickup"))
	void Pickup(class AUS_Character* OwningCharacter);

public:
	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

};
