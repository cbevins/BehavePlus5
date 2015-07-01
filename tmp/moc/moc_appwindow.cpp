/****************************************************************************
** AppWindow meta object code from reading C++ file 'appwindow.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../appwindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AppWindow::className() const
{
    return "AppWindow";
}

QMetaObject *AppWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AppWindow( "AppWindow", &AppWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AppWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AppWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AppWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"slotDocumentClose", 0, 0 };
    static const QUMethod slot_1 = {"slotAppInit", 0, 0 };
    static const QUMethod slot_2 = {"slotConfigureAppearance", 0, 0 };
    static const QUMethod slot_3 = {"slotConfigureFuelModels", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotConfigureLanguage", 1, param_slot_4 };
    static const QUMethod slot_5 = {"slotConfigureMoistureScenarios", 0, 0 };
    static const QUMethod slot_6 = {"slotConfigureModules", 0, 0 };
    static const QUMethod slot_7 = {"slotConfigureUnitsCustom", 0, 0 };
    static const QUMethod slot_8 = {"slotConfigureUnitsEnglish", 0, 0 };
    static const QUMethod slot_9 = {"slotConfigureUnitsMetric", 0, 0 };
    static const QUMethod slot_10 = {"slotDocumentCapture", 0, 0 };
    static const QUMethod slot_11 = {"slotDocumentClear", 0, 0 };
    static const QUMethod slot_12 = {"slotDocumentClone", 0, 0 };
    static const QUMethod slot_13 = {"slotDocumentExportFuelModelsFarsite", 0, 0 };
    static const QUMethod slot_14 = {"slotDocumentExportResults", 0, 0 };
    static const QUMethod slot_15 = {"slotDocumentNew", 0, 0 };
    static const QUMethod slot_16 = {"slotDocumentOpen", 0, 0 };
    static const QUMethod slot_17 = {"slotDocumentPrint", 0, 0 };
    static const QUMethod slot_18 = {"slotDocumentReset", 0, 0 };
    static const QUMethod slot_19 = {"slotDocumentRun", 0, 0 };
    static const QUMethod slot_20 = {"slotDocumentSave", 0, 0 };
    static const QUMethod slot_21 = {"slotDocumentSaveAsFuelModel", 0, 0 };
    static const QUMethod slot_22 = {"slotDocumentSaveAsMoistureScenario", 0, 0 };
    static const QUMethod slot_23 = {"slotDocumentSaveAsRun", 0, 0 };
    static const QUMethod slot_24 = {"slotDocumentSaveAsWorksheet", 0, 0 };
    static const QUMethod slot_25 = {"slotDocumentWorkspace", 0, 0 };
    static const QUParameter param_slot_26[] = {
	{ "clone", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_26 = {"slotDocumentWorkspace", 1, param_slot_26 };
    static const QUMethod slot_27 = {"slotDocumentWorkspaceClone", 0, 0 };
    static const QUMethod slot_28 = {"slotDocumentWorkspaceNew", 0, 0 };
    static const QUMethod slot_29 = {"slotDocumentWorkspaceOpen", 0, 0 };
    static const QUMethod slot_30 = {"slotHelpAboutBehavePlus", 0, 0 };
    static const QUMethod slot_31 = {"slotHelpAboutHelp", 0, 0 };
    static const QUMethod slot_32 = {"slotHelpInstallationInfo", 0, 0 };
    static const QUMethod slot_33 = {"slotHelpMasterIndex", 0, 0 };
    static const QUMethod slot_34 = {"slotHelpUsersGuide", 0, 0 };
    static const QUMethod slot_35 = {"slotHelpVariableHelp", 0, 0 };
    static const QUMethod slot_36 = {"slotPageFirst", 0, 0 };
    static const QUMethod slot_37 = {"slotPageLast", 0, 0 };
    static const QUMethod slot_38 = {"slotPageNext", 0, 0 };
    static const QUMethod slot_39 = {"slotPagePrev", 0, 0 };
    static const QUMethod slot_40 = {"slotPagesMenuAboutToShow", 0, 0 };
    static const QUParameter param_slot_41[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_41 = {"slotPagesMenuActivated", 1, param_slot_41 };
    static const QUMethod slot_42 = {"slotQuit", 0, 0 };
    static const QUMethod slot_43 = {"slotSetStartupWorksheet", 0, 0 };
    static const QUMethod slot_44 = {"slotSplashPageShow", 0, 0 };
    static const QUMethod slot_45 = {"slotStatusUpdate", 0, 0 };
    static const QUMethod slot_46 = {"slotToolsFdfmcDialog", 0, 0 };
    static const QUMethod slot_47 = {"slotToolsHorizontalDistance", 0, 0 };
    static const QUMethod slot_48 = {"slotToolsHumidityDialog", 0, 0 };
    static const QUMethod slot_49 = {"slotToolsSlopeFromMapMeasurements", 0, 0 };
    static const QUMethod slot_50 = {"slotToolsSunMoonCalendar", 0, 0 };
    static const QUMethod slot_51 = {"slotToolsUnitsConverter", 0, 0 };
    static const QUMethod slot_52 = {"slotToolsUnitsEditor", 0, 0 };
    static const QUMethod slot_53 = {"slotViewMenuAboutToShow", 0, 0 };
    static const QUMethod slot_54 = {"slotWindowsMenuAboutToShow", 0, 0 };
    static const QUParameter param_slot_55[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_55 = {"slotWindowsMenuActivated", 1, param_slot_55 };
    static const QMetaData slot_tbl[] = {
	{ "slotDocumentClose()", &slot_0, QMetaData::Public },
	{ "slotAppInit()", &slot_1, QMetaData::Private },
	{ "slotConfigureAppearance()", &slot_2, QMetaData::Private },
	{ "slotConfigureFuelModels()", &slot_3, QMetaData::Private },
	{ "slotConfigureLanguage(int)", &slot_4, QMetaData::Private },
	{ "slotConfigureMoistureScenarios()", &slot_5, QMetaData::Private },
	{ "slotConfigureModules()", &slot_6, QMetaData::Private },
	{ "slotConfigureUnitsCustom()", &slot_7, QMetaData::Private },
	{ "slotConfigureUnitsEnglish()", &slot_8, QMetaData::Private },
	{ "slotConfigureUnitsMetric()", &slot_9, QMetaData::Private },
	{ "slotDocumentCapture()", &slot_10, QMetaData::Private },
	{ "slotDocumentClear()", &slot_11, QMetaData::Private },
	{ "slotDocumentClone()", &slot_12, QMetaData::Private },
	{ "slotDocumentExportFuelModelsFarsite()", &slot_13, QMetaData::Private },
	{ "slotDocumentExportResults()", &slot_14, QMetaData::Private },
	{ "slotDocumentNew()", &slot_15, QMetaData::Private },
	{ "slotDocumentOpen()", &slot_16, QMetaData::Private },
	{ "slotDocumentPrint()", &slot_17, QMetaData::Private },
	{ "slotDocumentReset()", &slot_18, QMetaData::Private },
	{ "slotDocumentRun()", &slot_19, QMetaData::Private },
	{ "slotDocumentSave()", &slot_20, QMetaData::Private },
	{ "slotDocumentSaveAsFuelModel()", &slot_21, QMetaData::Private },
	{ "slotDocumentSaveAsMoistureScenario()", &slot_22, QMetaData::Private },
	{ "slotDocumentSaveAsRun()", &slot_23, QMetaData::Private },
	{ "slotDocumentSaveAsWorksheet()", &slot_24, QMetaData::Private },
	{ "slotDocumentWorkspace()", &slot_25, QMetaData::Private },
	{ "slotDocumentWorkspace(bool)", &slot_26, QMetaData::Private },
	{ "slotDocumentWorkspaceClone()", &slot_27, QMetaData::Private },
	{ "slotDocumentWorkspaceNew()", &slot_28, QMetaData::Private },
	{ "slotDocumentWorkspaceOpen()", &slot_29, QMetaData::Private },
	{ "slotHelpAboutBehavePlus()", &slot_30, QMetaData::Private },
	{ "slotHelpAboutHelp()", &slot_31, QMetaData::Private },
	{ "slotHelpInstallationInfo()", &slot_32, QMetaData::Private },
	{ "slotHelpMasterIndex()", &slot_33, QMetaData::Private },
	{ "slotHelpUsersGuide()", &slot_34, QMetaData::Private },
	{ "slotHelpVariableHelp()", &slot_35, QMetaData::Private },
	{ "slotPageFirst()", &slot_36, QMetaData::Private },
	{ "slotPageLast()", &slot_37, QMetaData::Private },
	{ "slotPageNext()", &slot_38, QMetaData::Private },
	{ "slotPagePrev()", &slot_39, QMetaData::Private },
	{ "slotPagesMenuAboutToShow()", &slot_40, QMetaData::Private },
	{ "slotPagesMenuActivated(int)", &slot_41, QMetaData::Private },
	{ "slotQuit()", &slot_42, QMetaData::Private },
	{ "slotSetStartupWorksheet()", &slot_43, QMetaData::Private },
	{ "slotSplashPageShow()", &slot_44, QMetaData::Private },
	{ "slotStatusUpdate()", &slot_45, QMetaData::Private },
	{ "slotToolsFdfmcDialog()", &slot_46, QMetaData::Private },
	{ "slotToolsHorizontalDistance()", &slot_47, QMetaData::Private },
	{ "slotToolsHumidityDialog()", &slot_48, QMetaData::Private },
	{ "slotToolsSlopeFromMapMeasurements()", &slot_49, QMetaData::Private },
	{ "slotToolsSunMoonCalendar()", &slot_50, QMetaData::Private },
	{ "slotToolsUnitsConverter()", &slot_51, QMetaData::Private },
	{ "slotToolsUnitsEditor()", &slot_52, QMetaData::Private },
	{ "slotViewMenuAboutToShow()", &slot_53, QMetaData::Private },
	{ "slotWindowsMenuAboutToShow()", &slot_54, QMetaData::Private },
	{ "slotWindowsMenuActivated(int)", &slot_55, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"AppWindow", parentObject,
	slot_tbl, 56,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AppWindow.setMetaObject( metaObj );
    return metaObj;
}

void* AppWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AppWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool AppWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotDocumentClose(); break;
    case 1: slotAppInit(); break;
    case 2: slotConfigureAppearance(); break;
    case 3: slotConfigureFuelModels(); break;
    case 4: slotConfigureLanguage((int)static_QUType_int.get(_o+1)); break;
    case 5: slotConfigureMoistureScenarios(); break;
    case 6: slotConfigureModules(); break;
    case 7: slotConfigureUnitsCustom(); break;
    case 8: slotConfigureUnitsEnglish(); break;
    case 9: slotConfigureUnitsMetric(); break;
    case 10: slotDocumentCapture(); break;
    case 11: slotDocumentClear(); break;
    case 12: slotDocumentClone(); break;
    case 13: slotDocumentExportFuelModelsFarsite(); break;
    case 14: slotDocumentExportResults(); break;
    case 15: slotDocumentNew(); break;
    case 16: slotDocumentOpen(); break;
    case 17: slotDocumentPrint(); break;
    case 18: slotDocumentReset(); break;
    case 19: slotDocumentRun(); break;
    case 20: slotDocumentSave(); break;
    case 21: slotDocumentSaveAsFuelModel(); break;
    case 22: slotDocumentSaveAsMoistureScenario(); break;
    case 23: slotDocumentSaveAsRun(); break;
    case 24: slotDocumentSaveAsWorksheet(); break;
    case 25: slotDocumentWorkspace(); break;
    case 26: slotDocumentWorkspace((bool)static_QUType_bool.get(_o+1)); break;
    case 27: slotDocumentWorkspaceClone(); break;
    case 28: slotDocumentWorkspaceNew(); break;
    case 29: slotDocumentWorkspaceOpen(); break;
    case 30: slotHelpAboutBehavePlus(); break;
    case 31: slotHelpAboutHelp(); break;
    case 32: slotHelpInstallationInfo(); break;
    case 33: slotHelpMasterIndex(); break;
    case 34: slotHelpUsersGuide(); break;
    case 35: slotHelpVariableHelp(); break;
    case 36: slotPageFirst(); break;
    case 37: slotPageLast(); break;
    case 38: slotPageNext(); break;
    case 39: slotPagePrev(); break;
    case 40: slotPagesMenuAboutToShow(); break;
    case 41: slotPagesMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 42: slotQuit(); break;
    case 43: slotSetStartupWorksheet(); break;
    case 44: slotSplashPageShow(); break;
    case 45: slotStatusUpdate(); break;
    case 46: slotToolsFdfmcDialog(); break;
    case 47: slotToolsHorizontalDistance(); break;
    case 48: slotToolsHumidityDialog(); break;
    case 49: slotToolsSlopeFromMapMeasurements(); break;
    case 50: slotToolsSunMoonCalendar(); break;
    case 51: slotToolsUnitsConverter(); break;
    case 52: slotToolsUnitsEditor(); break;
    case 53: slotViewMenuAboutToShow(); break;
    case 54: slotWindowsMenuAboutToShow(); break;
    case 55: slotWindowsMenuActivated((int)static_QUType_int.get(_o+1)); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AppWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AppWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool AppWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
