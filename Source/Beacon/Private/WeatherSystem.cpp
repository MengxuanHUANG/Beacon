// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherSystem.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "BeaconLog.h"
#include "UnitComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeatherSystem::AWeatherSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RainColliderBox = CreateDefaultSubobject<UBoxComponent>(FName("box"));
	SetRootComponent(RainColliderBox);

	RainColliderPlane = CreateDefaultSubobject<UBoxComponent>(FName("plane"));
	RainColliderPlane->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	RainParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("rain"));
	RainParticle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}


void AWeatherSystem::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName(BEACON_FLAMMABLE_UNIT_TAG)))
	{
		UUnitComponent* other = Cast<UUnitComponent>(OtherComp->GetAttachParent());
		FlammableUnitSet.Emplace(other);
	}

}

void AWeatherSystem::OnEndOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName(BEACON_FLAMMABLE_UNIT_TAG)))
	{
		UUnitComponent* other = Cast<UUnitComponent>(OtherComp->GetAttachParent());
		FlammableUnitSet.Remove(other);
	}
}




void AWeatherSystem::Tick(float DeltaTime)
{
	float x1;
	x1 = Rain_Direction.X;
	float y2;
	y2 = Rain_Direction.Y;
	BEACON_LOG(Warning, "X is: %f, Y is %f", Rain_Direction.X, Rain_Direction.Y);
	BEACON_LOG(Warning, "X is: %f, Y is %f", x1, y2);
	if (x1 == float(0) && y2 == float(0))
	{
		m_Rain_Direction = FVector(0, 0, -Rain_speed * 100);
	}
	else if (FMath::Abs(x1) > FMath::Abs(y2))
	{
		m_Rain_Direction = FVector(Rain_speed * 100 * Rain_Direction.X / FMath::Abs(x1),   Rain_speed * 100 * Rain_Direction.Y / FMath::Abs(x1), -Rain_speed * 100);

	}
	else
	{
		m_Rain_Direction = FVector(Rain_speed * 100 * Rain_Direction.X / FMath::Abs(y2), float(Rain_speed * 100) * Rain_Direction.Y / FMath::Abs(y2), -Rain_speed * 100);
	}
	BEACON_LOG(Warning, "%s", *m_Rain_Direction.ToString());
	Super::Tick(DeltaTime);
	RainParticle->SetVectorParameter("Wind_power", m_Rain_Direction);
	for (UUnitComponent* Flammableunit : FlammableUnitSet)
	{
		if (Flammableunit != nullptr)
		{
			FVector location = Flammableunit->GetComponentLocation();
			const AActor* actor = Flammableunit->GetOwner();
			FHitResult outHit;
			CastRay(actor, outHit, -m_Rain_Direction, 114514.f, location);
			if (outHit.Component.IsValid() && outHit.Component == RainColliderPlane)
			{
				FString cname = outHit.Component->GetName();
				FString aname = outHit.Actor->GetName();
				UE_LOG(LogTemp, Display, TEXT("Actor is %s, Component is %s"), *aname, *cname);
				Flammableunit->AddValue(-1000);
			}
		}		
	}

	


	
}

void AWeatherSystem::CastRay(const AActor* actor, FHitResult& result, FVector direction, float length, FVector start_location)
{
	FCollisionQueryParams param;
	param.AddIgnoredComponent(RainColliderBox);
	param.AddIgnoredActor(actor);
	GetWorld()->LineTraceSingleByChannel(result, start_location, start_location + direction * length, ECC_Visibility, param);
}

// Called when the game starts or when spawned
void AWeatherSystem::BeginPlay()
{
	Super::BeginPlay();
	RainColliderBox->OnComponentBeginOverlap.AddDynamic(this, &AWeatherSystem::OnBeginOverlap);
	RainColliderBox->OnComponentEndOverlap.AddDynamic(this, &AWeatherSystem::OnEndOverlap);
}

