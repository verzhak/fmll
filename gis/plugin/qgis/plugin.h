
#ifndef PLUGIN_H
#define PLUGIN_H

#ifdef WIN32

	#define QGISEXTERN extern "C" __declspec( dllexport )

#else

	#define QGISEXTERN extern "C"

#endif

#include <QObject>
#include <QAction>
#include <qgisplugin.h>
#include <qgisinterface.h>

class CSFirePlugin : public QObject, public QgisPlugin
{
	Q_OBJECT

	QgisInterface * iface;
	QAction * action;

	public:
		
		CSFirePlugin(QgisInterface * t_iface);
		~CSFirePlugin();

		void initGui();
		void unload();

	private slots:

		void menu_entry();
};

#endif

