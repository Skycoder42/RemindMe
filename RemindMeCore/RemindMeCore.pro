TEMPLATE = lib

QT += core gui remoteobjects
CONFIG += c++11 staticlib #important because dlls are problematic

TARGET = RemindMeCore
VERSION = $$RM_VERSION

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
	remindmeapp.h \
	maincontrol.h

SOURCES += \
	remindmeapp.cpp \
	maincontrol.cpp

REPC_REPLICA += $$fromfile(../RemindMeDaemon/rep.pri, REPC_FILES)

TRANSLATIONS += remindme_core_de.ts \
	remindme_core_template.ts

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
