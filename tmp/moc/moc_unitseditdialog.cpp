/****************************************************************************
** UnitsEditDialog meta object code from reading C++ file 'unitseditdialog.h'
**
** Created: Thu Apr 29 11:04:21 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../unitseditdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UnitsEditDialog::className() const
{
    return "UnitsEditDialog";
}

QMetaObject *UnitsEditDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UnitsEditDialog( "UnitsEditDialog", &UnitsEditDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UnitsEditDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UnitsEditDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UnitsEditDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UnitsEditDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UnitsEditDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppTabDialog::staticMetaObject();
    static const QUMethod slot_0 = {"store", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "store()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"UnitsEditDialog", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UnitsEditDialog.setMetaObject( metaObj );
    return metaObj;
}

void* UnitsEditDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UnitsEditDialog" ) )
	return this;
    return AppTabDialog::qt_cast( clname );
}

bool UnitsEditDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: store(); break;
    default:
	return AppTabDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UnitsEditDialog::qt_emit( int _id, QUObject* _o )
{
    return AppTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UnitsEditDialog::qt_property( int id, int f, QVariant* v)
{
    return AppTabDialog::qt_property( id, f, v);
}

bool UnitsEditDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
