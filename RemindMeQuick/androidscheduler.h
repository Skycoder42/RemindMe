#ifndef ANDROIDSCHEDULER_H
#define ANDROIDSCHEDULER_H

#include <QObject>
#include <QSettings>
#include <ischeduler.h>

class AndroidScheduler : public QObject, public IScheduler
{
	Q_OBJECT
	Q_INTERFACES(IScheduler)

public:
	explicit AndroidScheduler(QObject *parent = nullptr);

public slots:
	void initialize(const QList<Reminder> &allReminders) override;
	bool scheduleReminder(const Reminder &reminder) override;
	void cancleReminder(const QUuid &id) override;

signals:
	void scheduleTriggered(const QUuid &id) final;

private:
	QSettings *_settings;
	bool _autoSync;
};

#endif // ANDROIDSCHEDULER_H