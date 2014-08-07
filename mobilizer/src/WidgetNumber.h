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

/*! \class WidgetNumber
 *
 * \brief 
 */

#ifndef WIDGETNUMBER_H
#define WIDGETNUMBER_H

#include <QWidget>
#include "Month.h"

class QLabel;
class EditMonth;
class GridNumber;

class WidgetNumber : public QWidget
{
	Q_OBJECT

	private:
		void createWidgets();

		EditMonth * m_editMonth;

		GridNumber * m_gridNumber;

		QLabel * m_labelSearch,
			   * m_labelSearchText,
			   * m_labelCount;

	private Q_SLOTS:
		void setRowCount( int count );

	public:
		WidgetNumber( QWidget * parent );
		~WidgetNumber();

		void setMonth( const Month & month );

		Month month() const;

		QAction * actionPrint() const;

		QAction * actionSaveToPdf() const;

		QString search( const QString & text = QString() );

	public Q_SLOTS:
		void searchStop();
};

#endif

