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
#include "DialogNumber.h"
#include "EditMonth.h"

GridNumber::GridNumber( QWidget * parent, EditMonth * editMonth )
	: GridWidget( parent ), m_editMonth( editMonth ), m_orderBy( 1 )
{
	horizontalHeader()->setClickable( true );
	horizontalHeader()->setSortIndicatorShown( true );
	connect( horizontalHeader(), SIGNAL( sectionClicked( int ) ), SLOT( columnClicked( int ) ) );
	refresh();
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
	m_menu->addAction( QIcon(), "Изменить запись", this, SLOT( update() ), Qt::CTRL + Qt::Key_E );
	//m_menu->addAction( QIcon(), "Удалить номер", this, SLOT( del() ), Qt::Key_Delete );

	return m_menu;
}

void
GridNumber::refresh( const QVariant & key )
{
	const QVariant prevKey = key.isNull() ? currentKeyValue() : key;

	m_model->setQuery( QString("SELECT DISTINCT "
			"CASE WHEN n.number IS NULL "
				"THEN m.number "
				"ELSE n.number "
			"END, "
			"CASE WHEN n.people_id IS NULL "
				"THEN n.pseudo "
				"ELSE common.fio( cp.fam, cp.nam, cp.pat ) "
			"END, "
			"n.post, "
			"eo.caption, "
			"t.caption, "
			"to_char( t.\"limit\", %1), "
			"to_char( m.bill, %1), "
			"to_char( m.bill - t.\"limit\", %1), "
			"c.rcap "
		"FROM "
			"\"mobi\".\"number\" n "
		"LEFT OUTER JOIN "
			"\"common\".\"people\" cp ON n.people_id = cp.id "
		"LEFT OUTER JOIN "
			"\"erp\".\"otdel\" eo ON n.otdel_id = eo.id "
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
			"WHERE month = %2 AND year = %3 ) m ON n.number = m.number "
		//"WHERE "
			//"n.people_id IS NOT NULL "
		//"OR  n.pseudo IS NOT NULL "
		//"OR ( m.month = %2 AND m.year = %3 ) "
		"ORDER BY "
			"%4 %5 ")
			.arg( "'FM9999999999999990D00L'" )
			.arg( m_editMonth->month().month() )
			.arg( m_editMonth->month().year() )
			.arg( qAbs( m_orderBy ) )
			.arg( m_orderBy < 0 ? "DESC" : "ASC" ) );

	setColumnCaption( 0, "Номер");
	setColumnCaption( 1, "Пользователь");
	setColumnCaption( 2, "Должность");
	setColumnCaption( 3, "Отдел");
	setColumnCaption( 4, "Тарифный план");
	setColumnCaption( 5, "Лимит");
	setColumnCaption( 6, m_editMonth->month().toString() );
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
		refresh( prevKeyValue() );
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

