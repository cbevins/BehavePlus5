/****************************************************************************
** RunDialog meta object code from reading C++ file 'rundialog.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../rundialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RunDialog::className() const
{
    return "RunDialog";
}

QMetaObject *RunDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RunDialog( "RunDialog", &RunDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RunDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RunDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RunDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RunDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RunDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"RunDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RunDialog.setMetaObject( metaObj );
    return metaObj;
}

void* RunDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RunDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool RunDialog::qt_invoke( int _id, QUObject* _o )
{
    return AppDialog::qt_invoke(_id,_o);
}

bool RunDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RunDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool RunDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
