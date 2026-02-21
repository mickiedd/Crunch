# Crunch Project AI Analysis - Behaviac Integration Guide

**Date:** 2026-02-21 22:45  
**Analyst:** Doggy 🐶  
**Target:** Minion NPC System

---

## 📊 Project AI Overview

### Current AI System (Unreal Engine 5.5 Default)

**Architecture:**
```
MinionBarrack (Spawner)
    ↓
AMinion (AI Pawn) ← ACCharacter
    ↓
ACAIController (AI Controller)
    ↓
Minion_BT.uasset (Behavior Tree)
    ↓
Minion_BB.uasset (Blackboard)
```

**Key Components:**

| Component | Type | Location | Purpose |
|-----------|------|----------|---------|
| **AMinion** | C++ Class | `Source/Crunch/Private/AI/Minion.h/.cpp` | AI pawn with team support & goal system |
| **ACAIController** | C++ Class | `Source/Crunch/Private/AI/CAIController.h/.cpp` | Perception-based targeting & BT runner |
| **AMinionBarrack** | C++ Class | `Source/Crunch/Private/AI/MinionBarrack.h/.cpp` | Spawner/pooling system |
| **Minion_BP** | Blueprint | `Content/AI/Minions/Minion_BP.uasset` | Minion actor blueprint |
| **Minion_BT** | Behavior Tree | `Content/AI/Minions/Minion_BT.uasset` | UE5 behavior tree asset |
| **Minion_BB** | Blackboard | `Content/AI/Minions/Minion_BB.uasset` | Blackboard data |
| **AIC_Minion_BP** | Blueprint | `Content/AI/Minions/AIC_Minion_BP.uasset` | AI controller blueprint |

---

## 🎯 Perfect Test Candidate: **AMinion**

### Why Minion is Ideal for Testing Behaviac

✅ **Simple AI Structure**
- Inherits from `ACCharacter` (GAS-enabled)
- Lightweight logic (team, goal, active state)
- Perfect for before/after comparison

✅ **Already Has AI Systems**
- UE5 Behavior Tree (`Minion_BT`)
- Blackboard (`Minion_BB`)
- AI Perception (sight-based targeting)
- Can demonstrate Behaviac as alternative/enhancement

✅ **Well-Integrated**
- Works with GAS (Gameplay Ability System)
- Team system (Red vs Blue)
- Respawn/death mechanics
- Spawner system (MinionBarrack)

✅ **Visual Feedback**
- Uses Paragon minion models
- Clear attack/patrol behaviors
- Easy to observe in editor

✅ **Production-Ready**
- Used in actual gameplay
- Not a throwaway test actor
- Real performance considerations

---

## 🔄 UE5 BT vs Behaviac: Feature Comparison

### Unreal Engine 5 Behavior Trees (Current)

**Strengths:**
- ✅ Native UE5 integration
- ✅ Visual Blueprint editing
- ✅ Deep engine integration (NavMesh, EQS, etc.)
- ✅ Excellent debugging tools (visual debugger)
- ✅ Event-driven execution
- ✅ Built-in services and decorators

**Limitations:**
- ❌ Node creation requires C++/Blueprint knowledge
- ❌ Limited to tree structure (no FSM, HTN)
- ❌ Harder to test outside of UE5
- ❌ Data-driven design requires custom setup
- ❌ No external designer tool

**Current Minion Setup:**
```
Minion_BT.uasset
├─ Root Sequence
│  ├─ Find Target (Blackboard: Target)
│  ├─ Move To Target
│  └─ Attack (BTTask_SendInputToAbilitySystem)
└─ Services: Update Perception
```

### Behaviac Plugin (New)

