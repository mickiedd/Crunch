# Behaviac Path Fix - Quick Guide

## Issue
The BehaviacTestMinion was looking for the behavior tree in the wrong location:
- ❌ Old path: `/Game/BehaviacData/MinionTestTree`
- ✅ New path: `/Game/AI/Behaviac/MinionTestTree`

## What Was Fixed

**File:** `Source/Crunch/Private/AI/BehaviacTestMinion.h`

**Changed line 66:**
```cpp
// OLD
FString BehaviorTreeAssetPath = "MinionTestTree";

// NEW
FString BehaviorTreeAssetPath = "/Game/AI/Behaviac/MinionTestTree";
```

## Rebuild Instructions

**Option 1: Live Coding (Fastest)**
1. Editor should already be open
2. Press `Ctrl+Alt+F11` for Live Coding
3. Wait for "Compile Complete" notification
4. Test again!

**Option 2: Full Rebuild**
```bash
cd /Volumes/M2/Works/Crunch-master

xcodebuild -workspace "Crunch (Mac).xcworkspace" \
  -scheme "CrunchEditor" \
  -configuration Development \
  build
```

**Option 3: If Editor is Closed**
1. Build first (option 2)
2. Then open: `open Crunch.uproject`

## Expected Result

After rebuild, the Output Log should show:
```
LogTemp: Warning: [BehaviacTestMinion] Initializing Behaviac AI for Minion_BP_C_35
LogTemp: Log: [BehaviacTestMinion] Loaded behavior tree: /Game/AI/Behaviac/MinionTestTree
LogTemp: Log: [BehaviacTestMinion] Method called: HasTarget
```

No more "Failed to find object" errors!

## Blueprint Update

If you already created `BP_BehaviacTestMinion`:
1. Open the blueprint
2. Details Panel → Find "Behavior Tree Asset Path"
3. Should now show: `/Game/AI/Behaviac/MinionTestTree`
4. If not, manually set it to: `/Game/AI/Behaviac/MinionTestTree`

## Verify XML Import

Make sure the XML is properly imported:
1. Content Browser → Navigate to `Content/AI/Behaviac/`
2. You should see:
   - `MinionTestTree.xml` (with Behaviac icon)
   - `BP_BehaviacTestMinion` (blueprint)
3. If XML doesn't have Behaviac icon:
   - Right-click → Reimport
   - Should create a Behaviac behavior tree asset

---

**🐶 Path fixed! Rebuild and try again!**
