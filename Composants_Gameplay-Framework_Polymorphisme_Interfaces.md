# Cours Unreal Engine 5.6  
## Programmation C++ — Composants, Gameplay Framework, Polymorphisme & Interfaces  
---
```xml
<?xml version="1.0" encoding="utf-8" ?>
<Configuration xmlns="https://www.unrealengine.com/BuildConfiguration">
    <BuildConfiguration>
        <bAllowUBAExecutor>false</bAllowUBAExecutor>
        <bAllowUBALocalExecutor>false</bAllowUBALocalExecutor>
    </BuildConfiguration>
</Configuration>
```
%APPDATA%\Unreal Engine\UnrealBuildTool\

# MODULE 1 — Introduction à un Actor en C++  
## Et explication du fonctionnement `.h` / `.cpp` 

---

# 1.1 Pourquoi Unreal utilise un `.h` et un `.cpp` ?

En C++, et particulièrement dans Unreal, une classe est divisée en deux fichiers :

### Fichier `.h` (header) — *déclaration*  
Contient :  
- déclaration de la classe  
- propriétés (`UPROPERTY`)  
- signatures de fonctions (`UFUNCTION`)  
- macros Unreal (`UCLASS`, `GENERATED_BODY`)  

 **Unreal Editor lit uniquement les `.h`** pour exposer les classes dans le moteur et les Blueprints.

---

###  Fichier `.cpp` (source) — *implémentation*  
Contient :  
- la logique réelle exécutée en jeu  
- les calculs  
- les algorithmes  
- les comportements  

 Le `.cpp` est compilé → c’est le code exécutable.

---

# 1.2 Exemple d’Actor minimal

## Fichier `.h`
```cpp
UCLASS()
class MYPROJECT_API AMyActor : public AActor
{
    GENERATED_BODY()

public:
    AMyActor();

protected:
    virtual void BeginPlay() override;

    // Mesh principale de l'acteur
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

public:
    virtual void Tick(float DeltaTime) override;
};
````

## Fichier `.cpp`

```cpp
AMyActor::AMyActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMyActor::BeginPlay()
{
    Super::BeginPlay();
}

void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
```

---

# Exercice 1 — Faire avancer un Actor

## `.h`

```cpp
UPROPERTY(EditAnywhere)
float Speed;
```

## `.cpp`

```cpp
void AMovementActor::Tick(float DeltaTime)
{
    FVector Location = GetActorLocation();
    Location.X += Speed * DeltaTime;
    SetActorLocation(Location);
}
```

---

# MODULE 2 — Composants & Gameplay Framework 

---

# 2.1 Théorie : Pourquoi Unreal utilise des composants ?

Unreal Engine est basé sur une architecture modulaire appelée **Component-Based Architecture**.

Un **Actor** = un conteneur vide.
Les **Components** = ses capacités (mesh, son, collision, IA, caméra…).

Exemples :

| Type de composant    | Rôle                    |
| -------------------- | ----------------------- |
| UStaticMeshComponent | Affichage 3D            |
| UCameraComponent     | Vue du joueur           |
| USpringArmComponent  | Offset caméra dynamique |
| UCapsuleComponent    | Collision               |
| UAudioComponent      | Sons                    |
| UActorComponent      | Logique non visuelle    |

### Pourquoi c’est puissant ?

Parce que tu peux “composer” un objet complexe en combinant plusieurs comportements simples.

---

# 2.2 Théorie : Le Gameplay Framework

Unreal possède une architecture fixe pour gérer le gameplay :

| Classe               | Rôle                                |
| -------------------- | ----------------------------------- |
| **GameMode**         | Règles du jeu, spawn du joueur      |
| **PlayerController** | Reçoit les inputs                   |
| **Pawn**             | Entité contrôlable                  |
| **Character**        | Version avancée du Pawn             |
| **GameState**        | Informations globales               |
| **PlayerState**      | Infos d’un joueur                   |

### Pourquoi c’est structuré ainsi ?

Parce que cela facilite :
✔ le multijoueur
✔ la séparation des responsabilités
✔ la clarté du code
✔ la réutilisabilité

---

# 2.3 Composants — Exemple pratique

## `.h`

```cpp
UPROPERTY(VisibleAnywhere)
UStaticMeshComponent* Mesh;

UPROPERTY(VisibleAnywhere)
USpringArmComponent* SpringArm;

