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

#include "DialogNumber.h"

#include <QtGui>
#include <NaraGui>
#include <NaraPg>
#include "SpinLimit.h"

#define TARIF_ID_IDX 0
#define TARIF_LIMIT_IDX 1

DialogNumber::DialogNumber( QWidget * parent )
	: Dialog( parent )
{
	setWindowIcon( QIcon(":/new_number.png") );
	setWindowTitle("Номер");
	createWidgets();
	fillComboTarif();
	fillComboCity();
	editNumberChanged( m_editNumber->text() );
}

void
DialogNumber::createWidgets()
{
	m_editNumber = new QLineEdit( this );
	m_editNumber->setMaxLength( 11 );
	m_editNumber->setValidator( new QRegExpValidator( QRegExp("[1-9]\\d{10}"), m_editNumber ) );
	connect( m_editNumber, SIGNAL( textChanged( const QString & ) ), SLOT( editNumberChanged( const QString & ) ) );

	m_groupMan = new QGroupBox("&Пользователь");
	m_groupMan->setCheckable( true );

	m_radioManNew = new QRadioButton("Создать нового", this );
	m_radioManExists = new QRadioButton("Выбрать существующего", this );
	m_radioManPseudo = new QRadioButton("П&севдопользователь");

	m_editFam = new QLineEdit( this );
	m_editFam->setMaxLength( 32 );
	connect( m_editFam, SIGNAL( textChanged( const QString & ) ), SLOT( editFamChanged( const QString & ) ) );

	m_editNam = new QLineEdit( this );
	m_editNam->setMaxLength( 32 );

	m_editPat = new QLineEdit( this );
	m_editPat->setMaxLength( 32 );

	m_comboSex = new ComboBox( this );
	m_comboSex->addItem( "муж.", true );
	m_comboSex->addItem( "жен.", false );

	m_editBirth = new QDateEdit( this );
	m_editBirth->setDisplayFormat("dd.MM.yyyy");

	QLabel * labelFam = new QLabel("&Фамилия"),
		   * labelNam = new QLabel("&Имя"),
		   * labelPat = new QLabel("&Отчество"),
		   * labelSex = new QLabel("&Пол"),
		   * labelBirth = new QLabel("Дата &рождения");

	labelFam->setBuddy( m_editFam );
	labelNam->setBuddy( m_editNam );
	labelPat->setBuddy( m_editPat );
	labelSex->setBuddy( m_comboSex );
	labelBirth->setBuddy( m_editBirth );

	m_listManExists = new QListWidget( this );

	m_editPseudo = new QLineEdit( this );
	m_editPseudo->setMaxLength( 64 );

	m_comboTarif = new ComboBox( this );
	connect( m_comboTarif, SIGNAL( currentIndexChanged( int ) ), SLOT( tarifChanged( int ) ) );

	m_spinLimit = new SpinLimit( this );

	m_comboCity = new ComboBox( this );

	m_editStart = new QDateEdit( this );
	m_editStart->setDisplayFormat("dd.MM.yyyy");
	m_editStart->setDate( QDate::currentDate() );

	m_editComm = new TextEdit( this, 2 );

	QLabel * labelTarif = new QLabel("&Тарифный план"),
		   * labelLimit = new QLabel("&Лимит"),
		   * labelCity = new QLabel("&Город"),
		   * labelStart = new QLabel("&Дата соглашения"),
		   * labelComm = new QLabel("&Примечание");

	labelTarif->setBuddy( m_comboTarif );
	labelLimit->setBuddy( m_spinLimit );
	labelCity->setBuddy( m_comboCity );
	labelStart->setBuddy( m_editStart );
	labelComm->setBuddy( m_editComm );

	m_checkDevice = new QCheckBox("&Выдан телефон", this );

	QGridLayout * layoutMan = new QGridLayout( m_groupMan );
	layoutMan->addWidget( m_radioManNew, 0, 0, 1, 2 );
	layoutMan->addWidget( m_radioManExists, 0, 2, 1, 2 );
	layoutMan->addWidget( labelFam, 1, 0, Qt::AlignRight );
	layoutMan->addWidget( m_editFam, 1, 1 );
	layoutMan->addWidget( labelNam, 2, 0, Qt::AlignRight );
	layoutMan->addWidget( m_editNam, 2, 1 );
	layoutMan->addWidget( labelPat, 3, 0, Qt::AlignRight );
	layoutMan->addWidget( m_editPat, 3, 1 );
	layoutMan->addWidget( labelSex, 4, 0, Qt::AlignRight );
	layoutMan->addWidget( m_comboSex, 4, 1 );
	layoutMan->addWidget( labelBirth, 5, 0, Qt::AlignRight );
	layoutMan->addWidget( m_editBirth, 5, 1 );
	layoutMan->addWidget( m_listManExists, 1, 2, 5, 2 );
	layoutMan->addWidget( m_radioManPseudo, 6, 0, 1, 2 );
	layoutMan->addWidget( m_editPseudo, 6, 2, 1, 2 );
	layoutMan->addWidget( labelTarif, 7, 0, Qt::AlignRight );
	layoutMan->addWidget( m_comboTarif, 7, 1, 1, 3 );
	layoutMan->addWidget( labelLimit, 8, 0, Qt::AlignRight );
	layoutMan->addWidget( m_spinLimit, 8, 1 );
	layoutMan->addWidget( labelCity, 9, 0, Qt::AlignRight );
	layoutMan->addWidget( m_comboCity, 9, 1, 1, 3 );
	layoutMan->addWidget( labelStart, 10, 0, Qt::AlignRight );
	layoutMan->addWidget( m_editStart, 10, 1 );
	layoutMan->addWidget( m_checkDevice, 10, 2 );
	layoutMan->addWidget( labelComm, 11, 0, Qt::AlignRight | Qt::AlignTop );
	layoutMan->addWidget( m_editComm, 11, 1, 1, 3 );

	connect( m_radioManNew, SIGNAL( toggled( bool ) ), m_editNam, SLOT( setEnabled( bool ) ) );
	connect( m_radioManNew, SIGNAL( toggled( bool ) ), labelNam, SLOT( setEnabled( bool ) ) );
	connect( m_radioManNew, SIGNAL( toggled( bool ) ), m_editPat, SLOT( setEnabled( bool ) ) );
	connect( m_radioManNew, SIGNAL( toggled( bool ) ), labelPat, SLOT( setEnabled( bool ) ) );
	connect( m_radioManNew, SIGNAL( toggled( bool ) ), m_comboSex, SLOT( setEnabled( bool ) ) );
	connect( m_radioManNew, SIGNAL( toggled( bool ) ), labelSex, SLOT( setEnabled( bool ) ) );
	connect( m_radioManNew, SIGNAL( toggled( bool ) ), m_editBirth, SLOT( setEnabled( bool ) ) );
	connect( m_radioManNew, SIGNAL( toggled( bool ) ), labelBirth, SLOT( setEnabled( bool ) ) );

	connect( m_radioManExists, SIGNAL( toggled( bool ) ), m_listManExists, SLOT( setEnabled( bool ) ) );

	connect( m_radioManPseudo, SIGNAL( toggled( bool ) ), m_editFam, SLOT( setDisabled( bool ) ) );
	connect( m_radioManPseudo, SIGNAL( toggled( bool ) ), labelFam, SLOT( setDisabled( bool ) ) );
	connect( m_radioManPseudo, SIGNAL( toggled( bool ) ), m_editPseudo, SLOT( setEnabled( bool ) ) );

	QLabel * labelNumber = new QLabel("&Номер", this );

	labelNumber->setBuddy( m_editNumber );

	QGridLayout * layout = new QGridLayout( this );

	layout->addWidget( labelNumber, 0, 0 );
	layout->addWidget( m_editNumber, 0, 1 );
	layout->addWidget( m_groupMan, 1, 0, 1, 4 );

	m_radioManExists->toggle();
	m_radioManPseudo->toggle();
	m_radioManNew->toggle();
	m_groupMan->setChecked( false );

	m_buttonBox = new QDialogButtonBox( QDialogButtonBox::Save | QDialogButtonBox::Close,
			Qt::Horizontal, this );
	connect( m_buttonBox, SIGNAL( accepted() ), SLOT( save() ) );
	connect( m_buttonBox, SIGNAL( rejected() ), SLOT( reject() ) );

	layout->addWidget( m_buttonBox, 2, 0, 1, 4 );
}

