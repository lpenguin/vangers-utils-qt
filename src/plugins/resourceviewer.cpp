#include "resourceviewer.h"

ResourceViewer::ResourceViewer(ResourceViewerPluginInterface* plugin, QWidget* parent)
    : QWidget(parent)
    , _plugin(plugin)
{}

ResourceViewerPluginInterface* ResourceViewer::plugin() const
{
    return _plugin;
}
