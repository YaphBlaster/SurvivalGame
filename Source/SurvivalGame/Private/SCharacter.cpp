// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/SInteractionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	//GetMesh() is the parent mesh. Currently it is visually represented by the Head model
	GetMesh()->SetWorldLocation(FVector(0.f, 0.f, -90.0f));
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.0f, 0.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->bUsePawnControlRotation = true;
	// Link camera to Head using a Socket
	CameraComponent->SetupAttachment(GetMesh(), FName("CameraSocket"));


	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetMesh"));
	HelmetMesh->SetupAttachment(GetMesh());
	HelmetMesh->SetMasterPoseComponent(GetMesh());

	ChestMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestMesh"));
	ChestMesh->SetupAttachment(GetMesh());
	ChestMesh->SetMasterPoseComponent(GetMesh());

	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsMesh"));
	LegsMesh->SetupAttachment(GetMesh());
	LegsMesh->SetMasterPoseComponent(GetMesh());

	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	FeetMesh->SetupAttachment(GetMesh());
	FeetMesh->SetMasterPoseComponent(GetMesh());

	VestMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VestMesh"));
	VestMesh->SetupAttachment(GetMesh());
	VestMesh->SetMasterPoseComponent(GetMesh());

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMesh"));
	HandsMesh->SetupAttachment(GetMesh());
	HandsMesh->SetMasterPoseComponent(GetMesh());

	BackpackMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackpackMesh"));
	BackpackMesh->SetupAttachment(GetMesh());
	BackpackMesh->SetMasterPoseComponent(GetMesh());

	InteractionCheckFrequency = 0.f;
	InteractionCheckDistance = 1000.f;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	GetMesh()->SetOwnerNoSee(true);

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ASCharacter::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Val);
	}

}

void ASCharacter::LookUp(float Val)
{
	if (Val != 0.0f)
	{
		AddControllerPitchInput(Val);
	}
}

void ASCharacter::Turn(float Val)
{
	if (Val != 0.0f)
	{
		AddControllerYawInput(Val);
	}
}

void ASCharacter::StartCrouching()
{
	Crouch();
}

void ASCharacter::StopCrouching()
{
	UnCrouch();
}

void ASCharacter::PerformInteractionCheck()
{

	if (GetController() == nullptr)
		return;

	//Timestamp
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector EyesLoc;
	FRotator EyesRot;

	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	FVector TraceStart = EyesLoc;
	FVector TraceEnd = (EyesRot.Vector() * InteractionCheckDistance) + TraceStart;
	FHitResult TraceHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		//Check if we hit an object
		if (TraceHit.GetActor())
		{
			//Check if actor has a interaction component
			if (USInteractionComponent* InteractionComponent = Cast<USInteractionComponent>(TraceHit.GetActor()->GetComponentByClass(USInteractionComponent::StaticClass())))
			{
				//How far away the player is
				float Distance = (TraceStart - TraceHit.ImpactPoint).Size();
				
				//If the InteractionComponent is not the old interactable we just saw and we are close enough to the interactable
				if (InteractionComponent != GetInteractable() && Distance <= InteractionComponent->InteractionDistance)
				{
					FoundNewInteractable(InteractionComponent);
				}
				else if (Distance > InteractionComponent->InteractionDistance && GetInteractable())
				{
					CouldNotFindInteractable();
				}

				return;
			}
				
		}
	}

	CouldNotFindInteractable();
}

void ASCharacter::CouldNotFindInteractable()
{

}

void ASCharacter::FoundNewInteractable(USInteractionComponent* Interactable)
{
	UE_LOG(LogTemp, Log, TEXT("We Found an interactable"));
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformInteractionCheck();

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::StartCrouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::StopCrouching);

}

