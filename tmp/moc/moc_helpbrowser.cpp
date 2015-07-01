/****************************************************************************
** HelpBrowser meta object code from reading C++ file 'helpbrowser.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../helpbrowser.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *HelpBrowser::className() const
{
    return "HelpBrowser";
}

QMetaObject *HelpBrowser::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HelpBrowser( "HelpBrowser", &HelpBrowser::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HelpBrowser::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HelpBrowser", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HelpBrowser::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HelpBrowser", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HelpBrowser::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUMethod slot_0 = {"showIndex", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "showIndex()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"HelpBrowser", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HelpBrowser.setMetaObject( metaObj );
    return metaObj;
}

void* HelpBrowser::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HelpBrowser" ) )
	return this;
    return QVBox::qt_cast( clname );
}

bool HelpBrowser::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: showIndex(); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool HelpBrowser::qt_emit( int _id, QUObject* _o )
{
    return QVBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool HelpBrowser::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool HelpBrowser::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
