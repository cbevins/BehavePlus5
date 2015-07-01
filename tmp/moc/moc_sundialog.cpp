/****************************************************************************
** SunDialog meta object code from reading C++ file 'sundialog.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../sundialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SunDialog::className() const
{
    return "SunDialog";
}

QMetaObject *SunDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SunDialog( "SunDialog", &SunDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SunDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SunDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SunDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SunDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SunDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"browse", 0, 0 };
    static const QUMethod slot_1 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "browse()", &slot_0, QMetaData::Protected },
	{ "store()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"SunDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SunDialog.setMetaObject( metaObj );
    return metaObj;
}

void* SunDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SunDialog" ) )
	return this;
    return AppTabDialog::qt_cast( clname );
}

bool SunDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: browse(); break;
    case 1: store(); break;
    default:
	return AppTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SunDialog::qt_emit( int _id, QUObject* _o )
{
    return AppTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool SunDialog::qt_property( int id, int f, QVariant* v)
{
    return AppTabDialog::qt_property( id, f, v);
}

bool SunDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *GeoCatalogDialog::className() const
{
    return "GeoCatalogDialog";
}

QMetaObject *GeoCatalogDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GeoCatalogDialog( "GeoCatalogDialog", &GeoCatalogDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GeoCatalogDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GeoCatalogDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GeoCatalogDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GeoCatalogDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GeoCatalogDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuActivated", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "lvi", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"doubleClicked", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "lvi", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "column", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"rightButtonClicked", 3, param_slot_2 };
    static const QUMethod slot_3 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuActivated(int)", &slot_0, QMetaData::Protected },
	{ "doubleClicked(QListViewItem*)", &slot_1, QMetaData::Protected },
	{ "rightButtonClicked(QListViewItem*,const QPoint&,int)", &slot_2, QMetaData::Protected },
	{ "store()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"GeoCatalogDialog", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GeoCatalogDialog.setMetaObject( metaObj );
    return metaObj;
}

void* GeoCatalogDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GeoCatalogDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool GeoCatalogDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 1: doubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: rightButtonClicked((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 3: store(); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GeoCatalogDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GeoCatalogDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool GeoCatalogDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *GeoPlaceDialog::className() const
{
    return "GeoPlaceDialog";
}

QMetaObject *GeoPlaceDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GeoPlaceDialog( "GeoPlaceDialog", &GeoPlaceDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GeoPlaceDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GeoPlaceDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GeoPlaceDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GeoPlaceDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GeoPlaceDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuActivated", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "lvi", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"doubleClicked", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "lvi", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "column", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"rightButtonClicked", 3, param_slot_2 };
    static const QUMethod slot_3 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuActivated(int)", &slot_0, QMetaData::Protected },
	{ "doubleClicked(QListViewItem*)", &slot_1, QMetaData::Protected },
	{ "rightButtonClicked(QListViewItem*,const QPoint&,int)", &slot_2, QMetaData::Protected },
	{ "store()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"GeoPlaceDialog", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GeoPlaceDialog.setMetaObject( metaObj );
    return metaObj;
}

void* GeoPlaceDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GeoPlaceDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool GeoPlaceDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 1: doubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: rightButtonClicked((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 3: store(); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GeoPlaceDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GeoPlaceDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool GeoPlaceDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
