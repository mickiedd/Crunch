# BehaviacTestMinion - Testing Guide

**Created:** 2026-02-21 22:50  
**Purpose:** Test Behaviac AI plugin with Crunch's Minion NPC system  
**Comparison:** Behaviac vs UE5 default Behavior Tree

---

## 📁 Files Created

```
Source/Crunch/Private/AI/
├── BehaviacTestMinion.h          ✅ 3.0 KB - Test minion class
└── BehaviacTestMinion.cpp        ✅ 7.2 KB - Implementation

Content/AI/Behaviac/
└── MinionTestTree.xml            ✅ 2.3 KB - Behavior tree

Source/Crunch/
└── Crunch.Build.cs               ✏️ Updated - Added BehaviacRuntime dependency
```

---

## 🏗️ Step 1: Build the Project

### Option A: Xcode Build (Recommended)

```bash
cd /Volumes/M2/Works/Crunch-master

# Clean build (optional, if you hit issues)
xcodebuild -workspace "Crunch (Mac).xcworkspace" \
  -scheme "CrunchEditor" \
  -configuration Development \
  clean

# Build
xcodebuild -workspace "Crunch (Mac).xcworkspace" \
  -scheme "CrunchEditor" \
  -configuration Development \
  build
```

**Expected Output:**
```
[XXX/XXX] Compile [Apple] BehaviacTestMinion.cpp
[XXX/XXX] Link [Apple] UnrealEditor-Crunch.dylib
** BUILD SUCCEEDED **
```

### Option B: UE5 Editor Live Coding

1. Open Crunch project in UE5 Editor
2. Make any small change to BehaviacTestMinion.cpp (add a space)
3. Press **Ctrl+Alt+F11** (Live Coding hotkey)
4. Wait for "Compile Complete" notification

---

## 🎮 Step 2: Create Test Blueprint

### In UE5 Editor:

1. **Content Browser** → Navigate to `Content/AI/Behaviac/`
2. **Right-click** → **Blueprint Class**
3. **Parent Class:** Search for `BehaviacTestMinion`
4. **Name:** `BP_BehaviacTestMinion`
5. **Open** the blueprint

### Configure the Blueprint:

In the **Details Panel:**

| Property | Value | Purpose |
|----------|-------|---------|
| **Use Behaviac AI** | ✅ True | Enable Behaviac system |
| **Behavior Tree Asset Path** | `MinionTestTree` | XML tree to load |
| **Auto Possess AI** | Place in World or Spawned | AI controller mode |

### Add Visual Mesh (Optional):

1. **Components Panel** → Select `Mesh (Inherited)`
2. **Details Panel** → **Skeletal Mesh:** Search for "Minion"
3. Choose one of the Paragon minion meshes (e.g., `SK_Minion_Melee`)
4. **Transform:** Adjust rotation if needed (usually -90° Z)

### Team Setup:

1. **Components Panel** → Select root component
2. **Details Panel** → **Generic Team Id:** Set to `1` (or different from player)

---

## 🗺️ Step 3: Create Test Map

### Quick Test Level:

1. **File** → **New Level** → **Empty Level**
2. **Save As:** `Content/Maps/BehaviacTest`

### Add Required Actors:

**Player Start:**
1. **Place Actors Panel** → Search "Player Start"
2. Drag into level
3. Position at (0, 0, 100)

**Navigation Mesh:**
1. **Place Actors Panel** → Search "Nav Mesh Bounds Volume"
2. Drag into level
3. Scale to cover playable area (e.g., 50x50x10 meters)
4. Press **P** key to visualize nav mesh (should show green)

**Lighting (Optional but recommended):**
1. **Place Actors Panel** → "Directional Light"
2. **Place Actors Panel** → "Sky Atmosphere"
3. **Place Actors Panel** → "Sky Light"

**Floor:**
1. **Place Actors Panel** → "Cube" (Basic Shape)
2. Scale: X=50, Y=50, Z=0.1
3. Position: Z=0

---

## 🤖 Step 4: Spawn Test Minion

