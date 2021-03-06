#!/bin/bash
set -e

rootdir=$(pwd)

if [[ $PLATFORM == "android_"* ]]; then
	if [[ "$PLATFORM" == "android_armv7" ]]; then
		PCODE=0
	fi
	if [[ "$PLATFORM" == "android_x86" ]]; then
		PCODE=1
	fi
	
	sed -i "s/android:versionCode=\"\([[:digit:]]*\)\"/android:versionCode=\"\1%{pcode}\"/g" gui/quick/android/AndroidManifest.xml
	sed -i "s/%{pcode}/$PCODE/g" gui/quick/android/AndroidManifest.xml
	
	sed -i "s/private void nativeReady/public void nativeReady/g" /opt/qt/$QT_VER/$PLATFORM/src/android/java/src/de/skycoder42/qtservice/AndroidService.java
	# debug
	cat /opt/qt/$QT_VER/$PLATFORM/src/android/java/src/de/skycoder42/qtservice/AndroidService.java

	pushd build-$PLATFORM
	make INSTALL_ROOT="$rootdir/install" deploy
	popd
else
	pushd build-$PLATFORM
	make INSTALL_ROOT="$rootdir/install" deploy
	make INSTALL_ROOT="$rootdir/install" package
	popd
fi

find install/
