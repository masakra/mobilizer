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

#include "GridTarif.h"

#include <QtGui>
#include <QtSql>
#include <NaraPg>
#include "DialogTarif.h"

GridTarif::GridTarif( QWidget * parent )
	: GridWidget( parent )
{
	setWindowIcon( QIcon(":/ruble.png") );
	refresh();
}

void
GridTarif::refresh( const QVariant & key )
{
	const QVariant prevKey = key.isNull() ? currentKeyValue() : key;

	m_model->setQuery("SELECT "
			"id, "
			"caption, "
			"\"limit\" "
		"FROM "
			"\"mobi\".\"tarif\" "
		"ORDER BY "
			"caption");

	hideColumn( m_keyColumn );
	setColumnCaption( 1, "Название\nтарифа");
	setColumnCaption( 2, "Лимит");
	setColumnWidth( 1, 500 );

	selectRow( prevKey );
}

QMenu *
GridTarif::menu() // virtual
{
	if ( m_menu )
		return m_menu;

	m_menu = new QMenu("Тарифные планы", this );
	m_menu->setIcon( windowIcon() );

	m_menu->addAction( QIcon::fromTheme("view-refresh", QIcon() ), "&Обновить", this, SLOT( refresh() ),
			Qt::CTRL + Qt::Key_R );
	m_menu->addAction( QIcon::fromTheme("document-new", QIcon() ), "&Добавить запись", this, SLOT( insert() ),
			Qt::Key_Insert );
	m_menu->addAction( QIcon(), "&Изменить запись", this, SLOT( update() ), Qt::CTRL + Qt::Key_E );
	m_menu->addAction( QIcon::fromTheme("edit-delete", QIcon() ), "&Удалить запись", this, SLOT( del() ),
			Qt::Key_Delete );

	return m_menu;
}

void
GridTarif::insert()
{
	DialogTarif d( this );

	if ( d.exec() )
		refresh( d.key() );
}

void
GridTarif::update()
{
	DialogTarif d( this );
	if ( d.exec( currentKeyValue() ) )
		refresh( d.key() );
}

void
GridTarif::del()
{
	PgQuery q;

	q.prepare("DELETE FROM \"mobi\".\"tarif\" WHERE id = :key");
	q.bindValue(":key", currentKeyValue() );
	if ( q.exec() )
		refresh( prevKeyValue() );
}

