#!/usr/bin/env bash

# Check if commit message is provided
if [ -z "$1" ]; then
  echo "Usage: ./run.sh \"commit message\" [branch-name]"
  exit 1
fi

# Set default branch if not provided
BRANCH=${2:-main}

# Pull latest changes
git pull origin "$BRANCH"

# Stage all changes
git add .

# Commit with provided message
git commit -m "$1"

# Push to the specified branch
git push origin "$BRANCH"