**Strengths:**
- ✅ **41 behavior tree node types** (vs UE's ~15 base)
- ✅ **Multiple AI paradigms:** BT + FSM + HTN
- ✅ **XML-based** - Version control friendly
- ✅ **External designer tool** (original behaviac designer)
- ✅ **Data-driven** - Non-programmers can edit
- ✅ **Portable** - Can test logic outside UE5
- ✅ **Method-based execution** - Clean C++ handlers
- ✅ **Blackboard-like property system**

**Limitations:**
- ❌ Less UE5-native (no visual debugger)
- ❌ XML import workflow (not Blueprint)
- ❌ Smaller UE5 community
- ❌ Requires C++ for method handlers

**What Behaviac Adds:**
```
BehaviorTree System:
  - 41 node types (Sequence, Selector, Parallel, Loop, etc.)
  - Decorators (Time, Count, Loop, Repeat, etc.)
  - Probabilistic selection
  - Sub-tree support

FSM (Finite State Machines):
  - State enter/exit actions
  - Conditional transitions
  - Complex state hierarchies

HTN (Hierarchical Task Networks):
  - Goal-based planning
  - Task decomposition
  - Dynamic replanning
```

---

## 🧪 Proposed Test Integration

### Option 1: Side-by-Side Comparison (Recommended)

**Goal:** Add Behaviac to Minion WITHOUT removing UE5 BT

**Implementation:**
```cpp
// In Minion.h - Add new component
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
UBehaviacAgentComponent* BehaviacAgent;

UPROPERTY(EditAnywhere, Category = "AI")
bool bUseBehaviacAI = false;  // Toggle between systems

// In Minion.cpp
ABehaviacTestMinion::ABehaviacTestMinion()
{
    // Existing setup...
    
    // Add Behaviac component
    BehaviacAgent = CreateDefaultSubobject<UBehaviacAgentComponent>(TEXT("BehaviacAgent"));
}

void ABehaviacTestMinion::BeginPlay()
{
    Super::BeginPlay();
    
    if (bUseBehaviacAI && BehaviacAgent)
    {
        // Use Behaviac system
        BehaviacAgent->SetIntProperty(TEXT("Health"), GetHealth());
        BehaviacAgent->SetBoolProperty(TEXT("HasTarget"), false);
        BehaviacAgent->OnMethodCalled.AddDynamic(this, &ABehaviacTestMinion::HandleBehaviacMethod);
        BehaviacAgent->LoadBehaviorTreeByPath("MinionBehaviacTree");
    }
    else
    {
        // Use existing UE5 BT (current system)
        // Already handled by ACAIController
    }
}
```

**Benefit:** Direct comparison, can toggle at runtime!

### Option 2: Separate Test Actor

**Create:** `ABehaviacTestMinion` (inherits from `AMinion`)

**Why:**
- ✅ Non-invasive to production code
- ✅ Safe testing environment
- ✅ Can compare side-by-side in same level
- ❌ Duplicate some setup code

### Option 3: Hybrid System

**Goal:** Use both systems for different behaviors

**Example:**
- **UE5 BT:** High-level strategy (patrol, chase, flee)
- **Behaviac FSM:** Combat states (idle, attacking, blocking)
- **Behaviac HTN:** Squad tactics and planning

**Benefit:** Best of both worlds!

---

## 📝 Test Scenarios

### Scenario 1: Basic Behavior Replacement

**Current UE5 BT Logic:**
1. Check for target (Perception)
2. Move to target
3. Attack when in range
4. Return to spawn if target lost

**Behaviac Equivalent:**
```xml
<behavior name="MinionBasic">
  <node class="Sequence">
    <node class="Condition" method="HasTarget" />
    <node class="Action" method="MoveToTarget" />
    <node class="Condition" method="InAttackRange" />
    <node class="Action" method="AttackTarget" />
  </node>
</behavior>
```

**Metrics to Compare:**
- CPU usage per minion
- Memory footprint
- Response time (perception → action)
- Ease of editing/iteration

### Scenario 2: Advanced FSM

**New with Behaviac:**
```xml
<behavior name="MinionCombatFSM">
  <node class="FSM">
    <node class="State" name="Patrol">
      <enter method="StartPatrol" />
      <transition to="Chase" condition="EnemySighted" />
    </node>
    <node class="State" name="Chase">
      <enter method="BeginChase" />
      <update method="MoveToTarget" />
      <transition to="Attack" condition="InRange" />
      <transition to="Patrol" condition="TargetLost" />
    </node>
    <node class="State" name="Attack">
      <enter method="WindUpAttack" />
      <update method="ExecuteCombo" />
      <transition to="Chase" condition="OutOfRange" />
    </node>
  </node>
</behavior>
```

**Cannot be done with UE5 BT without complex custom nodes!**

### Scenario 3: Squad Tactics with HTN

**Goal:** Multiple minions coordinate attacks

**Behaviac HTN:**
```xml
<behavior name="SquadTactics">
  <node class="HTN">
    <task name="DefendBase">
      <method name="CallAllies" />
      <method name="FormDefensiveLine" />
    </task>
    <task name="AttackEnemy">
      <subtask name="Flank" />
      <subtask name="DirectAssault" />
    </task>
  </node>
</behavior>
```

**Result:** Intelligent group behavior, dynamic planning

---

## 🛠️ Implementation Roadmap

### Phase 1: Proof of Concept (2-4 hours)

**Goal:** Get ONE minion running with Behaviac

**Steps:**
1. ✅ Plugin integrated (DONE)
2. ✅ Build successful (DONE)
3. Create `ABehaviacTestMinion` (new C++ class)
4. Add `UBehaviacAgentComponent`
5. Implement 3 basic methods:
   - `HasTarget()` - Check perception
   - `MoveToTarget()` - Use nav system
   - `AttackTarget()` - Trigger GAS ability
6. Create XML behavior tree
7. Spawn in test level
8. Compare with regular Minion

**Success Metric:** Behaviac minion attacks player like original

### Phase 2: Feature Parity (4-8 hours)

**Goal:** Match all current minion behaviors

**Add:**
- Team support
- Goal system (move to objective)
- Respawn handling
- GAS integration (all abilities)
- Perception updates
- Blackboard sync

**Success Metric:** Can replace Minion_BT entirely

### Phase 3: Advanced Features (8-16 hours)

**Goal:** Demonstrate Behaviac advantages

**Add:**
- FSM for combat states
- HTN for squad tactics
- Decorator patterns (loop attack, timed patrol)
- Probabilistic behavior (random idle animations)
- Sub-tree behaviors (different minion types)

**Success Metric:** Behaviors impossible with standard UE5 BT

### Phase 4: Performance & Polish (4-8 hours)

**Goal:** Production-ready

**Tasks:**
- Profile CPU/memory usage
- Optimize tick rates
- Add visual debugging
- Documentation
- Designer-friendly XML templates
- Blueprint exposure for parameters

**Success Metric:** Performs as well or better than UE5 BT

---

## 📋 Recommended Starting Point

### Create ABehaviacTestMinion

**File:** `Source/Crunch/Private/AI/BehaviacTestMinion.h`

```cpp
#pragma once

#include "CoreMinimal.h"
#include "AI/Minion.h"
#include "BehaviacAgent.h"
#include "BehaviacTestMinion.generated.h"

/**
 * Test minion using Behaviac AI instead of UE5 Behavior Tree.
 * Inherits all Minion functionality (team, respawn, GAS) but
 * uses Behaviac for decision-making.
 */
UCLASS()
class ABehaviacTestMinion : public AMinion
{
    GENERATED_BODY()

public:
    ABehaviacTestMinion();

protected:
    virtual void BeginPlay() override;
    
public:
    virtual void Tick(float DeltaTime) override;

private:
    // Behaviac AI component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UBehaviacAgentComponent* BehaviacAgent;

    // Method handlers for Behaviac
    UFUNCTION()
    void HandleBehaviacMethod(const FString& MethodName, EBehaviacStatus& OutResult);

    // Behavior implementations
    bool CheckHasTarget();
    bool CheckInAttackRange();
    void MoveToTarget();
    void AttackTarget();
    void PatrolToGoal();

    // Cached references
    AActor* CurrentTarget;
    float AttackRange = 200.0f;
};
```

### Create Test Behavior Tree XML

**File:** `Content/AI/Behaviac/MinionTestTree.xml`

```xml
<?xml version="1.0" encoding="utf-8"?>
<behavior name="MinionTestTree" agenttype="BehaviacTestMinion" version="5">
    <node class="Selector" id="1">
        <!-- Combat Sequence -->
        <node class="Sequence" id="2">
            <node class="Condition" id="3">
                <property Method="HasTarget" />
            </node>
            <node class="Action" id="4">
                <property Method="MoveToTarget" />
            </node>
            <node class="Condition" id="5">
                <property Method="InAttackRange" />
            </node>
            <node class="Action" id="6">
                <property Method="AttackTarget" />
            </node>
        </node>
        
        <!-- Default: Patrol -->
        <node class="Action" id="7">
            <property Method="PatrolToGoal" />
        </node>
    </node>
</behavior>
```

**Logic:**
1. **Try Combat:** If has target → move → check range → attack
2. **Fallback:** If no target, patrol to goal

**Node Count:** 7 nodes (simple proof of concept)

---

## 🎯 Success Criteria

### Must Have:
- ✅ Behaviac minion spawns correctly
- ✅ Perceives and targets player
- ✅ Moves using nav system
- ✅ Attacks using GAS abilities
- ✅ Respects team affiliation
- ✅ Performance comparable to UE5 BT

### Nice to Have:
- ✅ Visual debugging in editor
- ✅ Runtime tree hot-reload
- ✅ Blueprint-exposed parameters
- ✅ Squad coordination (multiple minions)
- ✅ Designer-friendly XML templates

### Stretch Goals:
- ✅ Full FSM implementation
- ✅ HTN squad tactics
- ✅ Probabilistic behavior variations
- ✅ Per-minion personality parameters

---

## 📊 Expected Benefits

### Development Velocity:
- **Faster iteration:** Edit XML, reimport, test (no C++ recompile)
- **Non-programmer friendly:** Designers can tweak AI
- **Version control:** XML diffs vs binary .uasset

### Runtime Performance:
- **Lighter weight:** Less overhead than UE5 BT in some cases
- **Better scaling:** Efficient for many agents
- **Predictable:** No hidden Blueprint VM costs

### AI Capabilities:
- **More node types:** 41 vs ~15 base nodes
- **FSM support:** State-based AI out of the box
- **HTN planning:** Goal-driven strategic AI
- **Hybrid systems:** Mix paradigms as needed

---

## 🚀 Next Steps

### Immediate (This Session):
1. Create `BehaviacTestMinion.h/.cpp` files
2. Add to `Crunch.Build.cs` dependencies
3. Implement 5 basic methods
4. Create test XML tree
5. Spawn in test map
6. Verify basic behavior

### Short Term (Next Session):
1. Add all Minion features to Behaviac version
2. Performance profiling
3. Side-by-side comparison video
4. Documentation for team

### Long Term (Next Week):
1. Replace production Minion with Behaviac (if proven)
2. Expand to other AI actors
3. Create designer toolkit
4. Advanced behaviors (FSM, HTN)

---

## 📄 Files to Create

```
Source/Crunch/Private/AI/
├── BehaviacTestMinion.h          # New test minion class
├── BehaviacTestMinion.cpp        # Implementation
└── README_BEHAVIAC.md            # Integration notes

Content/AI/Behaviac/
├── MinionTestTree.xml            # Simple test tree
├── MinionCombatFSM.xml           # Advanced FSM example
└── MinionSquadHTN.xml            # Squad tactics HTN
```

---

## 🐶 Conclusion

**AMinion is the perfect test subject for Behaviac integration!**

**Why:**
- ✅ Simple enough to test quickly
- ✅ Complex enough to demonstrate value
- ✅ Production code (real-world validation)
- ✅ Visual feedback (easy to demo)
- ✅ Multiple instances (performance testing)
- ✅ Existing baseline (UE5 BT comparison)

**Recommendation:** Start with **Option 1 (Side-by-Side)** to build confidence, then expand to advanced features once proven.

---

**Ready to implement? Let's create `BehaviacTestMinion` and watch it come to life! 🎮**
