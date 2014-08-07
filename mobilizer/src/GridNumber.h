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

/*! \class GridNumber
 *
 * \brief 
 */

#ifndef GRIDNUMBER_H
#define GRIDNUMBER_H

#include "GridWidget.h"
#include <QTimer>

class WidgetNumber;

class GridNumber : public GridWidget
{
	Q_OBJECT

	private:
		WidgetNumber * m_widgetNumber;

		int m_orderBy;

		QTimer m_timerSearch;
		QLabel * m_labelSearch,
			   * m_labelSearchText;

	private Q_SLOTS:
		void update();
		void del();
		void columnClicked( int logicalIndex );
		void detail();

	protected:
		void keyPressEvent( QKeyEvent * event );

	public:
		GridNumber( QWidget * parent, WidgetNumber * widgetNumber );

		QMenu * menu();	// virtual

	public Q_SLOTS:
		void refresh( const QVariant & key = QVariant() );

	Q_SIGNALS:
		void rowCountChanged( int count ) const;

};

#endif

