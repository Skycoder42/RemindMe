#include "snoozecontrol.h"
#include "snoozetimes.h"
#include <QSettings>
#include <coremessage.h>
#include <rep_snoozehelper_replica.h>
#include "remindmeapp.h"

SnoozeControl::SnoozeControl(QObject *parent) :
	Control(parent),
	_snoozeHelper(coreApp->node()->acquire<SnoozeHelperReplica>()),
	_loaded(false),
	_reminderId(),
	_description(tr("<i>Loading Reminder, please wait…</i>")),
	_snoozeTimes()
{
	QSettings settings;
	_snoozeTimes = settings.value(QStringLiteral("daemon/snooze/times"),
								  QVariant::fromValue<SnoozeTimes>({
									  tr("in 20 minutes"),
									  tr("in 1 hour"),
									  tr("in 3 hours"),
									  tr("tomorrow"),
									  tr("in 1 week on Monday")
								  })).value<SnoozeTimes>();

	connect(_snoozeHelper, &SnoozeHelperReplica::reminderLoaded,
			this, &SnoozeControl::reminderLoaded);
	connect(_snoozeHelper, &SnoozeHelperReplica::reminderError,
			this, &SnoozeControl::reminderError);
	connect(_snoozeHelper, &SnoozeHelperReplica::reminderSnoozed,
			this, &SnoozeControl::reminderSnoozed);
}

bool SnoozeControl::isLoaded() const
{
	return _loaded;
}

QString SnoozeControl::description() const
{
	return _description;
}

QStringList SnoozeControl::snoozeTimes() const
{
	return _snoozeTimes;
}

QString SnoozeControl::expression() const
{
	return _expression;
}

void SnoozeControl::show(const QUuid &id, quint32 versionCode)
{
	_reminderId = id;
	if(_snoozeHelper->isInitialized())
		_snoozeHelper->loadReminder(id, versionCode);
	else {
		connect(_snoozeHelper, &SnoozeHelperReplica::initialized, _snoozeHelper, [this, id, versionCode](){
			_snoozeHelper->loadReminder(id, versionCode);
		});
	}
	Control::show();
}

void SnoozeControl::snooze()
{
	_snoozeHelper->snoozeReminder(_reminderId, _expression);
}

void SnoozeControl::setExpression(QString expression)
{
	if (_expression == expression)
		return;

	_expression = expression;
	emit expressionChanged(_expression);
}

void SnoozeControl::reminderLoaded(const QUuid &id, const QString &description)
{
	if(id == _reminderId) {
		_description = tr("Choose a snooze time for the reminder:<br/><i>%1</i>").arg(description);
		emit descriptionChanged(description);
		_loaded = true;
		emit loadedChanged(_loaded);
	}
}

void SnoozeControl::reminderSnoozed(const QUuid &id)
{
	if(id == _reminderId)
		close();
}

void SnoozeControl::reminderError(const QUuid &id, const QString &error, bool close)
{
	if(id == _reminderId) {
		if(close)
			this->close();
		CoreMessage::critical(tr("Failed to snooze reminder"), error);
	}
}
