#include "PortalsConnector.h"
#include "tile.h"
void PortalsConnector::AddPortal(Portal* portal) {
    auto connection = PORTALS_CONNECTIONS.find(portal->getPortalId());
    if (connection == PORTALS_CONNECTIONS.end()) {
        PORTALS_CONNECTIONS.insert({portal->getPortalId(), PortalConnection{portal, nullptr}});
    }
    else {
        assert(((*connection).second.secondPortal == nullptr)
               && " More than two portals with the same ID.");

        connection->second.secondPortal = portal;

        connection->second.firstPortal->setSiblingPortal(connection->second.secondPortal);
        connection->second.secondPortal->setSiblingPortal(connection->second.firstPortal);
    }
}

PortalsConnector::PortalsConnector() {}
