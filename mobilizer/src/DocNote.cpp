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

#include "DocNote.h"

#include "Month.h"

DocNote::DocNote( QWidget * parent, const Month & month )
	: Doc( parent )
{
	build( month );
}

void
DocNote::build( const Month & month )
{
	setHtml( QString(
	"<hr>"
	"<table width=100% bgcolor=white cellspacing=0 >"
	"<tr><td colspan=3 rowspan=2 valign=middle>Исх. от ____ 2014 года.</td>"
		"<td colspan=4 align=center><u><b>СЛУЖЕБНАЯ ЗАПИСКА</b></u></td></tr>"
	"<tr><td colspan=4 align=center><font size=1>(наименование вида документа)</font></td></tr>"
	"<tr><td>Кому:</td><td colspan=6><u><b>Руководителю лётного комплекса Симонову В.А.</b></u></td></tr>"
	"<tr><td><font size=1>&nbsp;</font></td><td colspan=6 align=center><font size=1>(наименование должности, инициалы и фамилия адресата)</font></td></tr>"
	"<tr><td>От:</td><td colspan=6><u><b>Директора по ИТС Наволоцкого И.Г.</b></u></td></tr>"
	"<tr><td><font size=1>&nbsp;</font></td><td colspan=6 align=center><font size=1>(наименование должности, инициалы и фамилия отправителя)</font></td></tr>"
	"<tr><td>Тема:</td><td colspan=6><u>Превышение лимита на сотовую связь за %1 года</u></td></tr>"
	"<tr><td><font size=1>&nbsp;</font></td><td colspan=6 align=center><font size=1>(о чём документ)</font></td></tr>"
	"</table>"
	"<hr>"
	"<p align=justify style='text-indent: 50px;'>Согласно приказа № 501 от 31.08.2010 г. об использовании служебной мобильной связи, прошу согласовать "
	"удержание суммы из заработной платы сотрудников, которые превысили установленный лимит на мобильную связь "
	"за %1 года, по следующему списку:"
	"%2"
	"<p align=right>"
	"<table>"
	"<tr><td><u>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</u></td><td><u>Наволоцкий И.Г.</u></td></tr>"
	"<tr><td align=center><font size=1>(подпись)</font></td><td align=center><font size=1>(инициалы и фамилия)</font></td></tr>"
	"</table>")
			.arg( month.toString() )
			.arg( detailTable( month ) )
	);
}

