/****************************************************************************
** UnitsConverterDialog meta object code from reading C++ file 'unitsconverterdialog.h'
**
** Created: Thu Apr 29 11:04:21 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../unitsconverterdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *UnitsConverterDialog::className() const
{
    return "UnitsConverterDialog";
}

QMetaObject *UnitsConverterDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UnitsConverterDialog( "UnitsConverterDialog", &UnitsConverterDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UnitsConverterDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UnitsConverterDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UnitsConverterDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UnitsConverterDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UnitsConverterDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_0 = {"convert", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "e", &static_QUType_ptr, "QKeyEvent", QUParameter::In }
    };
    static const QUMethod slot_1 = {"keyPressEvent", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "convert()", &slot_0, QMetaData::Protected },
	{ "keyPressEvent(QKeyEvent*)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"UnitsConverterDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UnitsConverterDialog.setMetaObject( metaObj );
    return metaObj;
}

void* UnitsConverterDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UnitsConverterDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool UnitsConverterDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: static_QUType_bool.set(_o,convert()); break;
    case 1: keyPressEvent((QKeyEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UnitsConverterDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UnitsConverterDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool UnitsConverterDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
