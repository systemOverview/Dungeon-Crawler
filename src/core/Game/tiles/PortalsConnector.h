#ifndef PORTALSCONNECTOR_H
#define PORTALSCONNECTOR_H

#include <map>
class Portal;
class PortalsConnector
{
    struct PortalConnection
    {
        Portal* firstPortal;
        Portal* secondPortal;
    };
    inline static std::map<int, PortalConnection> PORTALS_CONNECTIONS = {};

public:
    static void AddPortal(Portal* portal);
    PortalsConnector();
};

#endif // PORTALSCONNECTOR_H
