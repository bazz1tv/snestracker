#! /bin/bash

source ../project.sh || exit 1

mkdir b 2>/dev/null
cd b

DEST_DIR="${JDKS_BUILDS_DIR}/${PROJECT_NAME}"
mkdir -p "${DEST_DIR}"

../../configure ${JDKS_CONFIGURE_OPTIONS} &&
  make install-dev >& "${DEST_DIR}/autobuild${JDKS_AUTOBUILD_SUFFIX}.txt" 

