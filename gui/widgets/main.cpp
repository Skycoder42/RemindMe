#include <QApplication>
#include <QtMvvmWidgets/WidgetsPresenter>
#include <QtMvvmDataSyncWidgets/qtmvvmdatasyncwidgets_global.h>
#include <syremapp.h>

#include "mainwindow.h"
#include "createreminderdialog.h"
#include "snoozedialog.h"
#include "snoozetimesedit.h"
#include "termselectiondialog.h"

// Register the core app
QTMVVM_REGISTER_CORE_APP(SyremApp)

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QtMvvm::registerDataSyncWidgets();
	QtMvvm::WidgetsPresenter::registerView<MainWindow>();
	QtMvvm::WidgetsPresenter::registerView<CreateReminderDialog>();
	QtMvvm::WidgetsPresenter::registerView<SnoozeDialog>();
	QtMvvm::WidgetsPresenter::registerView<TermSelectionDialog>();
	QtMvvm::WidgetsPresenter::getInputWidgetFactory()->addSimpleWidget<SnoozeTimes, SnoozeTimesEdit>();

	return a.exec();
}
