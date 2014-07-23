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

#include "DialogLoadXmlToDb.h"
#include <QtGui>
#include <NaraPg>
#include "EditMonth.h"

DialogLoadXmlToDb::DialogLoadXmlToDb( QWidget * parent )
	: QDialog( parent )
{
	setWindowTitle("Загрузка данных из XML файла");
	createWidgets();
	resize( 500, 20 );
}

void
DialogLoadXmlToDb::createWidgets()
{
	m_editPath = new QLineEdit( this );

	QToolButton * buttonPath = new QToolButton( this );
	buttonPath->setIcon( QIcon::fromTheme("folder-open", QIcon(":/folder-open.png") ) );
	connect( buttonPath, SIGNAL( clicked() ), SLOT( openXmlFile() ) );

	QHBoxLayout * layoutPath = new QHBoxLayout();
	layoutPath->addWidget( m_editPath );
	layoutPath->addWidget( buttonPath );

	m_editMonth = new EditMonth( this );

	QLabel * labelPath = new QLabel("&XML файл", this ),
		   * labelMonth = new QLabel("&Месяц", this );

	labelPath->setBuddy( m_editPath );
	labelMonth->setBuddy( m_editMonth );

	QDialogButtonBox * buttonBox = new QDialogButtonBox( QDialogButtonBox::Save | QDialogButtonBox::Close,
			Qt::Horizontal, this );
	connect( buttonBox, SIGNAL( accepted() ), SLOT( save() ) );
	connect( buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

	QGridLayout * layout = new QGridLayout( this );
	layout->addWidget( labelPath, 0, 0, Qt::AlignRight );
	layout->addLayout( layoutPath, 0, 1 );
	layout->addWidget( labelMonth, 1, 0, Qt::AlignRight );
	layout->addWidget( m_editMonth, 1, 1 );
	layout->addWidget( buttonBox, 2, 0, 1, 2 );

}

void
DialogLoadXmlToDb::openXmlFile()
{
	const QString fileName = QFileDialog::getOpenFileName( this, "XML-файл для загрузки в базу данных",
			QString(), "XML файлы ( *.xml );;Все файлы ( *.* )");

	if ( fileName.isEmpty() )
		return;

	m_editPath->setText( fileName );


	QFile xmlFile( fileName );

	if ( ! xmlFile.open( QIODevice::ReadOnly ) )
		return;

	QXmlStreamReader xml( &xmlFile );

	QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

	while ( ! xml.atEnd() ) {
		xml.readNextStartElement();
		if ( xml.name() == "ds" ) {
			const QString sd = xml.attributes().value("sd").toString();
			if ( ! sd.isEmpty() ) {
				const QDate d = QDate::fromString( sd, "dd.MM.yyyy");
				m_editMonth->setMonth( d.month(), d.year() );
				break;
			}
		}
	}

	QApplication::restoreOverrideCursor();

	xmlFile.close();
}

void
DialogLoadXmlToDb::save()
{
	QFile xmlFile( m_editPath->text() );

	if ( ! xmlFile.open( QIODevice::ReadOnly ) )
		return;

	QXmlStreamReader xml( &xmlFile );

	QStringList list;

	while ( ! xml.atEnd() ) {
		if ( xml.readNextStartElement() )
			if ( xml.name() == "tp" ) {
				const QString n = xml.attributes().value("n").toString(),
							  a = xml.attributes().value("a").toString().replace(",", ".");
				list << QString("(%1,%2,'%3',%4)")
					.arg( m_editMonth->month().year() )
					.arg( m_editMonth->month().month() )
					.arg( n, a );
			}
	}

	QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	if ( saveToDb( list ) ) {
		db.commit();
		accept();
	} else
		db.rollback();

	QApplication::restoreOverrideCursor();
}

bool
DialogLoadXmlToDb::saveToDb( const QStringList & list ) const
{
	if ( list.isEmpty() )
		return false;

	PgQuery q;

	// удалить все записи за месяц
	q.prepare("DELETE FROM "
				"\"mobi\".\"montly\" "
			"WHERE "
				"year = :year "
			"AND month = :month");
	q.bindValue(":year", m_editMonth->month().year() );
	q.bindValue(":month", m_editMonth->month().month() );

	if ( ! q.exec() )
		return false;

	// добавить новый записи за месяц
	q.prepare("INSERT INTO "
			"\"mobi\".\"montly\" (year,month,number,bill) "
			"VALUES " + list.join(",") );

	return q.exec();
}

Month
DialogLoadXmlToDb::month() const
{
	return m_editMonth->month();
}

