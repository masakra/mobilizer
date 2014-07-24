/***************************************************************************
 *   Copyright (C) 2014 by Sergey N Chursanov                              *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   jabber: masakra@jabber.ru                                             *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include "DialogTarif.h"

#include <QtGui>
#include <NaraGui>
#include <NaraPg>
#include "SpinLimit.h"

DialogTarif::DialogTarif( QWidget * parent )
	: Dialog( parent )
{
	createWidgets();
	resize( 400, 50 );
}

void
DialogTarif::createWidgets()
{
	m_editCaption = new QLineEdit( this );

	m_spinLimit = new SpinLimit( this );

	QLabel * labelCaption = new QLabel("&Название", this ),
		   * labelLimit = new QLabel("&Лимит", this );

	labelCaption->setBuddy( m_editCaption );
	labelLimit->setBuddy( m_spinLimit );

	QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Save | QDialogButtonBox::Close,
			Qt::Horizontal, this );
	connect( buttonBox, SIGNAL( accepted() ), SLOT( save() ) );
	connect( buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

	QGridLayout * layout = new QGridLayout( this );
	layout->addWidget( labelCaption, 0, 0, Qt::AlignRight );
	layout->addWidget( m_editCaption, 0, 1 );
	layout->addWidget( labelLimit, 1, 0, Qt::AlignRight );
	layout->addWidget( m_spinLimit, 1, 1 );
	layout->addWidget( new Frame( QFrame::HLine ), 2, 0, 1, 2 );
	layout->addWidget( buttonBox, 3, 0, 1, 2 );
}

void
DialogTarif::save()
{
	PgQuery q;

	if ( key().isNull() ) {		// если key().isNull() следовательно добавляем запись, иначе изменяем запись
		q.prepare("INSERT INTO \"mobi\".\"tarif\" ("
				"caption, "
				"\"limit\" "
			") VALUES ("
				":caption, "
				":limit "
			") RETURNING "
				"id");
	} else {
		q.prepare("UPDATE \"mobi\".\"tarif\" SET "
				"caption = :caption, "
				"\"limit\" = :limit "
			"WHERE "
				"id = :key");
		q.bindValue(":key", key() );
	}

	q.bindValue(":caption", m_editCaption->text().trimmed() );
	q.bindValue(":limit", m_spinLimit->value() );

	if ( ! q.exec() )
		return;

	if ( q.first() )
		m_key = q.value( 0 );

	accept();
}

void
DialogTarif::fetchData()		// virtual
{
	PgQuery q;

	q.prepare("SELECT "
			"caption, "
			"\"limit\" "
		"FROM "
			"\"mobi\".\"tarif\" "
		"WHERE "
			"id = :key");
	q.bindValue(":key", key() );

	if ( ! q.exec() )
		return;

	q.first();
	m_editCaption->setText( q.value( 0 ).toString() );
	m_spinLimit->setValue( q.value( 1 ).toDouble() );
}

