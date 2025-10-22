// Fill out your copyright notice in the Description page of Project Settings.


#include "KnockablePhysicsObject.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Perception/AISense_Hearing.h"


// Sets default values
AKnockablePhysicsObject::AKnockablePhysicsObject()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	SetRootComponent(StaticMeshComponent);
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	NavAreaBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("NavAreaBoxComponent"));
	NavAreaBoxComponent->SetupAttachment(RootComponent);

	StaticMeshComponent->SetCanEverAffectNavigation(false);
	StaticMeshComponent->SetCollisionProfileName("Ragdoll");
	StaticMeshComponent->SetSimulatePhysics(true);
	NavAreaBoxComponent->SetCanEverAffectNavigation(true);
	NavAreaBoxComponent->bDynamicObstacle = true;
	AudioComponent->SetAutoActivate(false);
}

void AKnockablePhysicsObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AKnockablePhysicsObject::OnObjectHit);
}

bool AKnockablePhysicsObject::IsFallen(float Z) const
{
	FRotator MeshRotation = StaticMeshComponent->GetComponentRotation();
	return StaticMeshComponent->GetComponentVelocity().SizeSquared() > FallSpeedThreshold * FallSpeedThreshold
		|| LastHitZ - Z > FallZHitThreshold && (FMath::Abs(MeshRotation.Pitch) > FallAngleThreshold || FMath::Abs(MeshRotation.Roll) > FallAngleThreshold);	
}

void AKnockablePhysicsObject::OnObjectHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsFallen(Hit.Location.Z))
		return;

	if (OtherActor == nullptr || OtherActor->IsA<APawn>())
		return;

	LastHitZ = Hit.Location.Z;
	AudioComponent->Play();
	UAISense_Hearing::ReportNoiseEvent(this, Hit.Location, AINoiseLoudness, this, AINoiseSoundRange, ObjectHitAISoundTag.GetTagName());
}
