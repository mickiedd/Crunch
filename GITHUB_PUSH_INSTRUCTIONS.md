# 🚀 Push Crunch to GitHub - Instructions

**Status:** ✅ Local git repository initialized and committed  
**Files:** 249 files, 26,934 lines of code  
**Commit:** "Initial commit: Crunch UE5 game with Behaviac AI integration"

---

## 📋 Step 1: Create GitHub Repository

### Option A: Via Web Browser (Easiest)

1. **Go to:** https://github.com/new

2. **Repository Settings:**
   - **Repository name:** `Crunch` (or `Crunch-UE5-Behaviac`)
   - **Description:** `UE5 multiplayer action game with Gameplay Ability System and Behaviac AI plugin`
   - **Visibility:** 
     - ✅ **Public** (recommended for portfolio)
     - ⚠️ **Private** (if you want to keep it secret)
   - **Initialize repository:** 
     - ❌ **DO NOT** check "Add a README file"
     - ❌ **DO NOT** add .gitignore
     - ❌ **DO NOT** choose a license
     _(We already have these files!)_

3. **Click:** "Create repository"

4. **Copy the repository URL** shown on the next page:
   ```
   https://github.com/YOUR_USERNAME/Crunch.git
   ```

---

## 📋 Step 2: Push to GitHub

### Run these commands:

```bash
cd /Volumes/M2/Works/Crunch-master

# Add GitHub remote
git remote add origin https://github.com/YOUR_USERNAME/Crunch.git

# Push to GitHub
git push -u origin main
```

**Replace `YOUR_USERNAME` with your actual GitHub username!**

---

## 🔐 Authentication

When you push, GitHub will ask for credentials:

### Option A: Personal Access Token (Recommended)

