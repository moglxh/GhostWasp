#ifndef GW_SESSION_H
#define GW_SESSION_H

#include <stdint.h>
#include <stddef.h>

#define GW_MAX_SESSIONS 10

struct gw_session {
    int fd;
    uint32_t id;
};

struct gw_session_manager {
    struct gw_session *sessions[GW_MAX_SESSIONS];
    size_t count;
};

struct gw_session *gw_session_create(int fd, uint32_t id);

void gw_session_destroy(struct gw_session *session);

int gw_session_manager_init(struct gw_session_manager *manager );

int gw_session_manager_add(struct gw_session_manager *manager, struct gw_session *session );

struct gw_session *gw_session_manager_find(
    struct gw_session_manager *manager,
    uint32_t id
);

int gw_session_manager_remove(
    struct gw_session_manager *manager,
    uint32_t id
);

void gw_session_manager_destroy(
    struct gw_session_manager *manager
);

#endif
