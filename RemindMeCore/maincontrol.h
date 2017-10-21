#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <control.h>
#include <QAbstractItemModelReplica>
#include <QRemoteObjectHost>

class MainControl : public Control
{
	Q_OBJECT

	Q_PROPERTY(QAbstractItemModel* reminderModel READ reminderModel CONSTANT)

public:
	explicit MainControl(QObject *parent = nullptr);

	void onShow() override;
	void onClose() override;

	QAbstractItemModel* reminderModel() const;

private:
	QRemoteObjectNode *_node;
	QAbstractItemModelReplica* _reminderModel;
};

#endif // MAINCONTROL_H
