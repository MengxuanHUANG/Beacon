// Fill out your copyright notice in the Description page of Project Settings.


#include "LateUpdateComponent.h"

//Beacon Headers BEGIN
#include "BeaconLog.h"
//Beacon Headers END
// 
// Sets default values for this component's properties
ULateUpdateComponent::ULateUpdateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
	// ...
}


// Called every frame
void ULateUpdateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LateTickFunction.ExecuteIfBound(DeltaTime);
}

ULateUpdateComponent* ULateUpdateComponent::CreateLateUpdataComponent(UObject* Outer)
{
	ULateUpdateComponent* lateUpdateComp = NewObject<ULateUpdateComponent>(Outer);
	lateUpdateComp->RegisterComponent();

	return lateUpdateComp;
}