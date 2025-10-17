#pragma once

#include "GestureDataTypes.generated.h"

struct FGestureResult
{
    bool bStarted = false;
    bool bLooped = false;
    float ExpectedDuration = 0.f;
};

USTRUCT()
struct FGestureAttachedItemData
{
    GENERATED_BODY()

    TWeakObjectPtr<UStaticMeshComponent> AttachedMeshComponent;

    UPROPERTY()
    FName LeftHandItemSocketAttachment;

    UPROPERTY()
    FName RightHandItemSocketAttachment;

    UPROPERTY()
    FName AttachmentSocket;
    
    UPROPERTY()
    TSoftObjectPtr<UStaticMesh> ItemMesh;
};