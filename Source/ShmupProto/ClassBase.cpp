// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassBase.h"
#include "ShmupProtoCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


// Sets default values
AClassBase::AClassBase()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


}

// Called when the game starts or when spawned
void AClassBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClassBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AClassBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void AClassBase::Shoot()
{
	UWorld* World = GetWorld();

	ABullet* tempBullet = World->SpawnActor<ABullet>(Bullet, GetActorLocation(), FRotator(0.f, CharacterOwner->getLookAt().Yaw, 0.f));
	tempBullet->Speed = BulletSpeed;
	tempBullet->LifeLength = BulletLife;
}

void AClassBase::SetOwner(class AShmupProtoCharacter * ToBeOwner)
{
	CharacterOwner = ToBeOwner;
}

void AClassBase::MoveForward(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		AddMovementInput(FVector(1.f, 0.f, 0.f), AxisValue);
		UE_LOG(LogTemp, Warning, TEXT("Character Forward %f"), AxisValue);
	}
}

void AClassBase::MoveRight(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		AddMovementInput(FVector(0.f, 1.f, 0.f), AxisValue);
		UE_LOG(LogTemp, Warning, TEXT("Character Right"));
	}
}

