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

#include "WidgetNumber.h"

#include "EditMonth.h"
#include "GridNumber.h"

#include <QtGui>

WidgetNumber::WidgetNumber( QWidget * parent )
	: QWidget( parent )
{
	setWindowIcon( QIcon(":/mobile_phone.png") );
	createWidgets();

	QSettings s;

	m_editMonth->setMonth( s.value("month", QDate::currentDate().month() ).toInt(),
			s.value("year", QDate::currentDate().year() ).toInt() );
}

WidgetNumber::~WidgetNumber()
{
	QSettings s;

	s.setValue("month", m_editMonth->month().month() );
	s.setValue("year", m_editMonth->month().year() );
}

void
WidgetNumber::createWidgets()
{
	m_editMonth = new EditMonth( this );

	m_gridNumber = new GridNumber( this, m_editMonth );

	connect( m_editMonth, SIGNAL( changed( int, int ) ), m_gridNumber, SLOT( refresh() ) );

	QLabel * labelMonth = new QLabel("&Месяц");

	labelMonth->setBuddy( m_editMonth );

	QHBoxLayout * layoutParams = new QHBoxLayout();
	layoutParams->addWidget( labelMonth );
	layoutParams->addWidget( m_editMonth );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addLayout( layoutParams );
	layout->addWidget( m_gridNumber );
}

void
WidgetNumber::setMonth( const Month & month )
{
	m_editMonth->setMonth( month );
	//m_gridNumber->refresh();
}

Month
WidgetNumber::month() const
{
	return m_editMonth->month();
}

