// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CC_Projectile.generated.h"

class UProjectileMovementComponent;
class UGameplayEffect;

UCLASS()
class GASCRASHCOURSE_API ACC_Projectile : public AActor
{
	GENERATED_BODY()
	
public:
	ACC_Projectile();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|Damage", meta = (ExposeOnSpawn, ClampMin = "0.0"))
	float Damage{10.f};

private:
	UPROPERTY(VisibleAnywhere, Category = "Crash|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Crash|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
