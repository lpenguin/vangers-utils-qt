#include "resourceviewer.h"

ResourceViewer::ResourceViewer(ResourceViewerPlugin* plugin, QWidget* parent)
    : QWidget(parent)
    , _plugin(plugin)
{}

ResourceViewerPlugin* ResourceViewer::plugin() const
{
    return _plugin;
}
