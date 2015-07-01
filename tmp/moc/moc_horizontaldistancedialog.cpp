/****************************************************************************
** HorizontalDistanceDialog meta object code from reading C++ file 'horizontaldistancedialog.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../horizontaldistancedialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *HorizontalDistanceDialog::className() const
{
    return "HorizontalDistanceDialog";
}

QMetaObject *HorizontalDistanceDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HorizontalDistanceDialog( "HorizontalDistanceDialog", &HorizontalDistanceDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HorizontalDistanceDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HorizontalDistanceDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HorizontalDistanceDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HorizontalDistanceDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HorizontalDistanceDialog::staticMetaObject()
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
	"HorizontalDistanceDialog", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HorizontalDistanceDialog.setMetaObject( metaObj );
    return metaObj;
}

void* HorizontalDistanceDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HorizontalDistanceDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool HorizontalDistanceDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: static_QUType_bool.set(_o,convert()); break;
    case 1: keyPressEvent((QKeyEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return AppDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool HorizontalDistanceDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool HorizontalDistanceDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool HorizontalDistanceDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
