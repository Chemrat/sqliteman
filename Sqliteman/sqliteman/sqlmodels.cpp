/*
For general Sqliteman copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Sqliteman
for which a new license (GPL+exception) is in place.
*/

#include <QColor>
#include <QSqlField>

#include "sqlmodels.h"
#include "database.h"


SqlTableModel::SqlTableModel(QObject * parent, QSqlDatabase db)
	: QSqlTableModel(parent, db),
	m_pending(false),
	m_schema("")
{
	m_deleteCache.clear();
	m_useNull = PreferencesDialog::useNullHighlight();
	m_nullColor = PreferencesDialog::nullHighlightColor();
	m_nullText = PreferencesDialog::nullHighlightText();
	m_useBlob = PreferencesDialog::useBlobHighlight();
	m_blobColor = PreferencesDialog::blobHighlightColor();
	m_blobText = PreferencesDialog::blobHighlightText();

	connect(this, SIGNAL(primeInsert(int, QSqlRecord &)),
			this, SLOT(doPrimeInsert(int, QSqlRecord &)));
}

QVariant SqlTableModel::data(const QModelIndex & item, int role) const
{
	QString curr(QSqlTableModel::data(item, Qt::DisplayRole).toString());

	// nulls
	// FIXME: rewrite to the QSqlField::isNull?
	if (m_useNull && (curr.isNull() || curr.isEmpty()))
	{
		if (role == Qt::BackgroundColorRole)
			return QVariant(m_nullColor);
		if (role == Qt::ToolTipRole)
			return QVariant(tr("NULL value"));
		if (role == Qt::DisplayRole)
			return QVariant(m_nullText);
	}
	// BLOBs
	if (/*f.type.toUpper() == "BLOB" || */
		m_useBlob &&
		record().field(item.column()).type() == QVariant::ByteArray)
	{
		if (role == Qt::BackgroundColorRole)
			return QVariant(m_blobColor);
		if (role == Qt::ToolTipRole)
			return QVariant(tr("BLOB value"));
		if (role == Qt::DisplayRole)
			return QVariant(m_blobText);
		if (role == Qt::EditRole)
			return Database::hex(QSqlTableModel::data(item, Qt::DisplayRole).toByteArray());
	}
	// mark rows prepared for a deletion in this trasnaction
	if (role == Qt::BackgroundColorRole && m_deleteCache.contains(item.row()))
		return QVariant(Qt::red);

	// advanced tooltips
	if (role == Qt::ToolTipRole)
		return QVariant(curr.length() > 6 ? curr.left(6)+"..." : "");
	return QSqlTableModel::data(item, role);
}

bool SqlTableModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
	if (role == Qt::EditRole)
		m_pending = true;
	return QSqlTableModel::setData(index, value, role);
}

void SqlTableModel::doPrimeInsert(int row, QSqlRecord & record)
{
	FieldList fl = Database::tableFields(tableName(), m_schema);
	bool ok;
	QString defval;
	// guess what type is the default value.
	foreach (DatabaseTableField column, fl)
	{
		if (column.defval.isNull())
			continue;
		defval = column.defval;
		defval.toInt(&ok);
		if (!ok)
		{
			defval.toDouble(&ok);
			if (!ok)
			{
				if (defval.left(1) == "'" || defval.left(1) == "\"")
					defval = defval.mid(1, defval.length()-2);
			}
		}
		record.setValue(column.name, QVariant(defval));
	}
}

bool SqlTableModel::insertRows ( int row, int count, const QModelIndex & parent)
{
	m_pending = true;
	return QSqlTableModel::insertRows(row, count, parent);
}

bool SqlTableModel::removeRows ( int row, int count, const QModelIndex & parent)
{
	m_pending = true;
	// this is a workaround to allow mark heading as deletion
	// (as it's propably a bug in Qt QSqlTableModel ManualSubmit handling
	bool ret = QSqlTableModel::removeRows(row, count, parent);
	emit dataChanged( index(row, 0), index(row+count-1, columnCount()-1) );
	emit headerDataChanged(Qt::Vertical, row, row+count-1);
	m_deleteCache.append(row);
	return ret;
}

void SqlTableModel::setPendingTransaction(bool pending)
{
	m_pending = pending;

	// TODO: examine the better way to get only shown/changed lines.
	// If there is one...
	if (!pending)
	{
		 for (int i = 0; i < m_deleteCache.size(); ++i)
			emit headerDataChanged(Qt::Vertical, m_deleteCache[i], m_deleteCache[i]);
	}
	m_deleteCache.clear();
}




SqlQueryModel::SqlQueryModel( QObject * parent)
	: QSqlQueryModel(parent)
{
	m_useNull = PreferencesDialog::useNullHighlight();
	m_nullColor = PreferencesDialog::nullHighlightColor();
	m_nullText = PreferencesDialog::nullHighlightText();
	m_useBlob = PreferencesDialog::useBlobHighlight();
	m_blobColor = PreferencesDialog::blobHighlightColor();
	m_blobText = PreferencesDialog::blobHighlightText();
}

QVariant SqlQueryModel::data(const QModelIndex & item, int role) const
{
	QString curr(QSqlQueryModel::data(item, Qt::DisplayRole).toString());

	if (m_useNull && (curr.isNull() || curr.isEmpty()))
	{
		if (role == Qt::BackgroundColorRole)
			return QVariant(m_nullColor);
		if (role == Qt::ToolTipRole)
			return QVariant(tr("NULL value"));
		if (role == Qt::DisplayRole)
			return QVariant(m_nullText);
	}

	if (m_useBlob && info.field(item.column()).type() == QVariant::ByteArray)
	{
		if (role == Qt::BackgroundColorRole)
			return QVariant(m_blobColor);
		if (role == Qt::ToolTipRole)
			return QVariant(tr("BLOB value"));
		if (role == Qt::DisplayRole)
			return QVariant(m_blobText);
	}
	if (role == Qt::ToolTipRole)
		return QVariant(curr.length() > 6 ? curr.left(6)+"..." : "");
	return QSqlQueryModel::data(item, role);
}

void SqlQueryModel::setQuery ( const QSqlQuery & query )
{
	QSqlQueryModel::setQuery(query);
	info = record();
}

void SqlQueryModel::setQuery ( const QString & query, const QSqlDatabase & db)
{
	QSqlQueryModel::setQuery(query, db);
	info = record();
}