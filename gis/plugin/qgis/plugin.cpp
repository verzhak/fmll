
#include "plugin.h"

CSFirePlugin :: CSFirePlugin(QgisInterface * t_iface) : iface(t_iface), action(0)
{
	;
}

CSFirePlugin :: ~CSFirePlugin()
{
	;
}

void CSFirePlugin :: initGui()
{
	action = new QAction(tr("&TODO sfire plugin"), this);
	connect(action, SIGNAL(activated()), this, SLOT(menu_entry()));
	iface->addToolBarIcon(action);
	iface->addPluginToMenu(tr("&TODO sfire plugin"), action);
}

void CSFirePlugin :: unload()
{
	iface->removeToolBarIcon(action);
	iface->removePluginMenu(tr("&TODO sfire plugin"), action);
	delete action;
}

void CSFirePlugin :: menu_entry()
{
	qWarning("TODO sfire plugin");
}

QGISEXTERN QgisPlugin * classFactory(QgisInterface * t_iface)
{
	return new CSFirePlugin(t_iface);
}

QGISEXTERN QString name()
{
	return "TODO sfire plugin name";
}

QGISEXTERN QString description()
{
	return "TODO sfire plugin desc";
}

QGISEXTERN QString version()
{
	return "0.1";
}

QGISEXTERN int type()
{
	return QgisPlugin::UI;
}

QGISEXTERN void unload(QgisPlugin * plugin)
{
	delete plugin;
}

