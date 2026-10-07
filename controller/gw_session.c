#include "../include/gw_session.h"

#include <stdlib.h>
#include <unistd.h>

struct gw_session *gw_session_create(int fd, uint32_t id)
{
    // Allocate session.
    struct gw_session *session =
        malloc(sizeof *session);

    if (session == NULL)
        return NULL;

    // Initialize session.
    session->fd = fd;
    session->id = id;

    return session;
}

void gw_session_destroy(struct gw_session *session)
{
    if (session == NULL)
        return;

    // Release socket.
    close(session->fd);

    // Release session memory.
    free(session);
}

int gw_session_manager_init(
    struct gw_session_manager *manager)
{
    if (manager == NULL)
        return -1;

    // Initialize manager.
    manager->count = 0;

    for (size_t i = 0; i < GW_MAX_SESSIONS; i++)
        manager->sessions[i] = NULL;

    return 0;
}

int gw_session_manager_add(
    struct gw_session_manager *manager,
    struct gw_session *session)
{
    if (manager == NULL || session == NULL)
        return -1;

    // Find an empty slot.
    for (size_t i = 0; i < GW_MAX_SESSIONS; i++) {
        if (manager->sessions[i] == NULL) {
            manager->sessions[i] = session;
            manager->count++;
            return 0;
        }
    }

    return -1;
}

struct gw_session *gw_session_manager_find(
    struct gw_session_manager *manager,
    uint32_t id)
{
    if (manager == NULL)
        return NULL;

    // Search active sessions.
    for (size_t i = 0; i < GW_MAX_SESSIONS; i++) {
        struct gw_session *session =
            manager->sessions[i];

        if (session == NULL)
            continue;

        if (session->id == id)
            return session;
    }

    return NULL;
}

int gw_session_manager_remove(
    struct gw_session_manager *manager,
    uint32_t id)
{
    if (manager == NULL)
        return -1;

    // Find the session.
    for (size_t i = 0; i < GW_MAX_SESSIONS; i++) {
        struct gw_session *session =
            manager->sessions[i];

        if (session == NULL)
            continue;

        if (session->id != id)
            continue;

        // Manager owns the session.
        gw_session_destroy(session);

        manager->sessions[i] = NULL;
        manager->count--;

        return 0;
    }

    return -1;
}

void gw_session_manager_destroy(
    struct gw_session_manager *manager)
{
    if (manager == NULL)
        return;

    // Destroy all sessions.
    for (size_t i = 0; i < GW_MAX_SESSIONS; i++) {
        if (manager->sessions[i] != NULL) {
            gw_session_destroy(manager->sessions[i]);
            manager->sessions[i] = NULL;
        }
    }

    manager->count = 0;
}
