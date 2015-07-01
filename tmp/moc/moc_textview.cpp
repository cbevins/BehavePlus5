/****************************************************************************
** TextBrowser meta object code from reading C++ file 'textview.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../textview.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TextBrowser::className() const
{
    return "TextBrowser";
}

QMetaObject *TextBrowser::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TextBrowser( "TextBrowser", &TextBrowser::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TextBrowser::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TextBrowser", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TextBrowser::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TextBrowser", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TextBrowser::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTextBrowser::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuActivated", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuActivated(int)", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TextBrowser", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TextBrowser.setMetaObject( metaObj );
    return metaObj;
}

void* TextBrowser::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TextBrowser" ) )
	return this;
    return QTextBrowser::qt_cast( clname );
}

bool TextBrowser::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    default:
	return QTextBrowser::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TextBrowser::qt_emit( int _id, QUObject* _o )
{
    return QTextBrowser::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TextBrowser::qt_property( int id, int f, QVariant* v)
{
    return QTextBrowser::qt_property( id, f, v);
}

bool TextBrowser::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *TextView::className() const
{
    return "TextView";
}

QMetaObject *TextView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TextView( "TextView", &TextView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TextView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TextView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TextView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TextView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TextView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTextEdit::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuActivated", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuActivated(int)", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TextView", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TextView.setMetaObject( metaObj );
    return metaObj;
}

void* TextView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TextView" ) )
	return this;
    return QTextEdit::qt_cast( clname );
}

bool TextView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    default:
	return QTextEdit::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TextView::qt_emit( int _id, QUObject* _o )
{
    return QTextEdit::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TextView::qt_property( int id, int f, QVariant* v)
{
    return QTextEdit::qt_property( id, f, v);
}

bool TextView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
