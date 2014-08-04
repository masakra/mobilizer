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

#include "DocDetail.h"

#include <QtGui>
#include <NaraPg>
#include "Month.h"

DocDetail::DocDetail( QWidget * parent, const QString & number, const Month & month )
	: Doc( parent )
{
	build( number, month );
}

void
DocDetail::build( const QString & number, const Month & month )
{
	setHtml( QString(
	"<h2 align=right><u>%1</u></h2>"
	"<p>За %2 г.</p>"
	"%3"
	)
			.arg( number )
			.arg( month.toString() )
			.arg( table( number, month ) )
		   );
}

QString
DocDetail::table( const QString & number, const Month & month )
{
	QString html =
	"<table width=100% bgcolor=slategray cellspacing=1 cellpadding=3>"
		"<tr>"
			"<th bgcolor=gainsboro width=15%>Дата</th>"
			"<th bgcolor=gainsboro>Время</th>"
			"<th bgcolor=gainsboro>Вр.<br>зона</th>"
			"<th bgcolor=gainsboro>Номер</th>"
			"<th bgcolor=gainsboro>Услуга</th>"
			"<th bgcolor=gainsboro>Длительность<br>или объём</th>"
			"<th bgcolor=gainsboro width=10%>руб.</th>"
		"</tr>";

	PgQuery q;

	q.prepare("SELECT "
		"start, "
		"to_char( start, 'TZ' ), "
		"pair, "
		"type, "
		"durfact, "
		"duration, "
		"bill "
	"FROM "
		"\"mobi\".\"detail\" "
	"WHERE "
		"number = :number "
	"AND EXTRACT( year FROM start ) = :year "
	"AND EXTRACT( month FROM start ) = :month "
	"ORDER BY "
		"start ");
	q.bindValue(":number", number );
	q.bindValue(":year", month.year() );
	q.bindValue(":month", month.month() );

	if ( q.exec() ) {
		bool odd = false;
		qreal sum = 0;
		while ( q.next() ) {
			html += QString(
			"<tr>"
				"<td bgcolor=%1>%2</td>"
				"<td bgcolor=%1>%3</td>"
				"<td bgcolor=%1 align=center>%4</td>"
				"<td bgcolor=%1>%5</td>"
				"<td bgcolor=%1>%6</td>"
				"<td bgcolor=%1 align=center>%7/%8</td>"
				"<td bgcolor=%1 align=right>%9</td>"
			"</tr>")
				.arg( ( odd = ! odd ) ? "white" : "floralwhite" )
				.arg( q.value( 0 ).toDate().toString("dd.MM.yyyy") )
				.arg( q.value( 0 ).toTime().toString("hh:mm:ss") )
				.arg( q.value( 1 ).toString() )
				.arg( q.value( 2 ).toString() )
				.arg( q.value( 3 ).toString() )
				.arg( q.value( 4 ).toString() )
				.arg( q.value( 5 ).toString() )
				.arg( q.value( 6 ).toDouble(), 0, 'f', 2 );

			sum += q.value( 6 ).toDouble();
		}

		html += QString(
		"<tr>"
			"<td colspan=5 bgcolor=white>Строк: %1</td>"
			"<td colspan=1 align=right bgcolor=white><b>Итого:</b></td>"
			"<td align=right bgcolor=white><b>%2</b></td>"
		"</tr>")
			.arg( q.size() )
			.arg( sum, 0, 'f', 2 );
	}

	return html.append("</table>");
}

