/*
For general Sqliteman copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Sqliteman
for which a new license (GPL+exception) is in place.
*/

#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>

#include "createtriggerdialog.h"
#include "sqleditor.h"
#include "database.h"


CreateTriggerDialog::CreateTriggerDialog(const QString & name, const QString & schema, QWidget * parent)
	: QDialog(parent),
	update(false)
{
	ui.setupUi(this);
	new SqlEditorTools::SqlHighlighter(ui.textEdit->document());

	ui.textEdit->setText(
						 QString("-- sqlite3 simple trigger template\n\
CREATE TRIGGER [IF NOT EXISTS] \"<trigger_name>\"\n\
   [ BEFORE | AFTER ]\n\
   DELETE | INSERT | UPDATE | UPDATE OF <column-list>\n\
   ON %1.%2\n\
   [ FOR EACH ROW | FOR EACH STATEMENT ] [ WHEN expression ]\n\
BEGIN\n\
    <select * from foo;>\n\
END;").arg(schema).arg(name));

	connect(ui.createButton, SIGNAL(clicked()), this, SLOT(createButton_clicked()));
}


void CreateTriggerDialog::createButton_clicked()
{
	QString sql(ui.textEdit->toPlainText());
	QSqlQuery query(sql, QSqlDatabase::database(SESSION_NAME));
	
	if(query.lastError().isValid())
	{
		ui.resultEdit->setText(tr("Error while creating trigger: %2.\n\n%3").arg(query.lastError().databaseText()).arg(sql));
		return;
	}
	ui.resultEdit->setText(tr("Trigger created successfully"));
	update = true;
}