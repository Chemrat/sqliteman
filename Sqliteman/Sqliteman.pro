######################################################################
# Do not run qmake -project again!
######################################################################

TEMPLATE = app
TARGET =
win32:TARGET = sqliteman
DEPENDPATH += . sqliteman
INCLUDEPATH += . sqliteman
QT += sql
win32:CONFIG += windows
#win32:DEFINES += DOC_DIR=\\\"./doc/en/\\\"
win32:DEFINES += DOC_DIR=\\\"./doc/\\\"
win32:DEFINES += SQLITEMAN_VERSION=\\\"1.0\\\"
win32:DEFINES += TRANSLATION_DIR=\\\"./ts\\\"
win32:DEFINES += ICON_DIR=\\\":/icons/\\\"
win32:DEFINES += SQLITE_BINARY=\\\"sqlite3.exe\\\"
win32:DEFINES += HAVE_WIN

win32 {
    RC_FILE = sqliteman/icons/sqliteman.rc
    RESOURCES += sqliteman/icons/icons.qrc
}

# translations
TRANSLATIONS += sqliteman/ts/sqliteman_cs.ts \
                sqliteman/ts/sqliteman_de.ts \
                sqliteman/ts/sqliteman_pl.ts \
		sqliteman/ts/sqliteman_ru.ts
# Input
HEADERS += sqliteman/altertabledialog.h \
           sqliteman/altertriggerdialog.h \
           sqliteman/alterviewdialog.h \
           sqliteman/analyzedialog.h \
           sqliteman/constraintsdialog.h \
           sqliteman/createindexdialog.h \
           sqliteman/createtabledialog.h \
           sqliteman/createtriggerdialog.h \
           sqliteman/createviewdialog.h \
           sqliteman/database.h \
           sqliteman/dataexportdialog.h \
           sqliteman/dataviewer.h \
           sqliteman/helpbrowser.h \
           sqliteman/litemanwindow.h \
           sqliteman/preferencesdialog.h \
           sqliteman/queryeditordialog.h \
           sqliteman/sqleditor.h \
           sqliteman/sqlmodels.h \
           sqliteman/sqlparser.h \
           sqliteman/tableeditordialog.h \
           sqliteman/tabletree.h \
           sqliteman/vacuumdialog.h
FORMS += sqliteman/analyzedialog.ui \
         sqliteman/constraintsdialog.ui \
         sqliteman/createindexdialog.ui \
         sqliteman/createtriggerdialog.ui \
         sqliteman/createviewdialog.ui \
         sqliteman/dataexportdialog.ui \
         sqliteman/dataviewer.ui \
         sqliteman/helpbrowser.ui \
         sqliteman/preferencesdialog.ui \
         sqliteman/sqleditor.ui \
         sqliteman/tableeditordialog.ui \
         sqliteman/vacuumdialog.ui
SOURCES += sqliteman/altertabledialog.cpp \
           sqliteman/altertriggerdialog.cpp \
           sqliteman/alterviewdialog.cpp \
           sqliteman/analyzedialog.cpp \
           sqliteman/constraintsdialog.cpp \
           sqliteman/createindexdialog.cpp \
           sqliteman/createtabledialog.cpp \
           sqliteman/createtriggerdialog.cpp \
           sqliteman/createviewdialog.cpp \
           sqliteman/database.cpp \
           sqliteman/dataexportdialog.cpp \
           sqliteman/dataviewer.cpp \
           sqliteman/helpbrowser.cpp \
           sqliteman/litemanwindow.cpp \
           sqliteman/main.cpp \
           sqliteman/preferencesdialog.cpp \
           sqliteman/queryeditordialog.cpp \
           sqliteman/sqleditor.cpp \
           sqliteman/sqlmodels.cpp \
           sqliteman/sqlparser.cpp \
           sqliteman/tableeditordialog.cpp \
           sqliteman/tabletree.cpp \
           sqliteman/vacuumdialog.cpp
