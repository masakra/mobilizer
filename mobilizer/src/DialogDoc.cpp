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

#include "DialogDoc.h"

#include "EditMonth.h"

#include <QtGui>
#include <NaraGui>
#include "SpinLimit.h"

DialogDoc::DialogDoc( QWidget * parent, const QAction * action )
	: QDialog( parent )
{
	if ( action ) {
		setWindowIcon( action->icon() );
		setWindowTitle( action->text() );
	}
	createWidgets();
}

void
DialogDoc::createWidgets()
{
	m_editMonth = new EditMonth( this );

	m_spinThresh = new SpinLimit( this );
	m_spinThresh->setValue( 100. );

	QLabel * labelMonth = new QLabel("&Месяц", this ),
		   * labelThresh = new QLabel("&Более", this );

	labelMonth->setBuddy( m_editMonth );
	labelThresh->setBuddy( m_spinThresh );

	QGridLayout * layout = new QGridLayout( this );

	QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
			Qt::Horizontal, this );

	connect( buttonBox, SIGNAL( accepted() ), SLOT( accept() ) );
	connect( buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

	layout->addWidget( labelMonth, 0, 0, Qt::AlignRight );
	layout->addWidget( m_editMonth, 0, 1 );
	layout->addWidget( labelThresh, 1, 0, Qt::AlignRight );
	layout->addWidget( m_spinThresh, 1, 1 );
	layout->addWidget( new Frame( QFrame::HLine ), 2, 0, 1, 2 );
	layout->addWidget( buttonBox, 3, 0, 1, 2 );
}

void
DialogDoc::setMonth( const Month & month )
{
	m_editMonth->setMonth( month );
}

Month
DialogDoc::month() const
{
	return m_editMonth->month();
}

qreal
DialogDoc::threshold() const
{
	return m_spinThresh->value();
}

