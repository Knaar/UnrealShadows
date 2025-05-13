// Fill out your copyright notice in the Description page of Project Settings.


#include "US_Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera\CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components\CapsuleComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UObject/ConstructorHelpers.h"
#include "US_Utility.h"

using namespace US_Utility;

AUS_Character::AUS_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom=CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(60.0f, 96.0f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshAsset(TEXT("/Game/KayKit/Characters/rogue"));
	if (SkeletalMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;


}

void AUS_Character::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocallyControlled())return;
	APlayerController *PlayerController = Cast <APlayerController>(Controller);
	if ( !PlayerController) Erreturn("US_Character PlayerConrollerInvalid");
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem||!DefaultMappingContext) Erreturn("Encanced SUS or Default Mapping Context invalid");
	
	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	UpdateCharacterStats(1);
}

void AUS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUS_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUS_Character::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AUS_Character::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AUS_Character::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUS_Character::SprintEnd);
	}
}

void AUS_Character::Move(const struct FInputActionValue& Value)
{
	
	const auto MovementVector = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString::Printf(TEXT("MovementVector: %s"), *MovementVector.ToString()));
	if (Controller != nullptr)
	{
		const auto Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUS_Character::Look(const FInputActionValue& Value)
{
	const auto LookAxisVector = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("LookAxisVector: %s"), *LookAxisVector.ToString()));
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUS_Character::SprintStart(const FInputActionValue& Value)
{
	
	if (!GetCharacterStats()) Erreturn("SprintStart");
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->SprintSpeed;
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Blue, TEXT("SprintStart"));
}

void AUS_Character::SprintEnd(const FInputActionValue& Value)
{
	
	if (!GetCharacterStats()) Erreturn("SprintStart");
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Blue, TEXT("SprintEnd"));
}

void AUS_Character::Interact(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("Interact"));
}

void AUS_Character::OnRep_Score()
{
	
}

void AUS_Character::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUS_Character,Score);
}

void AUS_Character::UpdateCharacterStats(int32 CharacterLevel)
{
	if (!CharacterDataTable) return;
	TArray<FUS_CharacterStats*> CharacterStatsRows;
	CharacterDataTable->GetAllRows<FUS_CharacterStats>(TEXT("US_Character"), CharacterStatsRows);

	if(CharacterStatsRows.Num() <= 0) Erreturn("CharacterStatsRows.Num()=0");
	const int32 NewCharacterLevel = FMath::Clamp(CharacterLevel, 1, CharacterStatsRows.Num());
	CharacterStats = CharacterStatsRows[NewCharacterLevel - 1];
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
	
	
}

