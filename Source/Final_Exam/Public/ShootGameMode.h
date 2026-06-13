// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootGameMode.generated.h"	// 메타 데이터 뜯어야돼서 가장 밑에 있어야 됌 됑 // 런타임시 리플렉션을 

/**
 * 
 */
UCLASS()
class FINAL_EXAM_API AShootGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AShootGameMode();
// VM 이 있어서 얘도 소멸자를 쓰지 않는다.
	
	
	
private:
	
};
