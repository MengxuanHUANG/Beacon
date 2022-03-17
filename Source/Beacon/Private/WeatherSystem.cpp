// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherSystem.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "BeaconLog.h"
#include "UnitComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AWeatherSystem::AWeatherSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RainColliderBox = CreateDefaultSubobject<UBoxComponent>(FName("box"));
	SetRootComponent(RainColliderBox);

	RainColliderPlane = CreateDefaultSubobject<UBoxComponent>(FName("plane"));
	RainColliderPlane->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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
	Super::Tick(DeltaTime);

	for (UUnitComponent* Flammableunit : FlammableUnitSet)
	{
		if (Flammableunit != nullptr)
		{
			FVector location = Flammableunit->GetComponentLocation();
			const AActor* actor = Flammableunit->GetOwner();
			FHitResult outHit;
			m_Direction = FVector(0, 0, 1);
			CastRay(actor, outHit, m_Direction, 114514.f, location);

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

