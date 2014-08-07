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

/*! \class DialogLoadXmlToDb
 *
 * \brief 
 */

#ifndef DIALOGLOADXMLTODB_H
#define DIALOGLOADXMLTODB_H

#include <QDialog>
#include "Month.h"

class QListWidget;
class EditMonth;

class DialogLoadXmlToDb : public QDialog
{
	Q_OBJECT

	private:
		void createWidgets();

		//QLineEdit * m_editPath;
		QListWidget * m_listPath;

		EditMonth * m_editMonth;

		bool saveMontlyToDb( const QStringList & numbers, const QStringList & list ) const;

		bool saveDetailToDb( const QStringList & ds_numbers, const QStringList & detail ) const;

	private Q_SLOTS:
		void openXmlFile();
		void save();

	public:
		DialogLoadXmlToDb( QWidget * parent );

		Month month() const;
};

#endif

