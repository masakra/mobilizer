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

/*! \class FormMain
 *
 * \brief Главное окно
 */

#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QMainWindow>

class WidgetNumber;

class FormMain : public QMainWindow
{
	Q_OBJECT

	private:
		WidgetNumber * m_widgetNumber;

		QToolBar * m_actionsToolBar;

		QDockWidget * m_dockMessage;

		void createWidgets();
		void createActions();
		void createToolBar();
		void createMainMenu();
		void createMessageDock();

		QAction * m_actionDataTarif,
				* m_actionNewNumber,
				* m_actionXmlToDb,
				* m_actionDocOrder,
				* m_actionDocNote;

		QMenu * m_menuFile,
			  * m_menuActions;

		QAction * m_gridWidgetMenuAction,
				* m_actionPrint,
				* m_actionToPdf;

		static const char * m_settings_geometry_ident,
						  * m_settings_state_ident;

	private Q_SLOTS:
		void newNumber();
		void gridTarif();
		void loadXmlToDb();
		void docOrder();
		void docNote();
		void aboutToShowMenuFile();
		void aboutToHideMenuFile();
		void aboutToShowMenuActions();
		void aboutToHideMenuActions();
		void about();

	public:
		FormMain();
		~FormMain();
};

#endif

