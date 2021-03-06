// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "ClassBase.h"
#include "ShmupProtoCharacter.generated.h"

UCLASS(Blueprintable)
class AShmupProtoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShmupProtoCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Shoot();
	void Dash();
	void Swap();
	FRotator getLookAt();

	UPROPERTY(EditAnywhere)
		int frontLiner = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere)
		FRotator LookAt;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float DashSpeed;

	UPROPERTY(EditAnywhere, Category = "Adventurers")
		class AClassBase* CurrentAdventurers[3]{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Adventurers")
		TSubclassOf<class AClassBase> Adventurers[3];

	bool dashing = false;
};