### Method A: Place Directly

1. **Content Browser** → Find `BP_BehaviacTestMinion`
2. **Drag** into level viewport
3. Position at (500, 0, 100) - away from player start
4. In **Details Panel**, verify:
   - ✅ **Use Behaviac AI** = True
   - ✅ **Behavior Tree Asset Path** = "MinionTestTree"

### Method B: Use Minion Barrack (Advanced)

1. Find existing `MinionBarrack_BP` in `Content/AI/Minions/`
2. Duplicate it → Rename to `BP_BehaviacMinonBarrack`
3. Open blueprint
4. **Details Panel** → **Minion Class:** Set to `BP_BehaviacTestMinion`
5. Place in level and configure spawn settings

---

## 🧪 Step 5: Import XML Behavior Tree

**IMPORTANT:** UE5 needs to recognize the XML file as an asset!

### Method 1: Reimport (If XML shows as asset)

1. **Content Browser** → Navigate to `Content/AI/Behaviac/`
2. **Right-click** on `MinionTestTree.xml`
3. **Reimport**
4. Should show as Behaviac asset with icon

### Method 2: Force Recognition (If XML not visible)

1. Close UE5 Editor
2. Verify file exists at:
   ```
   /Volumes/M2/Works/Crunch-master/Content/AI/Behaviac/MinionTestTree.xml
   ```
3. Reopen editor
4. **Content Browser** → **Right-click in folder** → **Refresh**

### Method 3: Copy from BehaviacTest

```bash
# Copy working example
cp /Volumes/M2/Works/BehaviacTest/Content/AI/TestBehaviorTree.xml \
   /Volumes/M2/Works/Crunch-master/Content/AI/Behaviac/MinionTestTree.xml
```

Then modify the XML to use "MinionTestMinion" methods.

---

## ▶️ Step 6: Test!

### Launch Play Session:

1. **Select** `BehaviacTest` map (or your test map)
2. **Press Play** (Alt+P) or click **Play** button
3. **Watch** Output Log window

### Expected Output Log:

```
LogTemp: Warning: [BehaviacTestMinion] Initializing Behaviac AI for BP_BehaviacTestMinion_0
LogTemp: Log: [BehaviacTestMinion] Loaded behavior tree: MinionTestTree
LogTemp: Log: [BehaviacTestMinion] Method called: HasTarget
LogTemp: Log: [BehaviacTestMinion] Method called: PatrolToGoal
LogTemp: Log: [BehaviacTestMinion] Patrolling (no specific goal)
```

### When You Get Close:

```
LogTemp: Log: [BehaviacTestMinion] Method called: HasTarget
LogTemp: Log: [BehaviacTestMinion] Target acquired: BP_Phase_C_0
LogTemp: Log: [BehaviacTestMinion] Method called: MoveToTarget
LogTemp: Log: [BehaviacTestMinion] Moving to target: BP_Phase_C_0
LogTemp: Log: [BehaviacTestMinion] Method called: InAttackRange
LogTemp: Log: [BehaviacTestMinion] Target in range! Distance: 150.3
LogTemp: Log: [BehaviacTestMinion] Method called: AttackTarget
LogTemp: Warning: [BehaviacTestMinion] ATTACKING BP_Phase_C_0!
```

---

## 🔍 Step 7: Compare with Original Minion

### Side-by-Side Test:

1. In the same level, place:
   - **1x BP_BehaviacTestMinion** (your new AI)
   - **1x Minion_BP** (original UE5 BT minion)
2. Position them equidistant from player start
3. Press Play
4. Walk toward them

### What to Compare:

| Aspect | Behaviac Minion | Original Minion |
|--------|-----------------|-----------------|
| **Response Time** | Time to detect player | Time to detect player |
| **Movement** | Smooth pathfinding? | Smooth pathfinding? |
| **Attack Behavior** | When does it attack? | When does it attack? |
| **Output Log** | Method calls visible | BT task logs (if any) |
| **CPU Usage** | Check Profiler | Check Profiler |

