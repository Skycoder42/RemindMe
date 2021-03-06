TEMPLATE = app

TARGET = $${PROJECT_TARGET}_service

QT += androidextras mvvmcore service

HEADERS += \
	syremservice.h \
	androidscheduler.h \
	androidnotifier.h

SOURCES += main.cpp \
	syremservice.cpp \
	androidscheduler.cpp \
	androidnotifier.cpp

RESOURCES += ../../gui/quick/syrem_quick_android.qrc

TRANSLATIONS += syrem_daemon_de.ts \
	syrem_daemon_template.ts

DISTFILES += \
	$$TRANSLATIONS \
    syremd.tsdict

include(../../install.pri)

qpmx_ts_target.path = $$INSTALL_TRANSLATIONS
INSTALLS += qpmx_ts_target

# link against main lib
include(../../lib.pri)

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
