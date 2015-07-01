/****************************************************************************
** AppDialog meta object code from reading C++ file 'appdialog.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../appdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AppDialog::className() const
{
    return "AppDialog";
}

QMetaObject *AppDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AppDialog( "AppDialog", &AppDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AppDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AppDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AppDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"clear", 0, 0 };
    static const QUMethod slot_1 = {"store", 0, 0 };
    static const QUMethod slot_2 = {"wizard", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "clear()", &slot_0, QMetaData::Protected },
	{ "store()", &slot_1, QMetaData::Protected },
	{ "wizard()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AppDialog", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AppDialog.setMetaObject( metaObj );
    return metaObj;
}

void* AppDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AppDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool AppDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: clear(); break;
    case 1: store(); break;
    case 2: wizard(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AppDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AppDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool AppDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *AppPage::className() const
{
    return "AppPage";
}

QMetaObject *AppPage::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AppPage( "AppPage", &AppPage::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AppPage::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppPage", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AppPage::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppPage", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AppPage::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QSplitter::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"browserToggled", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "on", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"pictureToggled", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "browserToggled(bool)", &slot_0, QMetaData::Protected },
	{ "pictureToggled(bool)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AppPage", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AppPage.setMetaObject( metaObj );
    return metaObj;
}

void* AppPage::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AppPage" ) )
	return this;
    return QSplitter::qt_cast( clname );
}

bool AppPage::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: browserToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 1: pictureToggled((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QSplitter::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AppPage::qt_emit( int _id, QUObject* _o )
{
    return QSplitter::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AppPage::qt_property( int id, int f, QVariant* v)
{
    return QSplitter::qt_property( id, f, v);
}

bool AppPage::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *AppTabDialog::className() const
{
    return "AppTabDialog";
}

QMetaObject *AppTabDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AppTabDialog( "AppTabDialog", &AppTabDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AppTabDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppTabDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AppTabDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AppTabDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AppTabDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "store()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"AppTabDialog", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AppTabDialog.setMetaObject( metaObj );
    return metaObj;
}

void* AppTabDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AppTabDialog" ) )
	return this;
    return QTabDialog::qt_cast( clname );
}

bool AppTabDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: store(); break;
    default:
	return QTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AppTabDialog::qt_emit( int _id, QUObject* _o )
{
    return QTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AppTabDialog::qt_property( int id, int f, QVariant* v)
{
    return QTabDialog::qt_property( id, f, v);
}

bool AppTabDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
