#include "gnsswebsocketdragmap.h"

GnssWebsocketDragMap::GnssWebsocketDragMap() : mapServer("path/to/mapFiles")

{}


void GnssWebsocketDragMap::openMap()
{
    mapServer.open();
    webSocketRelay.listen(12345);
}