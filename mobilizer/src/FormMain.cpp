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

#include "FormMain.h"

#include <QtGui>
#include <NaraGui>
#include <NaraPg>
#include "_.h"
#include "DialogLoadXmlToDb.h"
#include "DialogDoc.h"
#include "DialogNumber.h"
#include "DocNote.h"
#include "DocOrder.h"
#include "EditMonth.h"
#include "GridNumber.h"
#include "GridTarif.h"
#include "WidgetNumber.h"

const char * FormMain::m_settings_geometry_ident = "formmain_geometry",
		   * FormMain::m_settings_state_ident = "formmain_state";

FormMain::FormMain()
	: QMainWindow(), m_gridWidgetMenuAction( 0 ), m_actionPrint( 0 ), m_actionToPdf( 0 )
{
	setWindowIcon( QIcon(":/main_icon.png") );
	createActions();
	createToolBar();
	createMessageDock();
	createMainMenu();
	createWidgets();

	QSettings s;

	restoreGeometry( s.value( m_settings_geometry_ident ).toByteArray() );
	restoreState( s.value( m_settings_state_ident ).toByteArray() );
}

FormMain::~FormMain()
{
	QSettings s;

	s.setValue( m_settings_geometry_ident, saveGeometry() );
	s.setValue( m_settings_state_ident, saveState() );

}

void
FormMain::createWidgets()
{
	_tab = new TabWidget( this );

	m_widgetNumber = new WidgetNumber( this );
	_tab->addTab( m_widgetNumber, "&Номера", QIcon(), false );

	setCentralWidget( _tab );
}

void
FormMain::createActions()
{
	m_actionNewNumber = new QAction( QIcon(":/new_number.png"), "Новый номер", this );
	m_actionNewNumber->setShortcut( Qt::CTRL + Qt::Key_N );
	connect( m_actionNewNumber, SIGNAL( triggered() ), SLOT( newNumber() ) );

	m_actionXmlToDb = new QAction( QIcon(":/xml_to_db.png"), "Загрузить данные из XML файла", this );
	m_actionXmlToDb->setShortcut( Qt::CTRL + Qt::Key_L );
	connect( m_actionXmlToDb, SIGNAL( triggered() ), SLOT( loadXmlToDb() ) );

	m_actionDocOrder = new QAction( QIcon(":/doc_order.png"), "Приказ", this );
	m_actionDocOrder->setShortcut( Qt::CTRL + Qt::Key_O );
	connect( m_actionDocOrder, SIGNAL( triggered() ), SLOT( docOrder() ) );

	m_actionDocNote = new QAction( QIcon(":/doc_note.png"), "Служебная записка", this );
	m_actionDocNote->setShortcut( Qt::CTRL + Qt::Key_U );
	connect( m_actionDocNote, SIGNAL( triggered() ), SLOT( docNote() ) );
}

void
FormMain::createToolBar()
{
	m_actionsToolBar = addToolBar("Панель &действий");
	m_actionsToolBar->setIconSize( QSize( 64, 64 ) );
	m_actionsToolBar->setObjectName("ActionsToolBar");

	m_actionsToolBar->addAction( m_actionNewNumber );
	m_actionsToolBar->addAction( m_actionXmlToDb );
	m_actionsToolBar->addAction( m_actionDocOrder );
	m_actionsToolBar->addAction( m_actionDocNote );
}

void
FormMain::createMessageDock()
{
	m_dockMessage = new QDockWidget("Сообщения", this );
	m_dockMessage->setObjectName("DockMessage");

	TextEdit * editMessage = new TextEdit( m_dockMessage );

	m_dockMessage->setWidget( editMessage );

	PgQuery::setTextEdit( editMessage );

	addDockWidget( Qt::BottomDockWidgetArea, m_dockMessage );


	    //menuView->addAction( dock->toggleViewAction() );
}

void
FormMain::newNumber()
{
	DialogNumber d( this );

	d.exec();
}

