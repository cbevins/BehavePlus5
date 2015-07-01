/****************************************************************************
** Conflict1Dialog meta object code from reading C++ file 'conflictdialog.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../conflictdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Conflict1Dialog::className() const
{
    return "Conflict1Dialog";
}

QMetaObject *Conflict1Dialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Conflict1Dialog( "Conflict1Dialog", &Conflict1Dialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Conflict1Dialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Conflict1Dialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Conflict1Dialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Conflict1Dialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Conflict1Dialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUMethod slot_0 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "store()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Conflict1Dialog", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Conflict1Dialog.setMetaObject( metaObj );
    return metaObj;
}

void* Conflict1Dialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Conflict1Dialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool Conflict1Dialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: store(); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Conflict1Dialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Conflict1Dialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool Conflict1Dialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *Conflict2Dialog::className() const
{
    return "Conflict2Dialog";
}

QMetaObject *Conflict2Dialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Conflict2Dialog( "Conflict2Dialog", &Conflict2Dialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Conflict2Dialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Conflict2Dialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Conflict2Dialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Conflict2Dialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Conflict2Dialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUMethod slot_0 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "store()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Conflict2Dialog", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Conflict2Dialog.setMetaObject( metaObj );
    return metaObj;
}

void* Conflict2Dialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Conflict2Dialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool Conflict2Dialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: store(); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Conflict2Dialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Conflict2Dialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool Conflict2Dialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
