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

#include "ModelNumber.h"

//#include <QtGui>
//
#include <QDebug>

ModelNumber::ModelNumber( QObject * parent )
	: QSqlQueryModel( parent )
{
}

QVariant
ModelNumber::data( const QModelIndex & index, int role ) const
{
	const int col = index.column();

	if ( col == 0 && role == Qt::TextAlignmentRole )
		return Qt::AlignCenter;

	if ( col == 5 || col == 6 || col == 7 ) {
		if ( role == Qt::TextAlignmentRole )
			return Qt::AlignRight + Qt::AlignVCenter;

		if ( role == Qt::DisplayRole && index.isValid() ) {
			const double d = QSqlQueryModel::data( index, role ).toDouble();
			return QString("%L1").arg( d, 0, 'f', 2 );
		}
	}

	return QSqlQueryModel::data( index, role );
}

