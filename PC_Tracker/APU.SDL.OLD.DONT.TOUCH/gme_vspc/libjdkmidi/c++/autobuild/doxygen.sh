#! /bin/bash

source ../project.sh || exit 1

DEST_DIR="${JDKS_BUILDS_DIR}/${PROJECT_NAME}"
mkdir -p "${DEST_DIR}"

mkdir b 2>/dev/null
cd b

../../configure ${JDKS_CONFIGURE_OPTIONS} &&
  make install-dev-docs >& "${DEST_DIR}/doxygen${JDKS_AUTOBUILD_SUFFIX}.txt" 


