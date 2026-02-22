#!/bin/bash
# GitHub Push Script for Crunch Project
# Run this after creating the GitHub repository

set -e  # Exit on error

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${YELLOW}🚀 Crunch GitHub Push Script${NC}"
echo ""

# Get GitHub username
read -p "Enter your GitHub username: " GITHUB_USER

if [ -z "$GITHUB_USER" ]; then
    echo -e "${RED}Error: GitHub username is required${NC}"
    exit 1
fi

# Repository name (you can change this)
REPO_NAME="Crunch"

echo ""
echo -e "${GREEN}Repository will be created at:${NC}"
echo "https://github.com/$GITHUB_USER/$REPO_NAME"
echo ""

read -p "Continue? (y/n): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Cancelled."
    exit 0
fi

# Navigate to project
cd /Volumes/M2/Works/Crunch-master

# Check if remote already exists
if git remote | grep -q origin; then
    echo -e "${YELLOW}Remote 'origin' already exists. Removing...${NC}"
    git remote remove origin
fi

# Add remote
echo -e "${GREEN}Adding GitHub remote...${NC}"
git remote add origin "https://github.com/$GITHUB_USER/$REPO_NAME.git"

echo ""
echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"
echo -e "${YELLOW}IMPORTANT: Create the GitHub repository first!${NC}"
echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"
echo ""
echo "1. Open: https://github.com/new"
echo "2. Repository name: $REPO_NAME"
echo "3. Description: UE5 multiplayer action game with GAS and Behaviac AI"
echo "4. Visibility: Public (recommended)"
echo "5. DO NOT initialize with README, .gitignore, or license"
echo "6. Click 'Create repository'"
echo ""
read -p "Press Enter when repository is created..."

# Push to GitHub
echo ""
echo -e "${GREEN}Pushing to GitHub...${NC}"
echo ""

if git push -u origin main; then
    echo ""
    echo -e "${GREEN}✅ SUCCESS!${NC}"
    echo ""
    echo "Your project is now on GitHub:"
    echo "https://github.com/$GITHUB_USER/$REPO_NAME"
    echo ""
    echo "Next steps:"
    echo "1. Visit your repository and add topics/tags"
    echo "2. Update README.md with your personal info"
    echo "3. Consider adding a LICENSE file"
else
    echo ""
    echo -e "${RED}❌ Push failed!${NC}"
    echo ""
    echo "Common issues:"
    echo "1. Repository doesn't exist - create it at https://github.com/new"
    echo "2. Authentication failed - use Personal Access Token as password"
    echo "   Generate at: https://github.com/settings/tokens"
    echo "3. Wrong username - check spelling"
    exit 1
fi
