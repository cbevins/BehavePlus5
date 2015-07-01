/****************************************************************************
** GraphLimitsDialog meta object code from reading C++ file 'graphlimitsdialog.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../graphlimitsdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GraphLimitsDialog::className() const
{
    return "GraphLimitsDialog";
}

QMetaObject *GraphLimitsDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GraphLimitsDialog( "GraphLimitsDialog", &GraphLimitsDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GraphLimitsDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GraphLimitsDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GraphLimitsDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GraphLimitsDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GraphLimitsDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"GraphLimitsDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GraphLimitsDialog.setMetaObject( metaObj );
    return metaObj;
}

void* GraphLimitsDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GraphLimitsDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool GraphLimitsDialog::qt_invoke( int _id, QUObject* _o )
{
    return AppDialog::qt_invoke(_id,_o);
}

bool GraphLimitsDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GraphLimitsDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool GraphLimitsDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
