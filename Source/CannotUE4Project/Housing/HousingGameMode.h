#pragma once
#include "GameFramework/GameModeBase.h"
#include "HousingGameMode.generated.h"


UCLASS()
class CANNOTUE4PROJECT_API AHousingGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:

    virtual void PostLogin(APlayerController* NewPlayer) override;


protected:


};
