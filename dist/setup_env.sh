#!/bin/bash

GHREPO="https://github.com/${TRAVIS_REPO_SLUG}"
GHREPO_USER=$(dirname  ${TRAVIS_REPO_SLUG})
GHREPO_NAME=$(basename ${TRAVIS_REPO_SLUG})
if [ "${TRAVIS_TAG}" == "" ] ; then
  PKGJSON=package_${GHREPO_USER}_${GHREPO_NAME}-${TRAVIS_BRANCH}_index.json
  ARCHIVENAME=${TRAVIS_COMMIT}
  ARCHIVEURL="https://github.com/${TRAVIS_REPO_SLUG}/archive/${ARCHIVENAME}.tar.gz"
  RELEASEVER=${TRAVIS_BRANCH}-$(date -d @`git log -1 ${TRAVIS_COMMIT} --pretty=medium --format=%ct` +%Y%m%d%H%M%S)
  BM_FORCEOPT='-f'
else
  RELEASELINE_=$(echo ${TRAVIS_TAG} | sed 's/-.*$//')
  if [ "${RELEASELINE_}" != "${TRAVIS_TAG}" ] ; then
    RELEASELINE=${RELEASELINE_}
  else
    RELEASELINE=${GHREPO_NAME}
  fi
  PKGJSON=package_${GHREPO_USER}_${RELEASELINE}_index.json
  ARCHIVENAME=${GHREPO_NAME}-${TRAVIS_TAG}
  ARCHIVEURL="https://github.com/${TRAVIS_REPO_SLUG}/releases/download/${TRAVIS_TAG}/${ARCHIVENAME}.tar.bz2"
  RELEASEVER=${TRAVIS_TAG}
  BM_FORCEOPT=
fi

echo GHREPO=${GHREPO}
echo ARCHIVENAME=${ARCHIVENAME}
echo ARCHIVEURL=${ARCHIVEURL}
echo PKGJSON=${PKGJSON}
echo RELEASEVER=${RELEASEVER}

