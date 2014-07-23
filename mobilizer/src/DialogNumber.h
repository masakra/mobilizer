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

/*! \class DialogNumber
 *
 * \brief 
 */

#ifndef DIALOGNUMBER_H
#define DIALOGNUMBER_H

#include "Dialog.h"

class QDateEdit;
class QDialogButtonBox;
class QGroupBox;
class QLineEdit;
class QListWidget;
class QRadioButton;
class QSpinBox;
class ComboBox;

class DialogNumber : public Dialog
{
	Q_OBJECT

	private:
		void createWidgets();

		QLineEdit * m_editNumber,
				  * m_editFam,
				  * m_editNam,
				  * m_editPat,
				  * m_editPost,
				  * m_editPseudo;

		QGroupBox * m_groupMan;

		QRadioButton * m_radioManNew,
					 * m_radioManExists,
					 * m_radioManPseudo;

		ComboBox * m_comboOtdel,
				 * m_comboTarif,
				 * m_comboCity,
				 * m_comboSex;

		QDateEdit * m_editBirth;

		QListWidget * m_listManExists;

		void fillComboOtdel();

		void fillComboTarif();

		void fillComboCity();

		QSpinBox * m_spinLimit;

		QDialogButtonBox * m_buttonBox;

		bool numberExists( const QVariant & number ) const;

		QVariant createNewMan() const;

	private Q_SLOTS:
		void save();
		void tarifChanged( int index );
		void editNumberChanged( const QString & text );
		void editFamChanged( const QString & text );

	protected:
		void fetchData();		// virtual

	public:
		DialogNumber( QWidget * parent = 0 );
};

#endif

