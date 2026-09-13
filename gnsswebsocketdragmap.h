#ifndef GNSSWEBSOCKETDRAGMAP_H
#define GNSSWEBSOCKETDRAGMAP_H

#include "websocketrelay.h"

#include "mapserver.h"

class GnssWebsocketDragMap
{
public:
    GnssWebsocketDragMap();

    MapServer mapServer;
    WebSocketRelay webSocketRelay;
    void openMap();
};

#endif // GNSSWEBSOCKETDRAGMAP_H
