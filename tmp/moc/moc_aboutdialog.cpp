/****************************************************************************
** AboutDialog meta object code from reading C++ file 'aboutdialog.h'
**
** Created: Thu Apr 29 11:04:24 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../aboutdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AboutDialog::className() const
{
    return "AboutDialog";
}

QMetaObject *AboutDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AboutDialog( "AboutDialog", &AboutDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AboutDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AboutDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AboutDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AboutDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AboutDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppTabDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"AboutDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AboutDialog.setMetaObject( metaObj );
    return metaObj;
}

void* AboutDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AboutDialog" ) )
	return this;
    return AppTabDialog::qt_cast( clname );
}

bool AboutDialog::qt_invoke( int _id, QUObject* _o )
{
    return AppTabDialog::qt_invoke(_id,_o);
}

bool AboutDialog::qt_emit( int _id, QUObject* _o )
{
    return AppTabDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool AboutDialog::qt_property( int id, int f, QVariant* v)
{
    return AppTabDialog::qt_property( id, f, v);
}

bool AboutDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
