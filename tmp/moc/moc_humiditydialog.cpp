/****************************************************************************
** HumidityDialog meta object code from reading C++ file 'humiditydialog.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../humiditydialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *HumidityDialog::className() const
{
    return "HumidityDialog";
}

QMetaObject *HumidityDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HumidityDialog( "HumidityDialog", &HumidityDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HumidityDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HumidityDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HumidityDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HumidityDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HumidityDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppTabDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"HumidityDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HumidityDialog.setMetaObject( metaObj );
    return metaObj;
}

void* HumidityDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HumidityDialog" ) )
	return this;
    return AppTabDialog::qt_cast( clname );
}

bool HumidityDialog::qt_invoke( int _id, QUObject* _o )
{
    return AppTabDialog::qt_invoke(_id,_o);
}

bool HumidityDialog::qt_emit( int _id, QUObject* _o )
{
    return AppTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool HumidityDialog::qt_property( int id, int f, QVariant* v)
{
    return AppTabDialog::qt_property( id, f, v);
}

bool HumidityDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *HumidityPage::className() const
{
    return "HumidityPage";
}

QMetaObject *HumidityPage::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HumidityPage( "HumidityPage", &HumidityPage::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HumidityPage::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HumidityPage", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HumidityPage::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HumidityPage", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HumidityPage::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppPage::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_0 = {"dbChanged1", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_1 = {"dbChanged2", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_2 = {"dbChanged3", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_3 = {"dpChanged3", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_4 = {"seChanged1", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_5 = {"seChanged2", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_6 = {"wbChanged1", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_7 = {"wdChanged2", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_8 = {"useEnglish", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_9 = {"useMetric", 1, param_slot_9 };
    static const QMetaData slot_tbl[] = {
	{ "dbChanged1()", &slot_0, QMetaData::Protected },
	{ "dbChanged2()", &slot_1, QMetaData::Protected },
	{ "dbChanged3()", &slot_2, QMetaData::Protected },
	{ "dpChanged3()", &slot_3, QMetaData::Protected },
	{ "seChanged1()", &slot_4, QMetaData::Protected },
	{ "seChanged2()", &slot_5, QMetaData::Protected },
	{ "wbChanged1()", &slot_6, QMetaData::Protected },
	{ "wdChanged2()", &slot_7, QMetaData::Protected },
	{ "useEnglish()", &slot_8, QMetaData::Protected },
	{ "useMetric()", &slot_9, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"HumidityPage", parentObject,
	slot_tbl, 10,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HumidityPage.setMetaObject( metaObj );
    return metaObj;
}

void* HumidityPage::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HumidityPage" ) )
	return this;
    return AppPage::qt_cast( clname );
}

bool HumidityPage::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: static_QUType_bool.set(_o,dbChanged1()); break;
    case 1: static_QUType_bool.set(_o,dbChanged2()); break;
    case 2: static_QUType_bool.set(_o,dbChanged3()); break;
    case 3: static_QUType_bool.set(_o,dpChanged3()); break;
    case 4: static_QUType_bool.set(_o,seChanged1()); break;
    case 5: static_QUType_bool.set(_o,seChanged2()); break;
    case 6: static_QUType_bool.set(_o,wbChanged1()); break;
    case 7: static_QUType_bool.set(_o,wdChanged2()); break;
    case 8: static_QUType_bool.set(_o,useEnglish()); break;
    case 9: static_QUType_bool.set(_o,useMetric()); break;
    default:
	return AppPage::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool HumidityPage::qt_emit( int _id, QUObject* _o )
{
    return AppPage::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool HumidityPage::qt_property( int id, int f, QVariant* v)
{
    return AppPage::qt_property( id, f, v);
}

bool HumidityPage::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
