// Fill out your copyright notice in the Description page of Project Settings.


#include "FlammableComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Components/StaticMeshComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "GameplayTagContainer.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UFlammableComponent::UFlammableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Fire Particles"));
	m_ParticleSystem->SetupAttachment(this);

	//ignore parent's rotation
	SetUsingAbsoluteRotation(true);
}


// Called when the game starts
void UFlammableComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsUsingAbsoluteRotation())
	{
		UE_LOG(LogTemp, Warning, TEXT("reletive"));
	}

	const AActor* owner = GetOwner();

	if (owner != nullptr)
	{
		TArray<USceneComponent*> children;
		GetOwner()->GetRootComponent()->GetChildrenComponents(true, children);
		for (USceneComponent* component : children)
		{
			UClass* uClass = component->GetClass();
			FString name = uClass->GetFullGroupName(false);

			//Temp: Bind Dynamic Callback Function
			//TODO: template or abstract class
			if (name.Compare("BoxComponent") == 0)
			{
				Cast<UBoxComponent>(component)->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
			}
			else if (name.Compare("CapsuleComponent") == 0)
			{
				Cast<UCapsuleComponent>(component)->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
			}
			else if (name.Compare("SphereComponent") == 0)
			{
				Cast<USphereComponent>(component)->OnComponentBeginOverlap.AddDynamic(this, &UFlammableComponent::OnBeginOverlap);
			}
		}
	}

	if (m_InitializeWithFlame)
	{
		b_IsBurning = true;
		m_ParticleSystem->SetTemplate(T_FireParticle);
	}
}

//Called when component destroyed
void UFlammableComponent::DestroyComponent(bool bPromoteChildren)
{
	if (m_ParticleSystem != nullptr)
	{
		m_ParticleSystem->DestroyComponent(bPromoteChildren);
	}

	Super::DestroyComponent(bPromoteChildren);
}

// Called every frame
void UFlammableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFlammableComponent::Ignited(UParticleSystem* particle)
{
	if (m_ParticleSystem != nullptr)
	{
		m_ParticleSystem->SetTemplate(particle);
	}
}

void UFlammableComponent::OnBeginOverlap(UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
#ifndef UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(OtherActor->GetFullName()));
#endif

	if (!b_IsBurning && OtherActor->ActorHasTag(FName("Flammable")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ignite"));
		UFlammableComponent* otherflammable = Cast<UFlammableComponent>(OtherActor->GetComponentByClass(UFlammableComponent::StaticClass()));
		if (otherflammable->IsBurning())
		{
			this->Ignited(otherflammable->GetFireParticle());
		}
	}
}

UParticleSystem* UFlammableComponent::GetFireParticle() const
{
	if (b_IsBurning)
	{
		return m_ParticleSystem->Template;
	}
	return nullptr;
}