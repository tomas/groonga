#!/bin/bash

set -e
set -u

: ${DOCKER:=}
: ${TARGET:=}

if [ -n "${DOCKER}" ]; then
  docker run \
         --interactive \
         --tty \
          groonga/groonga-${DOCKER}
  exit $?
fi

if [ -n "${TARGET}" ]; then
  make \
    -C packages/${TARGET} \
    build \
    DEBUG_BUILD=yes \
    ARCHITECTURES=${ARCHITECTURES}
  exit $?
fi

: ${ENABLE_MRUBY:=no}
: ${TEST_TARGET:=all}

prefix=/tmp/local

command_test_options="--reporter=mark --timeout=60"

set -x

export COLUMNS=79

retry()
{
  local i=0
  while ! "$@"; do
    if [ $i -eq 3 ]; then
      exit 1
    fi
    i=$((i + 1))
  done
}

if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
  memory_fs_size=$[768 * 1024 * 1024] # 768MiB
  byte_per_sector=512
  n_sectors=$[${memory_fs_size} / ${byte_per_sector}]
  memory_fs_device_path=$(hdid -nomount ram://${n_sectors})
  newfs_hfs ${memory_fs_device_path}
  mkdir -p tmp
  mount -t hfs ${memory_fs_device_path} tmp

  command_test_options="${command_test_options} --n-workers=2"
  # if [ "${TEST_TARGET}" = "command" ]; then
  #   command_test_options="${command_test_options} --no-suppress-backtrace"
  # fi
else
  command_test_options="${command_test_options} --n-workers=4"
fi

case "${BUILD_TOOL}" in
  autotools)
    if [ "${TRAVIS_OS_NAME}" != "osx" ]; then
      test/unit/run-test.sh -v v
    fi
    test/command/run-test.sh ${command_test_options}
    if [ "${TRAVIS_OS_NAME}" != "osx" -a "${ENABLE_MRUBY}" = "yes" ]; then
      test/mruby/run-test.rb
      test/command_line/run-test.rb
    fi
    retry test/command/run-test.sh ${command_test_options} \
          --interface http
    if [ "${TRAVIS_OS_NAME}" != "osx" ]; then
      mkdir -p ${prefix}/var/log/groonga/httpd
      retry test/command/run-test.sh ${command_test_options} \
            --testee groonga-httpd
    fi
    ;;
  cmake)
    test/command/run-test.sh ${command_test_options}
    ;;
esac