void
DialogNumber::save()
{
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	PgQuery q;

	if ( m_key.isNull() || ! numberExists( m_key ) ) {		// it is a new number

		QStringList fields( QString("number") ),
					values( "'" + m_editNumber->text() + "'" );

		if ( m_groupMan->isChecked() ) {
			// a Man
			if ( m_radioManNew->isChecked() ) {
				const QVariant man_id = createNewMan();
				if ( man_id.isNull() ) {
					db.rollback();
					return;
				} else {
					fields << "people_id";
					values << man_id.toString();
				}
			} else if ( m_radioManExists->isChecked() ) {
				fields << "people_id";
				values << m_listManExists->currentItem()->data( Qt::UserRole ).toString();
			} else {	// pseudo
				fields << "pseudo";
				values << "'" + m_editPseudo->text().trimmed().replace("'", "`") + "'";

			}
			// Тариф
			fields << "tarif_id";
			values << m_comboTarif->currentData( TARIF_ID_IDX ).toString();
			// Лимит
			fields << "\"limit\"";
			values << QString::number( m_spinLimit->value() );
			// IATA код города
			fields << "city_iata";
			values << "'" + m_comboCity->currentData().toString() + "'";
			// Дата соглашения
			fields << "start";
			values << "'" + m_editStart->date().toString("yyyy-MM-dd") + "'";
			// Выдан телефон
			fields << "device";
			values << ( m_checkDevice->isChecked() ? "true" : "false" );
			// Комментарий
			fields << "comm";
			values << "'" + m_editComm->toPlainText().trimmed().replace("'", "`") + "'";
		}

		q.prepare("INSERT INTO \"mobi\".\"number\" (" + fields.join(",") +
				") VALUES (" + values.join(",") + ") RETURNING number");

	} else {				// update number
		QStringList pairs( QString("number = '%1'").arg( m_editNumber->text() ) );

		if ( m_groupMan->isChecked() ) {
			// a Man
			if ( m_radioManNew->isChecked() ) {
				const QVariant man_id = createNewMan();
				if ( man_id.isNull() ) {
					db.rollback();
					return;
				} else {
					pairs << QString("people_id = %1, pseudo = NULL").arg( man_id.toString() );
				}
			} else if ( m_radioManExists->isChecked() ) {
				pairs << QString("people_id = %1, pseudo = NULL")
					.arg( m_listManExists->currentItem()->data( Qt::UserRole ).toString() );
			} else {		// pseudo
				pairs << QString("people_id = NULL, pseudo = '%1'").arg( m_editPseudo->text().trimmed() );
			}
			// Тариф
			pairs << QString("tarif_id = %1").arg( m_comboTarif->currentData( TARIF_ID_IDX ).toString() );
			// Лимит
			pairs << QString("\"limit\" = %1").arg( m_spinLimit->value() );
			// IATA код города
			pairs << QString("city_iata = '%1'").arg( m_comboCity->currentData().toString() );
			// Дата соглашения
			pairs << QString("start = '%1'").arg( m_editStart->date().toString("yyyy-MM-dd") );
			// Выдан телефон
			pairs << QString("device = %1").arg( m_checkDevice->isChecked() ? "true" : "false" );
			// Комментарий
			pairs << QString("comm = '%1'").arg( m_editComm->toPlainText().trimmed().replace("'", "`") );

		} else {	// удалить все данные о человеке
			pairs << " people_id = NULL, pseudo = NULL, "
				"tarif_id = NULL, \"limit\" = default, city_iata = NULL, "
				"start = default, device = default, comm = NULL";
		}

		q.prepare("UPDATE \"mobi\".\"number\" SET " + pairs.join(",") + " WHERE number = :number");
		q.bindValue(":number", m_key );
	}

	if ( q.exec() ) {
		if ( q.first() )
			m_key = q.value( 0 );

		db.commit();

		QSettings s;
		s.setValue("last_city_iata", m_comboCity->currentData() );

		accept();
	} else
		db.rollback();
}

