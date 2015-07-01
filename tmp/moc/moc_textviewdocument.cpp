/****************************************************************************
** TextViewDocument meta object code from reading C++ file 'textviewdocument.h'
**
** Created: Thu Apr 29 11:04:22 2010
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../../textviewdocument.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TextViewDocument::className() const
{
    return "TextViewDocument";
}

QMetaObject *TextViewDocument::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TextViewDocument( "TextViewDocument", &TextViewDocument::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TextViewDocument::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TextViewDocument", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TextViewDocument::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TextViewDocument", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TextViewDocument::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = Document::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuActivated", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "points", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"rescale", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuActivated(int)", &slot_0, QMetaData::Protected },
	{ "rescale(int)", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"TextViewDocument", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TextViewDocument.setMetaObject( metaObj );
    return metaObj;
}

void* TextViewDocument::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TextViewDocument" ) )
	return this;
    return Document::qt_cast( clname );
}

bool TextViewDocument::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuActivated((int)static_QUType_int.get(_o+1)); break;
    case 1: rescale((int)static_QUType_int.get(_o+1)); break;
    default:
	return Document::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TextViewDocument::qt_emit( int _id, QUObject* _o )
{
    return Document::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TextViewDocument::qt_property( int id, int f, QVariant* v)
{
    return Document::qt_property( id, f, v);
}

bool TextViewDocument::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