1. **Generate token:**
   - Go to: https://github.com/settings/tokens
   - Click: "Generate new token (classic)"
   - Select scopes: `repo` (Full control of private repositories)
   - Generate and **copy the token** (you won't see it again!)

2. **Use token as password:**
   ```
   Username: YOUR_USERNAME
   Password: <paste your token here>
   ```

### Option B: SSH (More secure, requires setup)

1. **Generate SSH key:**
   ```bash
   ssh-keygen -t ed25519 -C "mickiedd@gmail.com"
   ```

2. **Add to GitHub:**
   - Copy: `cat ~/.ssh/id_ed25519.pub`
   - Go to: https://github.com/settings/keys
   - Click: "New SSH key"
   - Paste your public key

3. **Use SSH URL instead:**
   ```bash
   git remote set-url origin git@github.com:YOUR_USERNAME/Crunch.git
   git push -u origin main
   ```

---

## ✅ Verification

After pushing, you should see:

```
Enumerating objects: 269, done.
Counting objects: 100% (269/269), done.
Delta compression using up to 8 threads
Compressing objects: 100% (242/242), done.
Writing objects: 100% (269/269), XXX.XX MiB | X.XX MiB/s, done.
Total 269 (delta 45), reused 0 (delta 0)
To https://github.com/YOUR_USERNAME/Crunch.git
 * [new branch]      main -> main
Branch 'main' set up to track remote branch 'main' from 'origin'.
```

**Check on GitHub:**
- Go to: `https://github.com/YOUR_USERNAME/Crunch`
- You should see all your files!
- README.md should render nicely with project description

---

## 📊 What's Included in the Push

### Source Code (C++)
- ✅ 249 files committed
- ✅ All C++ classes (AI, GAS, Player, Widgets)
- ✅ BehaviacTestMinion implementation
- ✅ BehaviacPlugin source code (full plugin!)

### Configuration
- ✅ Crunch.uproject
- ✅ Config/ folder (all .ini files)
- ✅ .gitignore (excludes Binaries, Intermediate, etc.)
- ✅ Crunch.code-workspace (VSCode setup)

### Documentation (50KB)
- ✅ README.md (8.5KB) - Main project description
- ✅ BEHAVIAC_MINION_ANALYSIS.md (14KB)
- ✅ BEHAVIAC_TEST_INSTRUCTIONS.md (10KB)
- ✅ BEHAVIAC_QUICKREF.md (5.4KB)
- ✅ BEHAVIAC_IMPLEMENTATION_COMPLETE.md (9.8KB)
- ✅ BUILD_SUCCESS.md (6.6KB)
- ✅ BEHAVIAC_INTEGRATION.md

### Behaviac Content
- ✅ BehaviacPlugin source (41 BT nodes, FSM, HTN)
- ✅ MinionTestTree.xml (test behavior tree)
- ✅ BP_BehaviacTestMinion.uasset (if exists)

### NOT Included (via .gitignore)
- ❌ Binaries/ - Too large, users will build themselves
- ❌ Intermediate/ - Build artifacts
- ❌ Saved/ - User-specific saves
- ❌ DerivedDataCache/ - Cache files
- ❌ Large content (ParagonMinions, ParagonCrunch, etc.)
- ❌ .xcodeproj, .sln - Generated files

---

## 🎯 After Push Success

### Update README with your info:

```bash
cd /Volumes/M2/Works/Crunch-master

# Edit README.md
code README.md  # or use any editor

# Find and replace:
# - [Your Name] → Your actual name
# - [Add your license here] → MIT, GPL, etc.
# - [Add your fork URL] → Your BehaviorU fork (if any)
# - [Add your links] → Portfolio, Twitter, etc.

# Commit changes
git add README.md
git commit -m "docs: Update README with personal info"
git push
```

### Add GitHub Repository Settings:

1. **Topics/Tags** (for discoverability):
   - Go to: `https://github.com/YOUR_USERNAME/Crunch`
   - Click: ⚙️ (gear icon) next to "About"
   - Add topics: `unreal-engine`, `ue5`, `cpp`, `game-development`, `ai`, `behaviac`, `gameplay-ability-system`, `multiplayer`, `third-person-shooter`

2. **Description:**
   ```
   UE5 multiplayer action game with Gameplay Ability System and Behaviac AI plugin
   ```

3. **Website:** (optional)
   - Your portfolio site
   - Demo video on YouTube
   - Itch.io page

---

## 🎮 For Others to Clone and Build

Once pushed, anyone can clone and build:

```bash
# Clone
git clone https://github.com/YOUR_USERNAME/Crunch.git
cd Crunch

# macOS: Generate project files
/Path/To/UE_5.5/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh \
  -project="Crunch.uproject"

# Build
xcodebuild -workspace "Crunch (Mac).xcworkspace" \
  -scheme "CrunchEditor" \
  -configuration Development \
  build

# Open editor
open Crunch.uproject
```

**Note:** They'll need UE5.5 installed and configured!

---

## 📝 Recommended Next Commits

### Add License

```bash
# Create LICENSE file (example: MIT)
cat > LICENSE << 'EOF'
MIT License

Copyright (c) 2026 YOUR_NAME

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
EOF

git add LICENSE
git commit -m "docs: Add MIT license"
git push
```

### Add CONTRIBUTING.md

```bash
cat > CONTRIBUTING.md << 'EOF'
# Contributing to Crunch

Thank you for your interest in contributing!

## How to Contribute

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Code Style

- Follow UE5 coding standards
- Use tabs for indentation (matching .editorconfig)
- Comment complex logic
- Update documentation for new features
EOF

git add CONTRIBUTING.md
git commit -m "docs: Add contributing guidelines"
git push
```

---

## 🐛 Troubleshooting

### Error: "remote origin already exists"

```bash
git remote remove origin
git remote add origin https://github.com/YOUR_USERNAME/Crunch.git
```

### Error: "src refspec main does not exist"

You might be on `master` branch instead:

```bash
git branch -M main  # Rename to main
git push -u origin main
```

### Error: "Repository size too large"

Check if large files leaked in:

```bash
# Find large files
find . -type f -size +10M -not -path "./.git/*"

# If found, remove from git history (careful!)
git rm --cached path/to/large/file
git commit --amend
```

### Authentication Failed

Use Personal Access Token instead of password!
- Generate at: https://github.com/settings/tokens
- Use token as password when prompted

---

## 🎉 Success Checklist

After successful push, verify:

- [ ] Repository shows on GitHub
- [ ] README.md renders with project info
- [ ] All documentation files visible
- [ ] Source/ folder structure correct
- [ ] Plugins/BehaviacPlugin/ included
- [ ] .gitignore working (no Binaries/, Intermediate/)
- [ ] Commit message clear and descriptive
- [ ] Repository topics/tags added
- [ ] Description set

---

**🐶 Ready to push! Run the commands above and your project will be on GitHub!**

Let me know if you need help with any step!
