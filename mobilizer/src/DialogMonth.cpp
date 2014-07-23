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

#include "DialogMonth.h"

#include "EditMonth.h"

#include <QtGui>
#include <NaraGui>

DialogMonth::DialogMonth( QWidget * parent, const QAction * action )
	: QDialog( parent )
{
	if ( action ) {
		setWindowIcon( action->icon() );
		setWindowTitle( action->text() );
	}
	createWidgets();
}

void
DialogMonth::createWidgets()
{
	m_editMonth = new EditMonth( this );

	QLabel * labelMonth = new QLabel("&Месяц", this );

	labelMonth->setBuddy( m_editMonth );

	QGridLayout * layout = new QGridLayout( this );

	QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
			Qt::Horizontal, this );

	connect( buttonBox, SIGNAL( accepted() ), SLOT( accept() ) );
	connect( buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

	layout->addWidget( labelMonth, 0, 0, Qt::AlignRight );
	layout->addWidget( m_editMonth, 0, 1 );
	layout->addWidget( new Frame( QFrame::HLine ), 1, 0, 1, 2 );
	layout->addWidget( buttonBox, 2, 0, 1, 2 );
}

void
DialogMonth::setMonth( const Month & month )
{
	m_editMonth->setMonth( month );
}

Month
DialogMonth::month() const
{
	return m_editMonth->month();
}

