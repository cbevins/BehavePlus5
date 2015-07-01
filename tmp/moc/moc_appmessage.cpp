/****************************************************************************
** HelpDialog meta object code from reading C++ file 'appmessage.h'
**
** Created: Thu Apr 29 11:04:23 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../appmessage.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *HelpDialog::className() const
{
    return "HelpDialog";
}

QMetaObject *HelpDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HelpDialog( "HelpDialog", &HelpDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HelpDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HelpDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HelpDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HelpDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HelpDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"HelpDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HelpDialog.setMetaObject( metaObj );
    return metaObj;
}

void* HelpDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HelpDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool HelpDialog::qt_invoke( int _id, QUObject* _o )
{
    return AppDialog::qt_invoke(_id,_o);
}

bool HelpDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool HelpDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool HelpDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *MessageDialog::className() const
{
    return "MessageDialog";
}

QMetaObject *MessageDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MessageDialog( "MessageDialog", &MessageDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MessageDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MessageDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MessageDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MessageDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MessageDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = AppDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MessageDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MessageDialog.setMetaObject( metaObj );
    return metaObj;
}

void* MessageDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MessageDialog" ) )
	return this;
    return AppDialog::qt_cast( clname );
}

bool MessageDialog::qt_invoke( int _id, QUObject* _o )
{
    return AppDialog::qt_invoke(_id,_o);
}

bool MessageDialog::qt_emit( int _id, QUObject* _o )
{
    return AppDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MessageDialog::qt_property( int id, int f, QVariant* v)
{
    return AppDialog::qt_property( id, f, v);
}

bool MessageDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
