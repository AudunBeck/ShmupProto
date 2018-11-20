// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ShmupProtoCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Bullet.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AShmupProtoCharacter::AShmupProtoCharacter()
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

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AShmupProtoCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);

			LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TraceHitResult.ImpactPoint);
			CurrentAdventurers[0]->SetActorRotation(FRotator(GetControlRotation().Pitch, LookAt.Yaw, GetControlRotation().Roll));
			CurrentAdventurers[1]->SetActorRotation(FRotator(GetControlRotation().Pitch, LookAt.Yaw, GetControlRotation().Roll));
			CurrentAdventurers[2]->SetActorRotation(FRotator(GetControlRotation().Pitch, LookAt.Yaw, GetControlRotation().Roll));
		}
	}

	if (frontLiner > 2)
		frontLiner = 0;
	if (frontLiner < 0)
		frontLiner = 2;
	FVector MainAdvLocation = CurrentAdventurers[frontLiner]->GetActorLocation();
	FVector MainAdvForward = CurrentAdventurers[frontLiner]->GetActorForwardVector();

	SetActorLocation(MainAdvLocation);
	//Disse to linjene under her plasserer de to andre karakterene bak hoved karakteren, prøver å ha de stående i et triangle, med den fremste først.
//men får ikke helt til. Kan du ordne noe annet? Trenger ikke bruke det under. 
// Om du ogsaa vil kan du se over Swap() og fikse den til aa bytte plass mellom alle 3.
	if (frontLiner == 0)
	{
		CurrentAdventurers[1]->SetActorLocation(MainAdvLocation + (FVector(-100.f, -100.f, 0.f)));
		CurrentAdventurers[2]->SetActorLocation(MainAdvLocation + (FVector(-100.f, 100.f, 0.f)));
	}
	if (frontLiner == 1)
	{
		CurrentAdventurers[2]->SetActorLocation(MainAdvLocation + (FVector(-100.f, -100.f, 0.f)));
		CurrentAdventurers[0]->SetActorLocation(MainAdvLocation + (FVector(-100.f, 100.f, 0.f)));
	}
	if (frontLiner == 2)
	{
		CurrentAdventurers[0]->SetActorLocation(MainAdvLocation + (FVector(-100.f, -100.f, 0.f)));
		CurrentAdventurers[1]->SetActorLocation(MainAdvLocation + (FVector(-100.f, 100.f, 0.f)));
	}
}

void AShmupProtoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShmupProtoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShmupProtoCharacter::MoveRight);
	PlayerInputComponent->BindAction("RightClick", IE_Released, this, &AShmupProtoCharacter::Dash);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &AShmupProtoCharacter::Shoot);
	PlayerInputComponent->BindAction("Swap", IE_Released, this, &AShmupProtoCharacter::Swap);


}

void AShmupProtoCharacter::MoveForward(float AxisValue)
{
	if ((Controller != NULL) && (AxisValue != 0.0f))
	{
		CurrentAdventurers[0]->MoveForward(AxisValue);
		UE_LOG(LogTemp, Warning, TEXT("Controller Forward"));
	}
}

void AShmupProtoCharacter::MoveRight(float AxisValue)
{
	if ((Controller != NULL) && (AxisValue != 0.0f))
	{
		CurrentAdventurers[0]->MoveRight(AxisValue);
		UE_LOG(LogTemp, Warning, TEXT("Controller Right"));
	}
}

void AShmupProtoCharacter::Shoot()
{
	CurrentAdventurers[0]->Shoot();
}



void AShmupProtoCharacter::Dash()
{
	FVector DashLocation = GetActorLocation() + GetActorForwardVector() * 200;

	LaunchCharacter(GetActorForwardVector() * DashSpeed, false, false);
}

void AShmupProtoCharacter::Swap()
{
	frontLiner++;
}

FRotator AShmupProtoCharacter::getLookAt()
{
	return LookAt;
}


// Called when the game starts or when spawned
void AShmupProtoCharacter::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	CurrentAdventurers[0] = World->SpawnActor<AClassBase>(Adventurers[0], GetActorLocation(), FRotator(0.f, LookAt.Yaw, 0.f));
	CurrentAdventurers[0]->SetOwner(this);

	CurrentAdventurers[1] = World->SpawnActor<AClassBase>(Adventurers[1], GetActorLocation() + FVector(-100.f, -100.f, 0.f) , FRotator(0.f, LookAt.Yaw, 0.f));
	CurrentAdventurers[1]->SetOwner(this);

	CurrentAdventurers[2] = World->SpawnActor<AClassBase>(Adventurers[2], GetActorLocation() + FVector(-100.f, 100.f, 0.f), FRotator(0.f, LookAt.Yaw, 0.f));
	CurrentAdventurers[2]->SetOwner(this);

}
