// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPainCausingVolume.h"

#include "DamageTaker.h"

void AMyPainCausingVolume::CausePainTo(AActor* Other)
{
	FDamageStats Stats;
	Stats.Damage = DamagePerSec * PainInterval;
	Stats.DamageType = Cast<UDamageTypeBase>(DamageType.GetDefaultObject());
	Stats.Mod = Mod;
	IDamageTaker* Taker = Cast<IDamageTaker>(Other);
	if(Taker)
	{
		Taker->TakeDamage_Implementation(Stats);
	}
}
