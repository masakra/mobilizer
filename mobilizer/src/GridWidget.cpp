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

#include "GridWidget.h"

#include <QtGui>
#include <QtSql>

GridWidget::GridWidget( QWidget * parent, int keyColumn )
	: Grid( parent ), m_model( new QSqlQueryModel( this ) ), m_keyColumn( keyColumn ), m_menu( 0 )
{
	setAttribute( Qt::WA_DeleteOnClose );
	verticalHeader()->hide();
	setModel( m_model );
}

void
GridWidget::setColumnCaption( int column, const QString & caption )
{
	m_model->setHeaderData( column, Qt::Horizontal, caption );
}

QVariant
GridWidget::currentKeyValue() const
{
	const int currentRow = selectionModel()->currentIndex().row();
	return m_model->index( currentRow, m_keyColumn ).data();
}

QVariant
GridWidget::prevKeyValue() const
{
	const int prevRow = qMax( selectionModel()->currentIndex().row() - 1, 0 );
	return m_model->index( prevRow, m_keyColumn ).data();
}

void
GridWidget::selectRow( const QVariant & keyValue )
{
	if ( keyValue.isNull() )
		return;

	for ( int i = 0; i < m_model->rowCount(); ++i )
		if ( m_model->index( i, m_keyColumn ).data() == keyValue ) {
			Grid::selectRow( i );
			break;
		}
}

void
GridWidget::contextMenuEvent( QContextMenuEvent * e )		// virtual
{
	menu()->exec( e->globalPos() );
}

