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

	QHBoxLayout * layoutMonth = new QHBoxLayout();
	layoutMonth->addWidget( m_editMonth );
	layoutMonth->addStretch();

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
	layout->addLayout( layoutMonth, 1, 1 );
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

	QStringList montly,
				detail;

	QString ds_number;

	//int count = 0;

	while ( ! xml.atEnd() ) {
		if ( xml.readNextStartElement() ) {
			if ( xml.name() == "tp" ) {
				const QString number = xml.attributes().value("n").toString(),
							  bill = xml.attributes().value("a").toString().replace(",", ".");
				montly << QString("(%1,%2,'%3',%4)")
					.arg( m_editMonth->month().year() )
					.arg( m_editMonth->month().month() )
					.arg( number, bill );
				continue;
			}

			if ( xml.name() == "ds" ) {
				ds_number = xml.attributes().value("sim").isEmpty() ? "" : xml.attributes().value("n").toString();
				continue;
			}

			if ( xml.name() == "i" ) {
				const QDateTime start = QDateTime::fromString( xml.attributes().value("d").toString(),
						"dd.MM.yyyy H:mm:ss");
				//qDebug() << ds_number << xml.attributes().value("n").toString() << start.toString();
				const QString pair = xml.attributes().value("n").toString(),
							  type = xml.attributes().value("s").toString(),
							  tz = xml.attributes().value("gmt").toString(),
							  durfact = xml.attributes().value("du").toString().replace(",", "."),
							  duration = xml.attributes().value("dup").toString().replace(",", "."),
							  bill = xml.attributes().value("c").toString().replace(",", ".");
				if ( ! ds_number.isEmpty() && ! bill.isEmpty()/* && ++count > 125 and count <= 150 */)
					detail << QString("('%1','%2','%3','%4','%5','%6',%7)")
						.arg( ds_number )
						.arg( start.toString("yyyy-MM-dd hh:mm:ss ") + tz )
						.arg( pair )
						.arg( type )
						.arg( durfact )
						.arg( duration )
						.arg( bill );

				continue;
			}
		}
	}

	QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	if ( saveMontlyToDb( montly ) && saveDetailToDb( detail ) ) {
		db.commit();
		accept();
	} else
		db.rollback();

	QApplication::restoreOverrideCursor();
}

bool
DialogLoadXmlToDb::saveMontlyToDb( const QStringList & montly ) const
{
	if ( montly.isEmpty() )
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
			"VALUES " + montly.join(",") );

	return q.exec();
}

bool
DialogLoadXmlToDb::saveDetailToDb( const QStringList & detail ) const
{
	if ( detail.isEmpty() )
		return false;

	PgQuery q;

	// удалить все записи за период
	q.prepare("DELETE FROM "
				"\"mobi\".\"detail\" "
			"WHERE "
				"EXTRACT( year FROM start ) = :year "
			"AND EXTRACT( month FROM start ) = :month ");
	q.bindValue(":year", m_editMonth->month().year() );
	q.bindValue(":month", m_editMonth->month().month() );

	if ( ! q.exec() )
		return false;

	// добавить записи
	q.prepare("INSERT INTO "
			"\"mobi\".\"detail\" (number,start,pair,type,durfact,duration,bill) "
			"VALUES " + detail.join(",") );

	return q.exec();
}

Month
DialogLoadXmlToDb::month() const
{
	return m_editMonth->month();
}