void
FormMain::createMainMenu()
{
	m_menuFile = menuBar()->addMenu("&Файл");
	m_menuFile->addAction( QIcon::fromTheme("application-exit", QIcon(":/exit.png") ),
			"Вы&ход", this, SLOT( close() ),
#ifdef Q_OS_WIN32
			Qt::CTRL + Qt::Key_Q
#else
			QKeySequence::Quit
#endif
			);
	connect( m_menuFile, SIGNAL( aboutToShow() ), SLOT( aboutToShowMenuFile() ) );
	connect( m_menuFile, SIGNAL( aboutToHide() ), SLOT( aboutToHideMenuFile() ) );

	QMenu * menuView = menuBar()->addMenu("&Вид");
	menuView->addAction( m_actionsToolBar->toggleViewAction() );
	menuView->addAction( m_dockMessage->toggleViewAction() );

	QMenu * menuData = menuBar()->addMenu("&Данные");
	m_actionDataTarif = menuData->addAction( QIcon(":/ruble.png"), "&Тарифные планы", this, SLOT( gridTarif() ), Qt::CTRL + Qt::Key_T );

	m_menuActions = menuBar()->addMenu("&Действия");
	m_menuActions->addAction( m_actionNewNumber );
	m_menuActions->addAction( m_actionXmlToDb );
	m_menuActions->addAction( m_actionDocOrder );
	m_menuActions->addAction( m_actionDocNote );
	connect( m_menuActions, SIGNAL( aboutToShow() ), SLOT( aboutToShowMenuActions() ) );
	connect( m_menuActions, SIGNAL( aboutToHide() ), SLOT( aboutToHideMenuActions() ) );

	QMenu * menuHelp = menuBar()->addMenu("&Помощь");
	menuHelp->addAction( windowIcon(), "О &программе", this, SLOT( about() ) );
}

void
FormMain::gridTarif()
{
	static const QString gridTarifCaption("&Тарифные планы");

	if ( ! _tab->haveLabel( gridTarifCaption ) )
		_tab->addTab( new GridTarif( _tab ), gridTarifCaption );
}

void
FormMain::aboutToShowMenuActions()
{
	GridWidget * gridWidget = qobject_cast< GridWidget * >( _tab->currentWidget() );

	if ( gridWidget )
		m_gridWidgetMenuAction = m_menuActions->addMenu( gridWidget->menu() );
}

void
FormMain::aboutToHideMenuActions()
{
	if ( m_gridWidgetMenuAction ) {
		m_menuActions->removeAction( m_gridWidgetMenuAction );
		m_gridWidgetMenuAction = 0;
	}
}

void
FormMain::loadXmlToDb()
{
	DialogLoadXmlToDb d( this );

	if ( d.exec() )
		m_widgetNumber->setMonth( d.month() );
}

void
FormMain::docOrder()
{
	DialogDoc d( this, m_actionDocOrder );

	d.setMonth( m_widgetNumber->month() );

	if ( d.exec() ) {
		const QString docOrderCaption("&Приказ за " + d.month().toString() );
		if ( ! _tab->haveLabel( docOrderCaption ) )
			_tab->addTab( new DocOrder( _tab, d.month(), d.threshold() ),
					docOrderCaption, m_actionDocOrder->icon() );
	}
}

void
FormMain::docNote()
{
	DialogDoc d( this, m_actionDocNote );

	d.setMonth( m_widgetNumber->month() );

	if ( d.exec() ) {
		const QString docNoteCaption("&Служебная записка за " + d.month().toString() );
		if ( ! _tab->haveLabel( docNoteCaption ) )
			_tab->addTab( new DocNote( _tab, d.month(), d.threshold() ),
					docNoteCaption, m_actionDocNote->icon() );
	}
}

void
FormMain::about()
{
	QMessageBox::about( this, "О программе mobilizer",
            QString("<h4>mobilizer</h4><BR>"
		            "Версия: %1<BR><BR>"
		            "%2 2014 ЗАО &laquo;Нордавиа-РА&raquo;<BR><BR>"
		            "разработка: Чурсанов Сергей Николаевич<BR>"
		            "e-mail: <A HREF=\"mailto:masakra@mail.ru\">masakra@mail.ru</A><BR>"
		            "jabber: masakra@jabber.ru<BR>"
		            "ICQ: 124231040"
		            )
		            .arg( qApp->applicationVersion() )
		            .arg( QChar( 0x00A9 ) ) // (c)
	        );
}

void
FormMain::aboutToShowMenuFile()
{
	TextEdit * textEdit = qobject_cast< TextEdit * >( _tab->currentWidget() );

	if ( textEdit ) {
		QAction * firstAction = m_menuFile->actions().at( 0 );
		m_menuFile->insertAction( firstAction, m_actionPrint = textEdit->actionPrint() );
		m_menuFile->insertAction( firstAction, m_actionToPdf = textEdit->actionToPdf() );
	}
}

void
FormMain::aboutToHideMenuFile()
{
	if ( m_actionPrint ) {
		m_menuFile->removeAction( m_actionPrint );
		m_actionPrint = 0;
	}

	if ( m_actionToPdf ) {
		m_menuFile->removeAction( m_actionToPdf );
		m_actionToPdf = 0;
	}
}

