// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "ShmupProtoCharacter.h"
#include "ClassBase.generated.h"

UCLASS()
class SHMUPPROTO_API AClassBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AClassBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Bullet")
		TSubclassOf<class ABullet> Bullet;

	UPROPERTY(EditAnywhere, Category = "Bullet")
		float BulletSpeed;

	UPROPERTY(EditAnywhere, Category = "Bullet")
		float BulletLife;

	class AShmupProtoCharacter* CharacterOwner{ nullptr };

	void Shoot();
	void SetOwner(class AShmupProtoCharacter* ToBeOwner);

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	
	
};
