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

#include "Doc.h"

#include <NaraPg>
#include "Month.h"

Doc::Doc( QWidget * parent )
	: TextEdit( parent )
{
#ifdef Q_OS_WIN32
	document()->setDefaultFont( QFont("Times New Roman", 12 ) );
#else
	document()->setDefaultFont( QFont("Liberation Serif", 12 ) );
#endif

}

void
Doc::setHtml( const QString & html )
{
	const QString container = QString(
	"<CENTER>"
	"<TABLE BORDER=0 BGCOLOR=white CELLSPACING=0 WIDTH=700>"
	"<TR><TD>"
		"<table width=100%><tr><td>"
		"<img src=':/nordavia_for_html.png' />"
		"</td>"
		"<td align=right valign=top>"
		"ЗАО &laquo;Нордавиа-региональные авиалинии&raquo;"
		"</td></tr>"
		"</table>"
		"%1"			// html this
	"</TD></TR>"
	"</TABLE>"
	"</CENTER>").arg( html );

	TextEdit::setHtml( container );
}

QString
Doc::detailTable( const Month & month, qreal thresh ) const
{
	QString html = QString(
	"<table width=100% bgcolor=gray cellspacing=1 cellpadding=5>"
	"<tr><th bgcolor=gainsboro>"
	"Должность</th><th bgcolor=gainsboro>"
	"Ф.И.О.</th><th bgcolor=gainsboro>"
	"Сумма удержаний, руб.</th></tr>"
	);

	PgQuery q;

	q.prepare("SELECT "
			"d.caption, "
			"common.fio( cp.fam, cp.nam, cp.pat ), "
			"m.bill - n.\"limit\" "
		"FROM "
			"\"mobi\".\"number\" n "
		"LEFT OUTER JOIN "
			"\"common\".\"people\" cp ON n.people_id = cp.id "
		"LEFT OUTER JOIN "
			"\"erp\".\"employee\" e ON n.people_id = e.people_id "
		"LEFT OUTER JOIN "
			"\"erp\".\"division\" d ON e.division_abbr = d.abbr "

		"LEFT OUTER JOIN "
			"\"mobi\".\"tarif\" t ON n.tarif_id = t.id "
		"LEFT OUTER JOIN "
			"\"mobi\".\"montly\" m ON n.number = m.number "
		"WHERE "
			"m.month = :month "
		"AND m.year = :year "
		"AND m.bill - n.\"limit\" > :thresh ");
	q.bindValue(":month", month.month() );
	q.bindValue(":year", month.year() );
	q.bindValue(":thresh", thresh );

	if ( ! q.exec() )
		return QString();

	while ( q.next() ) {
		html += QString(
				"<tr><td bgcolor=white>"
				"%1"		// должность
				"</td><td bgcolor=white>"
				"%2"		// ФИО
				"</td><td bgcolor=white align=right>"
				"%3"
				"</td></tr>")
			.arg( q.value( 0 ).toString() )
			.arg( q.value( 1 ).toString() )
			.arg( QString("%L1").arg( q.value( 2 ).toDouble(), 0, 'f', 2 ) );
	}

	return html.append("</table>");
}

