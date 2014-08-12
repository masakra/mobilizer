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

#include "GridNumber.h"

#include <QtGui>
#include <QtSql>
#include <NaraPg>
#include "_.h"
#include "DialogNumber.h"
#include "DocDetail.h"
#include "EditMonth.h"
#include "ModelNumber.h"
#include "WidgetNumber.h"

GridNumber::GridNumber( QWidget * parent, WidgetNumber * widgetNumber )
	: GridWidget( parent ), m_widgetNumber( widgetNumber ), m_orderBy( 1 )
{
	delete m_model;

	setModel( m_model = new ModelNumber( this ) );

	horizontalHeader()->setClickable( true );
	horizontalHeader()->setSortIndicatorShown( true );
	connect( horizontalHeader(), SIGNAL( sectionClicked( int ) ), SLOT( columnClicked( int ) ) );
	refresh();

	m_timerSearch.setInterval( 3000 ); // 3 seconds
	connect( &m_timerSearch, SIGNAL( timeout() ), widgetNumber, SLOT( searchStop() ) );
}

QMenu *
GridNumber::menu() // virtual
{
	if ( m_menu )
		return m_menu;

	m_menu = new QMenu("Номера", this );
	m_menu->setIcon( windowIcon() );

	m_menu->addAction( QIcon::fromTheme("view-refresh", QIcon() ), "&Обновить", this, SLOT( refresh() ),
			Qt::CTRL + Qt::Key_R );
	m_menu->addAction( QIcon(), "Детализация", this, SLOT( detail() ), Qt::CTRL + Qt::Key_D );
	m_menu->addAction( QIcon(), "Изменить запись", this, SLOT( update() ), Qt::CTRL + Qt::Key_E );
	m_menu->addAction( QIcon::fromTheme("edit-delete", QIcon() ), "Удалить запись", this,
			SLOT( del() ), Qt::Key_Delete );


	return m_menu;
}

void
GridNumber::refresh( const QVariant & key )
{
	const QVariant prevKey = key.isNull() ? currentKeyValue() : key;

	m_model->setQuery( QString("SELECT DISTINCT "
			"CASE WHEN n.number IS NULL "		// 0
				"THEN m.number "
				"ELSE n.number "
			"END, "
			"CASE WHEN n.people_id IS NULL "	// 1
				"THEN n.pseudo "
				"ELSE common.fio( cp.fam, cp.nam, cp.pat ) "
			"END, "
			"CASE WHEN b.chief IS NULL "		// 2 erp.post.caption - должность или 'Начальник'
				"THEN p.caption "
				"ELSE 'Начальник' "
			"END, "
			"CASE WHEN b.chief IS NULL "		// 3 erp.division.caption - отдел
				"THEN d.caption "
				"ELSE b.caption "
			"END, "
			"t.caption, "						// 4 mobi.tarif.caption - тариф
			"n.limit, "							// 5
			"m.bill, "							// 6
			"m.bill - n.limit, "				// 7
			"c.rcap "							// 8
		"FROM "
			"\"mobi\".\"number\" n "
		"LEFT OUTER JOIN "
			"\"common\".\"people\" cp ON n.people_id = cp.id "
		"LEFT OUTER JOIN "
			"\"erp\".\"employee\" e ON n.people_id = e.people_id "
		"LEFT OUTER JOIN "
			"\"erp\".\"division\" d ON e.division_abbr = d.abbr "
		"LEFT OUTER JOIN "
			"\"erp\".\"division\" b ON n.people_id = b.chief "	// boss
		"LEFT OUTER JOIN "
			"\"erp\".\"post\" p ON e.post_id = p.id "
		"LEFT OUTER JOIN "
			"\"mobi\".\"tarif\" t ON n.tarif_id = t.id "
		"LEFT OUTER JOIN "
			"\"common\".\"city\" c ON n.city_iata = c.iata "
		"FULL OUTER JOIN "
			"( SELECT "
				"number AS number, "
				"bill AS bill "
			"FROM "
				"\"mobi\".\"montly\" "
			"WHERE "
				"month = %1 AND year = %2 ) m ON n.number = m.number "
		"ORDER BY "
			"%3 %4 ")
			.arg( m_widgetNumber->month().month() )
			.arg( m_widgetNumber->month().year() )
			.arg( qAbs( m_orderBy ) )
			.arg( m_orderBy < 0 ? "DESC" : "ASC" ) );

	emit rowCountChanged( m_model->rowCount() );

	setColumnCaption( 0, "Номер");
	setColumnCaption( 1, "Пользователь");
	setColumnCaption( 2, "Должность");
	setColumnCaption( 3, "Отдел");
	setColumnCaption( 4, "Тарифный план");
	setColumnCaption( 5, "Лимит");
	setColumnCaption( 6, m_widgetNumber->month().toString() );
	setColumnCaption( 7, "Перерасход");
	setColumnCaption( 8, "Город");

	selectRow( prevKey );
}

/*
void
GridNumber::insert()
{
}
*/

void
GridNumber::update()
{
	DialogNumber d( this );

	if ( d.exec( currentKeyValue() ) )
		refresh( d.key() );
}

void
GridNumber::del()
{
	PgQuery q;

	q.prepare("DELETE FROM "
				"\"mobi\".\"number\" "
			"WHERE "
				"number = :number");
	q.bindValue(":number", currentKeyValue() );

	if ( q.exec() )
		refresh( currentKeyValue() );
}

void
GridNumber::columnClicked( int logicalIndex )
{
	if ( qAbs( m_orderBy ) == logicalIndex + 1 )
		m_orderBy = -m_orderBy;
	else
		m_orderBy = logicalIndex + 1;

	refresh();
}

void
GridNumber::keyPressEvent( QKeyEvent * event )
{
	if ( ( ( event->modifiers() & Qt::ControlModifier ) | ( event->modifiers() & Qt::AltModifier ) ) != 0 )
		return GridWidget::keyPressEvent( event );

	const QString text = event->text();

	if ( event->key() == Qt::Key_Backspace ) {
		keyboardSearch( m_widgetNumber->search() );
		m_timerSearch.start();

	} else if ( ! text.isEmpty() ) {
		keyboardSearch( m_widgetNumber->search( text ) );
		m_timerSearch.start();
	}

	if ( event->key() == Qt::Key_Escape ) {
		m_timerSearch.stop();
		m_widgetNumber->searchStop();
		keyboardSearch("");
	}

	GridWidget::keyPressEvent( event );
}


void
GridNumber::detail()
{
	const QString number = currentKeyValue().toString(),
				  docDetailCaption = QString("&Детализация %1 за %2")
					  .arg( number )
					  .arg( m_widgetNumber->month().toString() );

	if ( ! _tab->haveLabel( docDetailCaption ) )
		_tab->addTab( new DocDetail( _tab, number, m_widgetNumber->month() ), docDetailCaption );
}

