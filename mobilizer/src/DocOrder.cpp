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

#include "DocOrder.h"

#include "Month.h"

DocOrder::DocOrder( QWidget * parent, const Month & month )
	: Doc( parent )
{
	build( month );
}

void
DocOrder::build( const Month & month )
{
	QString html = QString(
	"<h3 align=center>ПРИКАЗ</h3>"
	"<table width=100%><tr><td align=center>"
	"<u>%1 г.</u>"			// за месяц
	"</td><td align=center>"
	"№ <u>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</u>"
	"</td></tr></table>"
	"<center>г. Архангельск</center>"
	"<p align=justify style='text-indent: 50px;'>Об удержании сумм, превышающих установленный лимит на мобильную связь.</p>"
	"<p align=justify style='text-indent: 50px;'>В соответствии с приказом от 18.08.2010 г. № 501, и на основе заключённых соглашений на предоставление "
	"и использование служебной сотовой связи<br><br>"
	"ПРИКАЗЫВАЮ:<br>"
	"Отделу бухгалтерского учёта (Горшенина Р.И.) произвести удержание из заработной платы сумм, превышающих "
	"установленный лимит на мобильную связь за %1 "		// за месяц
	"года, согласно прилагаемому списку:</p>"
	"%2"						// detailTable
	"<p align=justify style='text-indent: 50px;'>"
	"Заместитель ГД по качеству __________________________________ И.Б.Колпаков</p>"
	"<h4>СОГЛАСОВАНО</h4>"
	"<table width=100%>"
	"<tr><td colspan=3>&nbsp;</td></tr>"
	"<tr><td>Директор по ИТ</td><td align=right>__________________</td><td>И.Г.Наволоцкий</td></tr>"
	"<tr><td></td><td align=right valign=top><font size=1>(подпись)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</font></td><td></td></tr>"
	"<tr><td colspan=3>&nbsp;</td></tr>"
	"<tr><td>Начальние ЮС</td><td align=right>__________________</td><td>С.И.Туров</td></tr>"
	"<tr><td></td><td align=right valign=top><font size=1>(подпись)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</font></td><td></td></tr>"
	"</table>"
	"Рассылка:"
	"<ol>"
	"<li>Дело</li>"
	"<li>ОБУ (21-87-92)</li>"
	"<li>ССТК (21-87-88)</li>"
	"<li>БУМЦ (21-88-37)</li>"
	"</ol>"
	"Исп:<br><br>"
	"________________ А.П.Корсаков<br>"
	"<font size=1>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(подпись)</font><br>"
	"Тел. 21-88-88<br>"
	"<table>"
	"<tr><td width='20px'>В дело №</td><td>____________</td><td></td></tr>"
	"<tr><td></td><td align=center><font size=1>(номер документа)</font></td><td></td></tr>"
	"<tr><td colspan=2>________________________</td><td>________________________</td></tr>"
	"<tr><td colspan=2 align=center><font size=1>(подпись)</font></td><td align=center><font size=1>(инициалы и фамилия)</font></td></tr>"
	"<tr><td colspan=2>________________________</td><td></td></tr>"
	"<tr><td colspan=2 align=center><font size=1>(дата)</font></td><td></td></tr>"
	"</table>"
	)
		.arg( month.toString() )	// 1 за месяц
		.arg( detailTable( month ) )
	;

	setHtml( html );
}

