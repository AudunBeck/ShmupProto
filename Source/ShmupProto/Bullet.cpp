// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector locationChange = GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime);

	SetActorRelativeLocation(locationChange);

	if (LifeLength <= 0)
	{
		Destroy();
	}

	LifeLength -= DeltaTime;

}

void ABullet::OnOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult & SweepResult)
{
	
}

