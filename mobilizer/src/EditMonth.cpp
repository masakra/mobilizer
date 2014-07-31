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

#include "EditMonth.h"

#include <QtGui>

EditMonth::EditMonth( QWidget * parent )
	: QWidget( parent )
{
	createWidgets();
}

void
EditMonth::createWidgets()
{
	m_spinMonth = new QSpinBox( this );
	m_spinMonth->setRange( 1, 12 );
	connect( m_spinMonth, SIGNAL( valueChanged( int ) ), SLOT( smthChanged() ) );

	m_spinYear = new QSpinBox( this );
	m_spinYear->setRange( 1970, 9999 );
	connect( m_spinYear, SIGNAL( valueChanged( int ) ), SLOT( smthChanged() ) );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setContentsMargins( 0, 0, 0, 0 );
	layout->setSpacing( 1 );
	layout->addWidget( m_spinMonth );
	layout->addWidget( m_spinYear );
}

Month
EditMonth::month() const
{
	return Month( m_spinMonth->value(), m_spinYear->value() );
}

void
EditMonth::setMonth( int month, int year )
{
	m_spinMonth->setValue( month );
	m_spinYear->setValue( year );
}

void
EditMonth::setMonth( const Month & month )
{
	setMonth( month.month(), month.year() );
}

void
EditMonth::smthChanged() const
{
	emit changed( m_spinMonth->value(), m_spinYear->value() );
}

