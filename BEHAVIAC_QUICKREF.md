# BehaviacTestMinion - Quick Reference

## 🚀 Quick Start (5 Minutes)

```bash
# 1. Build
cd /Volumes/M2/Works/Crunch-master
xcodebuild -workspace "Crunch (Mac).xcworkspace" -scheme "CrunchEditor" -configuration Development build

# 2. Open project
open Crunch.uproject

# 3. In Editor:
# - Create Blueprint from BehaviacTestMinion
# - Set "Use Behaviac AI" = true
# - Set "Behavior Tree Asset Path" = "MinionTestTree"
# - Drag into level with Nav Mesh
# - Press Play

# 4. Watch Output Log for:
# "Method called: HasTarget", "Moving to target", "ATTACKING"
```

---

## 📋 Implementation Summary

### Files Created:
```
✅ BehaviacTestMinion.h      - 3.0 KB
✅ BehaviacTestMinion.cpp    - 7.2 KB
✅ MinionTestTree.xml        - 2.3 KB
✅ Crunch.Build.cs           - Updated
```

### Class Hierarchy:
```
ACharacter (UE5)
  └─ ACCharacter (Crunch base)
      └─ AMinion (Team + Goal + GAS)
          └─ ABehaviacTestMinion ⭐ NEW
              ├─ UBehaviacAgentComponent
              └─ Method handlers
```

---

## 🎯 Key Methods Implemented

| Method | Type | Purpose | Return |
|--------|------|---------|--------|
| **HasTarget** | Condition | Check if enemy perceived | Success/Failure |
| **InAttackRange** | Condition | Distance check (200 units) | Success/Failure |
| **MoveToTarget** | Action | AI movement to target | Running/Success/Failure |
| **AttackTarget** | Action | Trigger GAS Combo ability | Success/Failure |
| **PatrolToGoal** | Action | Idle behavior | Success |

---

## 🌲 Behavior Tree Logic

```
Selector (Pick first success)
├─ Sequence (Do all in order)
│  ├─ HasTarget? ──┐
│  ├─ MoveToTarget │ Combat Loop
│  ├─ InAttackRange│
│  └─ AttackTarget ┘
└─ PatrolToGoal (Fallback)
```

**Simple rule:** If see enemy → chase and attack. Else → patrol.

---

## 🎨 Properties Exposed to XML

| Property | Type | Updated | Usage |
|----------|------|---------|-------|
| **Health** | int | Every 0.2s | Future: Health-based decisions |
| **HasTarget** | bool | Every 0.2s | Quick target check |
| **DistanceToTarget** | float | Every 0.2s | Range calculations |
| **IsMoving** | bool | Every 0.2s | Animation triggers |

---

## 🔧 Configuration Properties

| Blueprint Property | Default | Description |
|-------------------|---------|-------------|
| **bUseBehaviacAI** | true | Toggle Behaviac vs UE5 BT |
| **BehaviorTreeAssetPath** | "MinionTestTree" | XML tree name |
| **AttackRange** | 200.0 | Melee attack distance |
| **MoveAcceptanceRadius** | 100.0 | Nav movement tolerance |
| **PropertyUpdateInterval** | 0.2 | How often to sync state |

---

## 📊 Expected Performance

### Single Minion:
- **Tick cost:** ~0.05ms (property updates)
- **Method calls:** 2-5 per second (depending on state)
- **Memory:** ~5KB overhead (Behaviac component)

### 10 Minions:
- **Total cost:** ~0.5ms tick time
- **Comparable** to UE5 BT system
- **Advantage:** XML editing without rebuild

### 100 Minions:
- **Need profiling!** Test recommended
- Behaviac may outperform due to simpler execution model

---

## 🐛 Common Issues & Quick Fixes

| Issue | Quick Fix |
|-------|-----------|
| Class not found | Rebuild: `xcodebuild ... clean build` |
| XML not loading | Check path: `/Game/AI/Behaviac/MinionTestTree` |
| No movement | Add Nav Mesh Bounds Volume |
| No perception | Verify AIController spawned |
| Methods not called | Check Output Log for errors |
| Build errors | Check BehaviacRuntime in .Build.cs |

---

## 🎯 What to Look For

### Success Indicators:
✅ Output Log shows "Behaviac AI Initializing"  
✅ Methods called in sequence (visible in log)  
✅ Minion chases player when spotted  
✅ Minion attacks when in range  
✅ Behavior comparable to original Minion  

### Comparison Points:
📊 Response time (perception → action)  
📊 Movement smoothness  
📊 Attack timing  
📊 CPU usage (profiler)  
📊 Ease of editing (XML vs Blueprint)  

---

## 🚀 After Testing

### If Successful:
1. ✅ Document results
2. ✅ Expand to more methods
3. ✅ Create FSM for combat states
4. ✅ Test squad tactics (HTN)
5. ✅ Consider replacing original Minion

### If Issues Found:
1. Check Output Log for errors
2. Compare with BehaviacTest project (working example)
3. Verify Nav Mesh (press P)
4. Check AIController configuration
5. Review method implementations

---

## 📚 Resources

**Documentation:**
- `BEHAVIAC_MINION_ANALYSIS.md` - Full technical analysis (14KB)
- `BEHAVIAC_TEST_INSTRUCTIONS.md` - Detailed guide (10KB)
- `BEHAVIAC_INTEGRATION.md` - Plugin integration details

**Reference Code:**
- `/Volumes/M2/Works/BehaviacTest/` - Working test project
- `Source/Crunch/Private/AI/Minion.h/.cpp` - Original minion
- `Content/AI/Minions/Minion_BT.uasset` - Original UE5 BT

**Original Plugin:**
- `/Volumes/M2/Works/BehaviorU/` - Source repository
- 41 node types in `BehaviorTree/` folder
- FSM and HTN examples available

---

## 💡 Tips

**Debugging:**
```cpp
// Add to method handlers for visibility
UE_LOG(LogTemp, Warning, TEXT("[Behaviac] %s called!"), *MethodName);
```

**XML Hot Reload:**
1. Edit MinionTestTree.xml
2. Content Browser → Right-click → Reimport
3. PIE (Play in Editor) should pick up changes
4. No code rebuild needed! ✨

**Toggle Systems:**
```cpp
// In blueprint or at runtime
bUseBehaviacAI = false;  // Use original UE5 BT
bUseBehaviacAI = true;   // Use Behaviac
```

---

**🐶 Quick reference complete! Build, test, and let me know the results!**
