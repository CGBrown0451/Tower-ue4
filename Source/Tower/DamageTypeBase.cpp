// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTypeBase.h"
#include "DamageManager.h"

FDamageResult UDamageTypeBase::CalculateDamage(FDamageStats Damage)
{
	FDamageResult Result;
	Result.Health = Damage.Damage;
	return Result;
}

FDamageResult UDamageTypeBase::CalculateArmorPen(float Damage, float Health, float Armor, float ArmorPen)
{
	FDamageResult Result;
	if (Armor > 0.0f)
	{
		Result.Health = Damage * ArmorPen;
		Result.Armor = (1 - ArmorPen) * Damage;

		if (Result.Armor > Armor) {
			float ArmorCoeff = Armor / Result.Armor;
			Result.Armor = Armor;
			Result.Health = (ArmorCoeff * Damage * ArmorPen) + ((1 - ArmorCoeff) * Damage);
		}
	}else
	{
		Result.Health = Health;
	}

	return Result;

}