void
DialogNumber::fillComboTarif()
{
	PgQuery q;

	q.prepare("SELECT "
			"id, "
			"caption, "
			"\"limit\" "
		"FROM "
			"\"mobi\".\"tarif\" "
		"ORDER BY "
			"caption");

	if ( q.exec() ) {
		m_comboTarif->clear();
		while ( q.next() )
			m_comboTarif->addItem( q.value( 1 ).toString(), QVariantList() << q.value( 0 ) << q.value( 2 ) );
	}
}

void
DialogNumber::fillComboCity()
{
	PgQuery q;

	q.prepare("SELECT "
			"iata, "
			"rcap "
		"FROM "
			"\"common\".\"city\" "
		"ORDER BY "
			"rcap");

	if ( q.exec() ) {
		m_comboCity->clear();
		while ( q.next() )
			m_comboCity->addItem( q.value( 1 ).toString(), q.value( 0 ) );

		QSettings s;
		m_comboCity->setCurrentData( s.value("last_city_iata", "ARH") );
	}
}

void
DialogNumber::tarifChanged( int index )
{
	const QVariant itemData = m_comboTarif->itemData( index );
	m_spinLimit->setValue( itemData.toList()[ TARIF_LIMIT_IDX ].toDouble() );
}

void
DialogNumber::editNumberChanged( const QString & text )
{
	QString str( text );
	int pos;
	QPushButton * saveButton = m_buttonBox->button( QDialogButtonBox::Save );

	if ( saveButton )
		saveButton->setEnabled( m_editNumber->validator()->validate( str, pos ) == QValidator::Acceptable );
}

