/****************************************************************************
** RealSpinBox meta object code from reading C++ file 'realspinbox.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../realspinbox.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RealSpinBox::className() const
{
    return "RealSpinBox";
}

QMetaObject *RealSpinBox::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RealSpinBox( "RealSpinBox", &RealSpinBox::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RealSpinBox::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RealSpinBox", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RealSpinBox::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RealSpinBox", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RealSpinBox::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QSpinBox::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"RealSpinBox", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RealSpinBox.setMetaObject( metaObj );
    return metaObj;
}

void* RealSpinBox::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RealSpinBox" ) )
	return this;
    return QSpinBox::qt_cast( clname );
}

bool RealSpinBox::qt_invoke( int _id, QUObject* _o )
{
    return QSpinBox::qt_invoke(_id,_o);
}

bool RealSpinBox::qt_emit( int _id, QUObject* _o )
{
    return QSpinBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RealSpinBox::qt_property( int id, int f, QVariant* v)
{
    return QSpinBox::qt_property( id, f, v);
}

bool RealSpinBox::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