UPROPERTY(VisibleAnywhere)
UCameraComponent* Camera;
```

## `.cpp`

```cpp
AMyActor::AMyActor()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetupAttachment(RootComponent);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);
}
```

---

# 2.4 Théorie : Enhanced Input System

Le nouveau système d’input d’Unreal repose sur :

* **Input Mapping Context** = “pack” de contrôles
* **Input Action** = action (sauter, tirer…)
* **Modifiers** = transformation de valeur
* **Triggers** = conditions (double clic, hold press…)

### Pourquoi c’est mieux ?

✔ supporte plusieurs devices
✔ personnalisable selon le mode de jeu
✔ clair, modulaire, extensible
✔ robuste pour le multijoueur

---

# MODULE 3 — Polymorphisme, Héritage et Interfaces 

---

# 3.1 Théorie : Qu’est-ce que le polymorphisme ?

Le polymorphisme = appeler la bonne fonction selon le **type réel** de l’objet, même si le pointeur est du type parent.

Exemple :

```cpp
AWeapon* Weapon = NewObject<APistol>();
Weapon->Fire(); // Applique APistol::Fire()
```

### Pourquoi c’est indispensable dans un jeu ?

Parce que des objets génériques (“arme”, “ennemi”, “pickup”)
se déclinent en **dizaines de variantes**.

---

# 3.2 Interfaces Unreal 

Une interface Unreal = combinaison de :

* une classe UINTERFACE (utilisée par l’éditeur)
* une classe C++ (le contrat réel)

Pourquoi utiliser une interface ?

* Un Actor peut hériter d’UN SEUL parent, mais de **MULTIPLES interfaces**
* Permet de créer des systèmes modulaires
* Compatible Blueprint et C++
* Parfait pour interactions (ouvrir portes, ramasser objets…)

---

# 3.3 Exemple d’interface

## UInterface (éditeur)

```cpp
UINTERFACE(Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};
```

## Interface C++

```cpp
class IInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    void Interact(AActor* Instigator);
};
```

## Implémentation

```cpp
void ADoor::Interact_Implementation(AActor* Instigator)
{
    OpenDoor();
}
```

---

# MODULE 4 — Système d’armes polymorphique

---

# 4.1 Pourquoi une classe de base abstraite ?

Cela permet de définir :

* un comportement commun
* un schéma pour toutes les armes
* une base extensible

Toutes les armes possèdent :

* Fire()
* Recoil
* Muzzle Flash
* Son
* Dégâts

Donc on crée :

```cpp
UCLASS(Abstract)
class AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    virtual void Fire();
};
```

---

# 4.2 Pourquoi dériver APistol et AShotgun ?

Parce que chaque arme a un comportement unique :

| Arme    | Comportement                  |
| ------- | ----------------------------- |
| Pistol  | tir unique, cadence modérée   |
| Shotgun | multiples projectiles, spread |
| Sniper  | précision élevée, zoom        |
| Rocket  | projectile lent + explosion   |

Le polymorphisme permet :

```cpp
CurrentWeapon->Fire();
```

→ appel correct selon le type réel.

---

# 4.3 Exemple d’implémentations

### Pistol

```cpp
void APistol::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Pistol Fire"));
}
```

### Shotgun

```cpp
void AShotgun::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Shotgun Boom!"));
}
```

---

# MODULE 5 — Système d’interaction

---

# 5.1 Pourquoi utiliser un Raycast ?

Parce qu’un Raycast :

* est performant
* peut cibler n’importe quoi
* fonctionne en FPS/TPS
* évite les collisions complexes
* est simple à déboguer

Tu tires un rayon devant la caméra :

```cpp
GetWorld()->LineTraceSingleByChannel(...);
```

S’il touche un objet **interactif**, tu appelles :

```cpp
IInteractable::Execute_Interact(HitActor, this);
```

---

# 5.2 Pourquoi utiliser une interface pour interagir ?

Sans interface :

* ton code devient énorme
* tu dois faire un `if / else` pour chaque type d’objet
* ton gameplay n’est pas extensible

Avec interface :

```cpp
if (HitActor->Implements<UInteractable>())
{
    IInteractable::Execute_Interact(HitActor, this);
}
```

Ajoute autant d’objets interactifs que tu veux **sans modifier le code du joueur**.

---

#  Exercice final — Mini système de combat + interaction

Créer :

1. **AWeaponBase** (classe abstraite)
2. **APistol** et **AShotgun**
3. **AEnemy** avec TakeDamage()
4. **IInteractable** pour les pickups
5. Un **raycast** dans le PlayerController
6. Des Blueprints pour effets visuels

---

