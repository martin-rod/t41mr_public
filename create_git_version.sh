#! /bin/bash
set -e
set -x

# prepoklada se verze ve formatu vMMMMM, kde MMMMM je uint cislo
# vyhleda posledni tag vMMMMM, MMMMM pouzihe jako verzi, SHA bude z aktualniho komitu
# 0 - verze pro lokalni preklad, cislovani zacina od 1

mkdir -p version
VERSION_FILE_C="version/git_version.h"
VERSION_FILE_SH="version/git_version.sh"

GIT_DATE=$(date '+%Y%m%d_%H%M%S')
GIT_SHA=$(git log --quiet --pretty=format:'%H' -n 1)
GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
GIT_TAG_VERSION=$(git describe --tags --first-parent --match 'v*' --abbrev=0)

GIT_VERSION=${GIT_TAG_VERSION:1}

echo "GIT_DATE:${GIT_DATE}"
echo "GIT_SHA:${GIT_SHA}"
echo "GIT_BRANCH:${GIT_BRANCH}"
echo "GIT_TAG_VERSION:${GIT_TAG_VERSION}"
echo "GIT_VERSION:${GIT_VERSION}"

cat << EOF >${VERSION_FILE_C}
#ifndef GIT_VERSION_H
  #define GIT_DATE "${GIT_DATE}"
  #define GIT_BRANCH "${GIT_BRANCH}"
  #define GIT_SHA "${GIT_SHA}"
  #define GIT_TAG_VERSION "${GIT_TAG_VERSION}"
  #define GIT_VERSION ${GIT_VERSION}
#endif
EOF

cat << EOF >${VERSION_FILE_SH}
export GIT_DATE="${GIT_DATE}"
export GIT_BRANCH="${GIT_BRANCH}"
export GIT_SHA="${GIT_SHA}"
export GIT_TAG_VERSION="${GIT_TAG_VERSION}"
export GIT_VERSION=${GIT_VERSION}
EOF