// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BuildableComponent.generated.h"


UCLASS( ClassGroup=(Beacon), BlueprintType)
class BEACON_API UBuildableComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildableComponent();

protected:
	virtual void DestroyComponent(bool bPromoteChildren) override;

protected:
	virtual bool Build_Implement() { return false; }
	virtual	void Clear_Implement() {}

public:
	UFUNCTION(BlueprintCallable)
		void Clear();

	UFUNCTION(BlueprintCallable)
		void Build();

	UFUNCTION(BlueprintCallable)
		void ReBuild();

protected:
	UPROPERTY(VisibleAnywhere)
		bool bIsBuilded;
};
