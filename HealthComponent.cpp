#include "HealthComponent.h"
#include "GameFramework/Actor.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamage(float Amount)
{
    if (Amount <= 0.f || CurrentHealth <= 0.f)
        return;

    CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.f, MaxHealth);

    if (CurrentHealth <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s est mort !"), *GetOwner()->GetName());
        // Tu pourrais déclencher un event, détruire l'acteur, etc.
    }
}

bool UHealthComponent::IsDead() const
{
    return CurrentHealth <= 0.f;
}