### Performance Profiling:

1. **Window** → **Developer Tools** → **Session Frontend**
2. **Profiler** tab
3. Look for AI-related stats
4. Compare frame time with/without minions

---

## 🐛 Troubleshooting

### Issue: "No BehaviacTestMinion class found"

**Solution:**
- Build didn't include new files
- Run full build: `xcodebuild ... clean build`
- Or restart UE5 Editor and try Live Coding

### Issue: "BehaviorTreeAssetPath not found"

**Solution:**
- XML not imported correctly
- Check file exists at `Content/AI/Behaviac/MinionTestTree.xml`
- Try reimporting or use full path: `/Game/AI/Behaviac/MinionTestTree`

### Issue: "Minion doesn't move"

**Solution:**
- No Nav Mesh! Press **P** to visualize
- Add or resize **Nav Mesh Bounds Volume**
- Rebuild navigation: **Build** → **Build Paths**

### Issue: "Method called but nothing happens"

**Solution:**
- Check AI Controller is spawned:
  ```cpp
  // In blueprint or code
  Auto Possess AI = "Placed in World" or "Spawned"
  ```
- Verify AIController exists in Output Log

### Issue: "No target detected"

**Solution:**
- Perception not configured on AIController
- ACAIController has sight config, but BehaviacTestMinion might need its own
- Add AIPerception component in blueprint

---

## 🎯 Success Checklist

- [ ] Build succeeded with BehaviacTestMinion
- [ ] BP_BehaviacTestMinion blueprint created
- [ ] XML tree imported and visible
- [ ] Test map has nav mesh (press P to verify)
- [ ] Minion spawns without errors
- [ ] Output Log shows "Behaviac AI Initializing"
- [ ] Output Log shows "Method called: HasTarget"
- [ ] Minion detects player when close
- [ ] Minion moves toward player
- [ ] Minion attacks in range
- [ ] Behavior matches or improves on original Minion

---

## 📊 Next Steps

### After Successful Test:

1. **Document Results:**
   - Screenshot Output Log
   - Record video of behavior
   - Note any performance differences

2. **Expand Behavior:**
   - Add more methods (Flee, Block, UseAbility)
   - Create FSM for combat states
   - Test HTN for squad tactics

3. **Optimize:**
   - Adjust PropertyUpdateInterval
   - Cache perception queries
   - Profile with many minions

4. **Integration:**
   - Replace original Minion if Behaviac proves superior
   - Or use hybrid: UE5 BT + Behaviac FSM
   - Create designer-friendly templates

---

## 📝 Behavior Tree Breakdown

### Current MinionTestTree.xml Logic:

```
Root: Selector (OR)
├─ Combat Sequence (AND) ✅ Try this first
│  ├─ Condition: HasTarget?
│  ├─ Action: MoveToTarget
│  ├─ Condition: InAttackRange?
│  └─ Action: AttackTarget
└─ Action: PatrolToGoal ✅ Fallback if no target
```

### Expected Flow:

**No Target:**
```
HasTarget → FALSE → Skip Sequence → PatrolToGoal → Success
```

**Target Acquired:**
```
HasTarget → TRUE → MoveToTarget → RUNNING → (Tick) → InAttackRange → FALSE → (Loop)
```

**Target in Range:**
```
HasTarget → TRUE → MoveToTarget → SUCCESS → InAttackRange → TRUE → AttackTarget → SUCCESS
```

---

## 🔗 Related Files

- **Analysis:** `BEHAVIAC_MINION_ANALYSIS.md` - Full technical breakdown
- **Original Minion:** `Source/Crunch/Private/AI/Minion.h/.cpp`
- **AI Controller:** `Source/Crunch/Private/AI/CAIController.h/.cpp`
- **UE5 BT:** `Content/AI/Minions/Minion_BT.uasset`
- **Reference Test:** `/Volumes/M2/Works/BehaviacTest/` - Working example

---

**🐶 Ready to test! Follow the steps and let me know what you see in the Output Log!**