void
DialogNumber::fetchData()		// virtual
{
	PgQuery q;

	q.prepare("SELECT "
			"n.number, "					// 0
			"n.people_id, "					// 1
			"cp.fam, "						// 2 fam
			"cp.nam, "						// 3 nam
			"cp.pat, "						// 4 pat
			"cp.sex, "						// 5 sex
			"cp.birth, "					// 6 дата рожедния
			"n.pseudo, "					// 7 pseudo
			"n.start, "		// 8 don`t used
			"n.tarif_id, "					// 9 tarif ID
			"n.\"limit\", "					// 10 limit
			"n.city_iata, "					// 11 city IATA code
			"n.start, "						// 12 дата соглашения
			"n.device, "					// 13 выдан телефон
			"n.comm "						// 14 Комментарий
		"FROM "
			"\"mobi\".\"number\" n "
		"LEFT OUTER JOIN "
			"\"common\".\"people\" cp ON n.people_id = cp.id "
		"WHERE "
			"number = :number");
	q.bindValue(":number", m_key );

	if ( ! q.exec() )
		return;

	// номер есть в месяцах но нет в таблице mobi.numbers
	m_editNumber->setText( m_key.toString() );
	if ( ! q.first() )
		return;

	// пользователь и псевдопользователь не существуют
	if ( q.value( 1 ).isNull() &&			// people_id
			q.value( 8 ).isNull() )	{		// pseudo
		m_groupMan->setChecked( false );
		return;
	}

	m_groupMan->setChecked( true );

	if ( ! q.value( 1 ).isNull() ) {		// пользователь существует
		m_editFam->setText( q.value( 2 ).toString() );
		m_editNam->setText( q.value( 3 ).toString() );
		m_editPat->setText( q.value( 4 ).toString() );
		m_comboSex->setCurrentData( q.value( 5 ) );
		m_editBirth->setDate( q.value( 6 ).toDate() );
		m_radioManExists->setChecked( true );
		for ( int i = 0; i < m_listManExists->count(); ++i )
			if ( q.value( 1 ) == m_listManExists->item( i )->data( Qt::UserRole ) ) {
				m_listManExists->setCurrentRow( i );
				break;
			}
	} else {								// псевдопользователь существует
		m_editPseudo->setText( q.value( 7 ).toString() );
		m_radioManPseudo->setChecked( true );
	}

	m_comboTarif->setCurrentData( q.value( 9 ), TARIF_ID_IDX );
	m_spinLimit->setValue( q.value( 10 ).toDouble() );
	m_comboCity->setCurrentData( q.value( 11 ) );
	m_editStart->setDate( q.value( 12 ).toDate() );
	m_checkDevice->setChecked( q.value( 13 ).toBool() );
	m_editComm->setText( q.value( 14 ).toString() );
}

void
DialogNumber::editFamChanged( const QString & text )
{
	const QString famStart = text.trimmed().toUpper();

	if ( famStart.size() < 4 ) {
		m_listManExists->clear();
		return;
	}

	// поиск людей в common.people

	PgQuery q;

	q.prepare("SELECT "
			"id, "
			"fam || ' ' || nam || ' ' || pat, "
			"birth "
		"FROM "
			"\"common\".\"people\" "
		"WHERE "
			"upper( fam ) LIKE :famStart");
	q.bindValue(":famStart", famStart + "%");

	if ( q.exec() ) {
		m_listManExists->clear();
		while ( q.next() ) {
			QListWidgetItem * listItem = new QListWidgetItem( q.value( 1 ).toString() );
			listItem->setToolTip( QString("%1, id=%2")
					.arg( q.value( 2 ).toDate().toString("dd.MM.yyyy г.р.") )
					.arg( q.value( 0 ).toInt() ) );
			listItem->setData( Qt::UserRole, q.value( 0 ) );
			m_listManExists->addItem( listItem );
		}
	}
}

bool
DialogNumber::numberExists( const QVariant & number ) const
{
	PgQuery q;

	q.prepare("SELECT "
			"number "
		"FROM "
			"\"mobi\".\"number\" "
		"WHERE "
			"number = :number");
	q.bindValue(":number", number );

	return q.exec() && q.first();
}

QVariant
DialogNumber::createNewMan() const
{
	PgQuery q;

	q.prepare("INSERT INTO \"common\".\"people\" ("
			"fam, "
			"nam, "
			"pat, "
			"birth, "
			"sex "
		") VALUES ("
			":fam, "
			":nam, "
			":pat, "
			":birth, "
			":sex ) "
		"RETURNING "
			"id");
	q.bindValue(":fam", m_editFam->text().trimmed() );
	q.bindValue(":nam", m_editNam->text().trimmed() );
	q.bindValue(":pat", m_editPat->text().trimmed() );
	q.bindValue(":birth", m_editBirth->date() );
	q.bindValue(":sex", m_comboSex->itemData( m_comboSex->currentIndex() ) );

	if ( q.exec() && q.first() )
		return q.value( 0 );

	return QVariant();
}

