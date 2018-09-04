#!/bin/bash

GHREPO="https://github.com/${TRAVIS_REPO_SLUG}"
GHREPO_USER=$(dirname  ${TRAVIS_REPO_SLUG})
GHREPO_NAME=$(basename ${TRAVIS_REPO_SLUG})
if [ "${TRAVIS_TAG}" == "" ] ; then
  PKGJSON=package_${GHREPO_USER}_${GHREPO_NAME}-${TRAVIS_BRANCH}_index.json
  ARCHIVENAME=${TRAVIS_COMMIT}
  ARCHIVEURL="https://github.com/${TRAVIS_REPO_SLUG}/archive/${ARCHIVENAME}.tar.gz"
  RELEASEVER=${TRAVIS_BRANCH}-0.0.$(date -d @`git log -1 ${TRAVIS_COMMIT} --pretty=medium --format=%ct` +%y%m%d%H%M)
  BM_FORCEOPT='-f'
else
  RELEASELINE=${GHREPO_NAME}
  RELEASELINE_=$(echo ${TRAVIS_TAG} | sed 's/-.*$//')
  if [[ $TRAVIS_TAG =~ ^v[0-9]+\.[0-9]+\.[0-9]+$ ]] ; then
    RELEASELINE="intiki"
  elif [ "${RELEASELINE_}" != "${TRAVIS_TAG}" ] ; then
    RELEASELINE=${RELEASELINE_}
  fi

  PKGJSON=package_${GHREPO_USER}_${RELEASELINE}_index.json
  ARCHIVENAME=${GHREPO_NAME}-${TRAVIS_TAG}
  ARCHIVEURL="https://github.com/${TRAVIS_REPO_SLUG}/releases/download/${TRAVIS_TAG}/${ARCHIVENAME}.tar.bz2"
  RELEASEVER=$(echo ${TRAVIS_TAG} | rev | sed 's/^\([0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*\).*$/\1/' | rev )
  BM_FORCEOPT=
fi

echo GHREPO=${GHREPO}
echo ARCHIVENAME=${ARCHIVENAME}
echo ARCHIVEURL=${ARCHIVEURL}
echo PKGJSON=${PKGJSON}
echo RELEASEVER=${RELEASEVER}

