#!/bin/bash
BA2TOOLCHAIN_VERSION=$(ruby dist/ci_ba2toolchainversion.rb  dist/platform_template.json)
echo ${BA2TOOLCHAIN_VERSION}
curl -l -L $(ruby dist/ci_toolurl.rb dist/tool_template.json -n BA2-toolchain -v ${BA2TOOLCHAIN_VERSION})  | tar xfj - -C ~
echo intiki.BA2-toolchain=${BA2TOOLCHAIN_VERSION} >> ~/BA2-toolchain/builtin_tools_versions.txt

INTIKI_VERSION=$(ruby dist/ci_intikiversion.rb  dist/platform_template.json)
echo ${INTIKI_VERSION}
curl -l -L $(ruby dist/ci_toolurl.rb dist/tool_template.json -n intiki -v ${INTIKI_VERSION})  | tar xfj - -C ~
echo intiki.intiki=${INTIKI_VERSION} >> ~/intiki/builtin_tools_versions.txt
mkdir -p ~/packages/intiki/hardware/intiki/
ln -s $PWD ~/packages/intiki/hardware/intiki/`basename $PWD`

(cd ~; curl -l -L https://mono-wireless.com/download/SDK/MWSDK_201805/MWSDK_Linux-i386_201805.tgz | tar xvfz -)

