# Behaviac Crash Fix - Path Resolution Issue

## 🐛 The Problem

**Crash Error:**
```
LogUObjectGlobals: Warning: LoadPackage can't find package /Game/BehaviacData//Game/AI/Behaviac/MinionTestTree.
Fatal error: [File:./Runtime/CoreUObject/Private/UObject/UObjectGlobals.cpp] [Line: 1062]
```

**Root Cause:**
The Behaviac plugin **hardcodes** a base path in `BehaviacAgent.cpp`:

```cpp
// Line 77 in BehaviacAgent.cpp
FString AssetPath = FString::Printf(TEXT("/Game/BehaviacData/%s"), *RelativePath);
```

This means:
- ✅ Input: `"MinionTestTree"` → Result: `/Game/BehaviacData/MinionTestTree` ✅ CORRECT
- ❌ Input: `/Game/AI/Behaviac/MinionTestTree` → Result: `/Game/BehaviacData//Game/AI/Behaviac/MinionTestTree` ❌ **DOUBLE PATH!**

## ✅ The Fix

### Changed Files:

**1. Moved XML to expected location:**
```bash
Content/BehaviacData/MinionTestTree.xml  # NEW location
Content/AI/Behaviac/MinionTestTree.xml   # OLD location (kept for reference)
```

**2. Updated BehaviacTestMinion.h:**
```cpp
// OLD (wrong - caused double path)
FString BehaviorTreeAssetPath = "/Game/AI/Behaviac/MinionTestTree";

// NEW (correct - relative path only)
FString BehaviorTreeAssetPath = "MinionTestTree";
```

**3. Updated .gitignore:**
```gitignore
!Content/BehaviacData/
!Content/BehaviacData/**
```

## 🚀 How to Test the Fix

### Step 1: Rebuild the Project

**Option A: Live Coding (if editor open)**
```
Ctrl+Alt+F11 (Cmd+Alt+F11 on Mac)
```

**Option B: Full rebuild**
```bash
cd /Volumes/M2/Works/Crunch-master

xcodebuild -workspace "Crunch (Mac).xcworkspace" \
  -scheme "CrunchEditor" \
  -configuration Development \
  build
```

### Step 2: Reimport XML in Editor

1. **Open UE5 Editor**
2. **Content Browser** → Navigate to `Content/BehaviacData/`
3. **Right-click** on `MinionTestTree.xml` → **Reimport**
4. Should create a Behaviac behavior tree asset

### Step 3: Update Blueprint (if already created)

If you created `BP_BehaviacTestMinion` before:
1. **Open** the blueprint
2. **Details Panel** → Find "Behavior Tree Asset Path"
3. **Change to:** `MinionTestTree` (just the name, no path!)
4. **Save**

### Step 4: Test Play

1. Place `BP_BehaviacTestMinion` in level
2. Ensure Nav Mesh exists (press `P` to visualize)
3. **Press Play**
4. **Check Output Log** for success:

```
✅ LogTemp: Warning: [BehaviacTestMinion] Initializing Behaviac AI for Minion_BP_C_0
✅ LogTemp: Log: [BehaviacTestMinion] Loaded behavior tree: MinionTestTree
✅ LogTemp: Log: [BehaviacTestMinion] Method called: HasTarget
✅ LogTemp: Log: [BehaviacTestMinion] Method called: PatrolToGoal
```

**No more crash!** ✅

## 📝 Important: Path Convention

**When using Behaviac, always use relative paths:**

| ✅ CORRECT | ❌ WRONG |
|-----------|----------|
| `"MinionTestTree"` | `"/Game/BehaviacData/MinionTestTree"` |
| `"Combat/AttackTree"` | `"/Game/AI/Behaviac/MinionTestTree"` |
| `"Subfolder/MyTree"` | `"/Game/BehaviacData/Subfolder/MyTree"` |

**Why?**
The plugin **automatically** prepends `/Game/BehaviacData/` to all paths.

## 🗂️ Folder Structure

```
Content/
├── AI/
│   ├── Behaviac/
│   │   ├── MinionTestTree.xml      # Kept for reference
│   │   └── BP_BehaviacTestMinion   # Blueprint
│   └── Minions/
│       └── (original UE5 BT assets)
└── BehaviacData/                    # ⭐ Required by plugin
    └── MinionTestTree.xml           # ⭐ Active XML location
```

## 🔧 Future XML Files

**When creating new Behaviac trees:**

1. **Place XML here:**
   ```
   Content/BehaviacData/YourTreeName.xml
   ```

2. **Reference in code:**
   ```cpp
   BehaviorTreeAssetPath = "YourTreeName";  // Just the name!
   ```

3. **Organize with subfolders (optional):**
   ```
   Content/BehaviacData/Combat/AttackTree.xml
   Content/BehaviacData/Patrol/WanderTree.xml
   ```
   
   Then use:
   ```cpp
   BehaviorTreeAssetPath = "Combat/AttackTree";
   ```

## 🎯 Blueprint Property

**When editing BP_BehaviacTestMinion:**

![image](https://docs.unrealengine.com/path-to-screenshot.png)

**Details Panel:**
- **Use Behaviac AI:** ✅ true
- **Behavior Tree Asset Path:** `MinionTestTree` ⭐
  - ❌ NOT: `/Game/BehaviacData/MinionTestTree`
  - ❌ NOT: `/Game/AI/Behaviac/MinionTestTree`
  - ✅ YES: `MinionTestTree`

## 🐛 Troubleshooting

### Still crashing after rebuild?

1. **Check the property:**
   ```
   BP_BehaviacTestMinion → Behavior Tree Asset Path
   Should be: "MinionTestTree" (no path!)
   ```

2. **Verify XML location:**
   ```bash
   ls /Volumes/M2/Works/Crunch-master/Content/BehaviacData/MinionTestTree.xml
   ```

3. **Reimport XML:**
   - Content Browser → Right-click XML → Reimport

4. **Check Output Log for different error:**
   - Share the new error if it's different

### Output Log shows "Failed to find"?

Make sure:
- XML file is in `Content/BehaviacData/` ✅
- Path in blueprint is just `"MinionTestTree"` ✅
- XML was reimported after moving ✅

---

**🐶 This fix resolves the double-path crash! Rebuild and test again.**
