// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWalker.h"
#include "DebugBaseWalker.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_API ADebugBaseWalker : public ABaseWalker
{
	GENERATED_BODY()
	ADebugBaseWalker();
	UPROPERTY()
	UStaticMeshComponent* ConeMesh;
	
};
