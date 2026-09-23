#include "AnimNotifyState_DealDamage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"

void UAnimNotifyState_DealDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow, TEXT("DealDamage Begin Notify "));
}

void UAnimNotifyState_DealDamage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    FVector StartTraceLocation = MeshComp->GetSocketLocation(TEXT("RightHandSocket"));
    FVector EndTraceLocation = MeshComp->GetSocketLocation(TEXT("LeftHandSocket"));
    float Radius = 10;
    bool bComplexTrace = false;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(MeshComp->GetOwner());
    bool bIgnoreSelf = true;
    FLinearColor TraceColor = FLinearColor::Red;
    FLinearColor TraceHitColor = FLinearColor::Green;
    float DrawTime = 20;

    TArray<FHitResult> HitArray;
    const bool Hit = UKismetSystemLibrary::SphereTraceMulti(
        MeshComp,
        StartTraceLocation,
        EndTraceLocation,
        Radius,
        UEngineTypes::ConvertToTraceType(ECC_Pawn), // Fixed: was ECC_Camera
        bComplexTrace,
        ActorsToIgnore,
        EDrawDebugTrace::ForDuration,
        HitArray,
        bIgnoreSelf,
        TraceColor,
        TraceHitColor,
        DrawTime
    );

    if (Hit)
    {
        for (const FHitResult HitResult : HitArray)
        {
			if (ABaseCharacter* AttackCharacter = Cast<ABaseCharacter>(HitResult.GetActor()))
            {
                UGameplayStatics::ApplyDamage(
                    HitResult.GetActor(),
                   AttackCharacter->Damage,
                    Cast<APawn>(MeshComp->GetOwner())->GetController(),
                    MeshComp->GetOwner(),
                    AttackCharacter->DamageTypeClass);
            }
        }
    }
}

void UAnimNotifyState_DealDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Yellow, TEXT("DealDamage End Notify "));
}