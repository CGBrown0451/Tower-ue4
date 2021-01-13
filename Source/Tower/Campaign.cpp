// Fill out your copyright notice in the Description page of Project Settings.


#include "Campaign.h"

UCampaign::UCampaign()
{
	
}

EScoreResultType UCampaign::CheckScore(float Score, FLevelProperties Level)
{
	if (bAreHigherScoresBetter)
	{
		if(Score > Level.AuthorScore)
		{
			return ScoreResultType_Author;
		}else if (Score > Level.GoldScore)
		{
			return ScoreResultType_Gold;
		}else if (Score > Level.SilverScore)
		{
			return ScoreResultType_Silver;
		}else if (Score > Level.BronzeScore)
		{
			return ScoreResultType_Bronze;
		}
		return ScoreResultType_None;
	}else
	{
		if (Score < Level.AuthorScore)
		{
			return ScoreResultType_Author;
		}
		else if (Score < Level.GoldScore)
		{
			return ScoreResultType_Gold;
		}
		else if (Score < Level.SilverScore)
		{
			return ScoreResultType_Silver;
		}
		else if (Score < Level.BronzeScore)
		{
			return ScoreResultType_Bronze;
		}
		return ScoreResultType_None;
	}
}

FLevelProperties UCampaign::GetLevelAtPosition(int Position)
{
	if (Position >= 0 && Position < Levels.Num())
	{
		return Levels[Position];
	}else
	{
		FLevelProperties Empty;
		return Empty;
	}
}
