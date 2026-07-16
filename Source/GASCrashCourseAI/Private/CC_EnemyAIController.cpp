// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CC_EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CC_AI_PatrolWorldSubsystem.h"
#include "CC_AI_PatrolPoint.h"
#include "Characters/CC_PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

ACC_EnemyAIController::ACC_EnemyAIController()
{
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 700.f;
	SightConfig->LoseSightRadius = 900.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SetMaxAge(5.f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	EnemyPerceptionComponent->ConfigureSense(*SightConfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	HearingConfig->HearingRange = 1200.f;
	HearingConfig->SetMaxAge(0.f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	EnemyPerceptionComponent->ConfigureSense(*HearingConfig);

	EnemyPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ACC_EnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyPerceptionComponent)
	{
		EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACC_EnemyAIController::OnTargetDetected);
	}
}

void ACC_EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset != nullptr)
	{
		RunBehaviorTree(BehaviorTreeAsset);

		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			if (ACC_EnemyCharacter* Enemy = Cast<ACC_EnemyCharacter>(InPawn))
			{
				BB->SetValueAsFloat(FName("AcceptanceRadius"), Enemy->AcceptanceRadius - 100.f);
			}
		}
	}

	if (ACC_EnemyCharacter* BaseChar = Cast<ACC_EnemyCharacter>(InPawn))
	{
		SightConfig->SightRadius = BaseChar->SearchRange;
		SightConfig->LoseSightRadius = BaseChar->SearchRange + 200.f;

		BaseChar->OnDeath.AddUObject(this, &ThisClass::OnPawnDeath);
		BaseChar->OnHealNeeded.AddUObject(this, &ThisClass::OnHealNeeded);

		EnemyPerceptionComponent->ConfigureSense(*SightConfig);
	}
}

void ACC_EnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	if (ACC_EnemyCharacter* Enemy = Cast<ACC_EnemyCharacter>(GetPawn()))
	{
		FVector EyeLoc;
		FRotator EyeRot;
		Enemy->GetActorEyesViewPoint(EyeLoc, EyeRot);
		FVector LookDir = EyeRot.Vector();

		float HalfAngleRads = FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees);

		DrawDebugCone(GetWorld(), EyeLoc, LookDir, SightConfig->SightRadius, HalfAngleRads, HalfAngleRads, 12, FColor::Green, false, -1.f, 0, 1.5f);

		DrawDebugSphere(GetWorld(), EyeLoc, SightConfig->LoseSightRadius, 16, FColor::Magenta, false, -1.f, 0, 1.f);

		DrawDebugSphere(GetWorld(), EyeLoc, HearingConfig->HearingRange, 16, FColor::Blue, false, -1.f, 0, 1.f);
	}
#endif
}

void ACC_EnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!IsValid(BB)) return;

	UE_LOG(LogTemp, Display, TEXT("OnTargetDetected"));
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{

		UE_LOG(LogTemp, Display, TEXT("OnTargetDetected Sight"));

		if (Actor->ActorHasTag(TEXT("Player")))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				BB->SetValueAsObject(FName("TargetActor"), Actor);
				BB->ClearValue(FName("LastKnownLocation"));

				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("I SEE YOU!"));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Target Lost!"));

				if (!BB->IsVectorValueSet("KiteLocation"))
				{
					BB->ClearValue(FName("TargetActor"));
				}
				BB->SetValueAsVector(FName("LastKnownLocation"), Actor->GetActorLocation());

				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("I LOST YOU!"));
			}
		}
	}
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		UE_LOG(LogTemp, Display, TEXT("OnTargetDetected Hearing"));

		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Display, TEXT("Stimulus WasSuccessfullySensed"));

			UObject* CurrentTarget = BB->GetValueAsObject(FName("TargetActor"));
			if (!CurrentTarget)
			{
				BB->SetValueAsVector(FName("LastKnownLocation"), Stimulus.StimulusLocation);

				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("I HEAR YOU!"));
			}
		}
	}
}

void ACC_EnemyAIController::OnPawnDeath()
{
	UE_LOG(LogTemp, Error, TEXT("OnPawnDeath called!"));

	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(ControlledPawn))
	{
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		{
			ASC->CancelAllAbilities();
		}
	}

	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("Dead"));
	}

	if (UCharacterMovementComponent* MoveComp = ControlledPawn->FindComponentByClass<UCharacterMovementComponent>())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
	}

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		if (ACC_AI_PatrolPoint* Point = Cast<ACC_AI_PatrolPoint>(BB->GetValueAsObject(FName("PatrolPoint"))))
		{
			if (UWorld* World = GetWorld())
			{
				if (UCC_AI_PatrolWorldSubsystem* Subsystem = World->GetSubsystem<UCC_AI_PatrolWorldSubsystem>())
				{
					Subsystem->ReleasePoint(Point);
				}
			}
		}
	}

	ControlledPawn->DetachFromControllerPendingDestroy();
}

void ACC_EnemyAIController::OnHealNeeded()
{
	UE_LOG(LogTemp, Error, TEXT("OnHealNeeded called!"));

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsBool("HealNeeded", true);
	}
}