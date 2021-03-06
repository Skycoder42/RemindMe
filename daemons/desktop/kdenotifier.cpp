#include "kdenotifier.h"

#include <QApplication>
#include <QtMvvmCore/CoreApp>
#include <dialogmaster.h>
#include <snoozeviewmodel.h>

#ifndef QT_NO_DEBUG
#include <QIcon>
#define Icon QIcon(QStringLiteral(":/icons/tray/main.ico")).pixmap(64, 64)
#define ErrorIcon QIcon(QStringLiteral(":/icons/tray/error.ico")).pixmap(64, 64)
#define setNotifyIcon setPixmap
#else
#define Icon QStringLiteral("de.skycoder42.syrem")
#define ErrorIcon QStringLiteral("de.skycoder42.syrem-error")
#define setNotifyIcon setIconName
#endif

KdeNotifier::KdeNotifier(QObject *parent) :
	QObject(parent),
	INotifier(),
	_settings(nullptr),
	_parser(nullptr),
	_notifications()
{}

void KdeNotifier::showNotification(const Reminder &reminder)
{
	auto important = reminder.isImportant();
	auto notification = new KNotification(important ?
											  QStringLiteral("remindimportant") :
											  QStringLiteral("remindnormal"),
										  KNotification::Persistent | KNotification::SkipGrouping,
										  this);
	_notifications.insert(reminder.id(), notification);

	notification->setTitle((important ?
							   tr("%1 — Important Reminder") :
							   tr("%1 — Reminder"))
						   .arg(QApplication::applicationDisplayName()));
	notification->setText(reminder.description());
	notification->setNotifyIcon(Icon);
	notification->setDefaultAction(tr("Open GUI")); //TODO allow change in settings to open URL instead (or both)
	QStringList actions {
		tr("Complete"),
		tr("Snooze")
	};
	if(reminder.hasUrls())
		actions.append(tr("Open URLs"));
	notification->setActions(actions);
	if(important)
		notification->setFlags(notification->flags() | KNotification::LoopSound);

	auto remId = reminder.id();
	auto vCode = reminder.versionCode();
	connect(notification, QOverload<>::of(&KNotification::activated), this, [this, remId](){
		if(removeNot(remId))
			emit messageActivated(remId);
	});
	connect(notification, &KNotification::action1Activated, this, [this, remId, vCode](){
		if(removeNot(remId))
			emit messageCompleted(remId, vCode);
	});
	connect(notification, &KNotification::action2Activated, this, [this, remId](){
		if(removeNot(remId))
			QtMvvm::CoreApp::show<SnoozeViewModel>(SnoozeViewModel::showParams(remId));
	});
	connect(notification, &KNotification::action3Activated, this, [this, remId](){
		if(removeNot(remId))
			emit messageOpenUrls(remId);
	});
	connect(notification, &KNotification::closed, this, [this, remId](){
		removeNot(remId);
	});

	notification->sendEvent();
}

void KdeNotifier::removeNotification(QUuid id)
{
	removeNot(id, true);
}

void KdeNotifier::showErrorMessage(const QString &error)
{
	auto notification = new KNotification(QStringLiteral("error"),
										  KNotification::Persistent | KNotification::SkipGrouping,
										  this);

	notification->setTitle(tr("%1 — Error").arg(QApplication::applicationDisplayName()));
	notification->setText(error);
	notification->setNotifyIcon(ErrorIcon);
	connect(notification, &KNotification::closed, this, [notification]() {
		notification->deleteLater();
	});
	connect(qApp, &QApplication::aboutToQuit,
			notification, &KNotification::close);

	notification->sendEvent();
}

void KdeNotifier::cancelAll()
{
	for(auto notification : qAsConst(_notifications)) {
		notification->close();
		notification->deleteLater();
	}
	_notifications.clear();
}

void KdeNotifier::qtmvvm_init()
{
	connect(qApp, &QApplication::aboutToQuit, this, [this](){
		for(auto notification : qAsConst(_notifications))
			notification->close();
	});
}

bool KdeNotifier::removeNot(QUuid id, bool close)
{
	auto notification = _notifications.take(id);
	if(notification) {
		if(close)
			notification->close();
		notification->deleteLater();
		return true;
	} else
		return false;
}
