/****************************************************************************
** VarCheckBox meta object code from reading C++ file 'varcheckbox.h'
**
** Created: Thu Apr 29 11:04:21 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../varcheckbox.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *VarCheckBox::className() const
{
    return "VarCheckBox";
}

QMetaObject *VarCheckBox::metaObj = 0;
static QMetaObjectCleanUp cleanUp_VarCheckBox( "VarCheckBox", &VarCheckBox::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString VarCheckBox::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "VarCheckBox", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString VarCheckBox::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "VarCheckBox", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* VarCheckBox::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QCheckBox::staticMetaObject();
    static const QUMethod slot_0 = {"timerDone", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "timerDone()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"VarCheckBox", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_VarCheckBox.setMetaObject( metaObj );
    return metaObj;
}

void* VarCheckBox::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "VarCheckBox" ) )
	return this;
    return QCheckBox::qt_cast( clname );
}

bool VarCheckBox::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: timerDone(); break;
    default:
	return QCheckBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool VarCheckBox::qt_emit( int _id, QUObject* _o )
{
    return QCheckBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool VarCheckBox::qt_property( int id, int f, QVariant* v)
{
    return QCheckBox::qt_property( id, f, v);
}

bool VarCheckBox::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
