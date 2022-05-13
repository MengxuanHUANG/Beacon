// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherSystem.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "BeaconLog.h"
#include "UnitComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "RainSystem.h"
#include "Components/ActorComponent.h"

// Sets default values
AWeatherSystem::AWeatherSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bGenerateOverlapEventsDuringLevelStreaming = 1;

	RainColliderBox = CreateDefaultSubobject<UBoxComponent>(FName("box"));
	RainColliderBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RainColliderBox->OnComponentBeginOverlap.AddDynamic(this, &AWeatherSystem::OnBeginOverlap);
	RainColliderBox->OnComponentEndOverlap.AddDynamic(this, &AWeatherSystem::OnEndOverlap);

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



void AWeatherSystem::WeatherActive(bool WeatherActive)
{
	RainParticle->SetActive(WeatherActive, false);
	m_WeatherActive = WeatherActive;
}

void AWeatherSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_WeatherActive)
	{
		return;
	}
	float x1;
	x1 = T_WeatherSystem->m_Weather_Data.Weather_Direction.X;
	float y2;
	y2 = T_WeatherSystem->m_Weather_Data.Weather_Direction.Y;
	BEACON_LOG(Warning, "X is: %f, Y is %f", x1, y2);
	float Rain_speed;
	Rain_speed = T_WeatherSystem->Speed;
	FName weather = T_WeatherSystem->m_Weather_Data.Weather;
	if (weather == "snow")
	{
		speed_coefficient = 50;
	}
	else if (weather == "rain")
	{
		speed_coefficient = 100;
	}
	if (x1 == float(0) && y2 == float(0))
	{
		m_Rain_Direction = FVector(0, 0, -Rain_speed * speed_coefficient);
		BEACON_LOG(Warning, "X is: %f, Y is %f", m_Rain_Direction.X, m_Rain_Direction.Y);
	}
	else if (FMath::Abs(x1) > FMath::Abs(y2))
	{
		m_Rain_Direction = FVector(x1 * speed_coefficient / FMath::Abs(x1) * Rain_speed,  y2 * speed_coefficient /FMath::Abs(x1) * Rain_speed, -Rain_speed * 100);
		BEACON_LOG(Warning, "X is: %f, Y is %f", m_Rain_Direction.X, m_Rain_Direction.Y);
	}
	else
	{
		m_Rain_Direction = FVector( x1 * speed_coefficient / FMath::Abs(y2) * Rain_speed, y2 * speed_coefficient / FMath::Abs(y2) * Rain_speed, -Rain_speed * 100);
		BEACON_LOG(Warning, "X is: %f, Y is %f", m_Rain_Direction.X, m_Rain_Direction.Y);
	}
	BEACON_LOG(Warning, "%s", *m_Rain_Direction.ToString());
	
	RainParticle->SetVectorParameter(T_WeatherSystem->m_Weather_Data.Weather, m_Rain_Direction);
	for (UUnitComponent* Flammableunit : FlammableUnitSet)
	{
		if (Flammableunit != nullptr)
		{
			FVector location = Flammableunit->GetComponentLocation();
			const AActor* actor = Flammableunit->GetOwner();
			FHitResult outHit;
			CastRay(actor, outHit, -m_Rain_Direction, 100000.f, location);
			if (outHit.Component.IsValid() && outHit.Component == RainColliderPlane)
			{
				FString cname = outHit.Component->GetName();
				FString aname = outHit.Actor->GetName();
				UE_LOG(LogTemp, Display, TEXT("Actor is %s, Component is %s"), *aname, *cname);
				Flammableunit->AddValue(T_WeatherSystem->Thermal_reduce * DeltaTime);
			}
		}		
	}

	


	
}

void AWeatherSystem::CastRay(const AActor* actor, FHitResult& result, FVector direction, float length, FVector start_location)
{
	FCollisionQueryParams param;
	param.AddIgnoredComponent(RainColliderBox);
	param.AddIgnoredActor(actor);
	for (const AActor* ignoredActor : m_IgnoredActors)
	{
		param.AddIgnoredActor(ignoredActor);
	}
	GetWorld()->LineTraceSingleByChannel(result, start_location, start_location + direction * length, ECC_Visibility, param);
}

// Called when the game starts or when spawned
void AWeatherSystem::BeginPlay()
{
	Super::BeginPlay();
	WeatherActive(false);
	
}

