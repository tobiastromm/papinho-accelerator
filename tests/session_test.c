#include "session.h"

static int papacc_test_initializer_and_zero_capacity(void)
{
    PAPACC_SESSION session = PAPACC_SESSION_INITIALIZER;
    PAPACC_SESSION_MANAGER manager = PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_SESSION *published = &session;

    papacc_session_close(NULL);
    papacc_session_close(&session);
    papacc_session_manager_shutdown(NULL);
    papacc_session_manager_shutdown(&manager);
    if (session.session_instance_id != 0 ||
        session.state != PAPACC_SESSION_STATE_UNINITIALIZED ||
        papacc_session_activate(NULL) != PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_session_activate(&session) != PAPACC_RESULT_INVALID_STATE ||
        papacc_session_manager_init(NULL, NULL, 0) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_session_manager_init(&manager, NULL, 1) !=
            PAPACC_RESULT_INVALID_ARGUMENT ||
        papacc_session_manager_init(&manager, NULL, 0) != PAPACC_RESULT_OK ||
        manager.initialized != PAPACC_TRUE || manager.count != 0 ||
        papacc_session_manager_publish(&manager, &published) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        published != NULL) {
        return 1;
    }
    papacc_session_manager_shutdown(&manager);
    return 0;
}

static int papacc_test_publication_lifecycle_and_lookup(void)
{
    PAPACC_SESSION_MANAGER manager = PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_SESSION storage[3];
    PAPACC_SESSION *sessions[3] = { NULL, NULL, NULL };
    PAPACC_SESSION *first_pointer;
    PAPACC_U64 ids[3];
    PAPACC_SIZE index;

    if (papacc_session_manager_init(&manager, storage, 3) != PAPACC_RESULT_OK ||
        papacc_session_manager_init(&manager, storage, 3) !=
            PAPACC_RESULT_INVALID_STATE) {
        return 10;
    }
    for (index = 0; index < 3; ++index) {
        if (papacc_session_manager_publish(&manager, &sessions[index]) !=
                PAPACC_RESULT_OK ||
            sessions[index] == NULL ||
            sessions[index]->session_instance_id == 0 ||
            sessions[index]->state != PAPACC_SESSION_STATE_ESTABLISHING) {
            return 11;
        }
        ids[index] = sessions[index]->session_instance_id;
    }
    first_pointer = sessions[0];
    if (ids[0] == ids[1] || ids[0] == ids[2] || ids[1] == ids[2] ||
        sessions[0] != first_pointer || manager.count != 3 ||
        papacc_session_manager_find(&manager, ids[0]) != sessions[0] ||
        papacc_session_manager_find(&manager, 0) != NULL ||
        papacc_session_manager_find(&manager, 9999) != NULL) {
        return 12;
    }
    sessions[0] = first_pointer;
    if (papacc_session_activate(sessions[0]) != PAPACC_RESULT_OK ||
        sessions[0]->state != PAPACC_SESSION_STATE_ACTIVE ||
        papacc_session_activate(sessions[0]) != PAPACC_RESULT_INVALID_STATE) {
        return 13;
    }
    papacc_session_close(sessions[0]);
    papacc_session_close(sessions[0]);
    papacc_session_close(sessions[1]);
    if (sessions[0]->state != PAPACC_SESSION_STATE_CLOSED ||
        sessions[0]->session_instance_id != ids[0] ||
        sessions[1]->state != PAPACC_SESSION_STATE_CLOSED ||
        sessions[1]->session_instance_id != ids[1]) {
        return 14;
    }
    if (papacc_session_manager_remove(&manager, ids[0]) != PAPACC_RESULT_OK ||
        storage[0].state != PAPACC_SESSION_STATE_UNINITIALIZED ||
        storage[0].session_instance_id != 0 || manager.count != 2 ||
        papacc_session_manager_remove(&manager, ids[1]) != PAPACC_RESULT_OK ||
        papacc_session_manager_remove(&manager, ids[2]) != PAPACC_RESULT_OK ||
        manager.count != 0 ||
        papacc_session_manager_remove(&manager, ids[0]) !=
            PAPACC_RESULT_INVALID_ARGUMENT) {
        return 15;
    }
    papacc_session_manager_shutdown(&manager);
    return 0;
}

static int papacc_test_capacity_reuse_wrap_and_shutdown(void)
{
    PAPACC_SESSION_MANAGER manager = PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_SESSION storage[3];
    PAPACC_SESSION *first = NULL;
    PAPACC_SESSION *second = NULL;
    PAPACC_SESSION *third = NULL;
    PAPACC_SESSION *failed = (PAPACC_SESSION *)storage;
    PAPACC_U64 first_id;

    if (papacc_session_manager_init(&manager, storage, 1) != PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&manager, &first) != PAPACC_RESULT_OK) {
        return 20;
    }
    first_id = first->session_instance_id;
    if (papacc_session_manager_publish(&manager, &failed) !=
            PAPACC_RESULT_LIMIT_EXCEEDED ||
        failed != NULL || first->session_instance_id != first_id ||
        first->state != PAPACC_SESSION_STATE_ESTABLISHING ||
        papacc_session_manager_remove(&manager, first_id) != PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&manager, &second) != PAPACC_RESULT_OK ||
        second->session_instance_id == first_id) {
        return 21;
    }
    papacc_session_manager_shutdown(&manager);
    if (storage[0].state != PAPACC_SESSION_STATE_UNINITIALIZED ||
        manager.initialized != PAPACC_FALSE || manager.count != 0 ||
        papacc_session_manager_init(&manager, storage, 3) != PAPACC_RESULT_OK) {
        return 22;
    }
    manager.next_instance_id = (PAPACC_U64)-1;
    if (papacc_session_manager_publish(&manager, &first) != PAPACC_RESULT_OK ||
        first->session_instance_id != (PAPACC_U64)-1 ||
        papacc_session_manager_publish(&manager, &second) != PAPACC_RESULT_OK ||
        second->session_instance_id == 0) {
        return 23;
    }
    manager.next_instance_id = (PAPACC_U64)-1;
    if (papacc_session_manager_publish(&manager, &third) != PAPACC_RESULT_OK ||
        third->session_instance_id == 0 ||
        third->session_instance_id == first->session_instance_id ||
        third->session_instance_id == second->session_instance_id) {
        return 24;
    }
    if (papacc_session_activate(second) != PAPACC_RESULT_OK) {
        return 25;
    }
    papacc_session_close(third);
    papacc_session_manager_shutdown(&manager);
    papacc_session_manager_shutdown(&manager);
    if (storage[0].state != PAPACC_SESSION_STATE_UNINITIALIZED ||
        storage[1].state != PAPACC_SESSION_STATE_UNINITIALIZED ||
        storage[2].state != PAPACC_SESSION_STATE_UNINITIALIZED ||
        manager.initialized != PAPACC_FALSE) {
        return 26;
    }
    if (papacc_session_manager_init(&manager, storage, 1) != PAPACC_RESULT_OK ||
        papacc_session_manager_publish(&manager, &first) != PAPACC_RESULT_OK) {
        return 27;
    }
    papacc_session_manager_shutdown(&manager);
    return 0;
}

static int papacc_test_malformed_state(void)
{
    PAPACC_SESSION session = PAPACC_SESSION_INITIALIZER;
    PAPACC_SESSION_MANAGER manager = PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_SESSION storage[1];
    PAPACC_SESSION *published = (PAPACC_SESSION *)storage;

    session.session_instance_id = 1;
    session.state = (PAPACC_SESSION_STATE)99;
    if (papacc_session_activate(&session) != PAPACC_RESULT_INVALID_STATE) {
        return 30;
    }
    papacc_session_close(&session);
    if (session.state != (PAPACC_SESSION_STATE)99 ||
        papacc_session_manager_init(&manager, storage, 1) != PAPACC_RESULT_OK) {
        return 31;
    }
    storage[0].state = (PAPACC_SESSION_STATE)99;
    if (papacc_session_manager_publish(&manager, &published) !=
            PAPACC_RESULT_INVALID_STATE ||
        published != NULL || manager.count != 0) {
        return 32;
    }
    papacc_session_manager_shutdown(&manager);
    return 0;
}

int main(void)
{
    int result = papacc_test_initializer_and_zero_capacity();
    if (result == 0) {
        result = papacc_test_publication_lifecycle_and_lookup();
    }
    if (result == 0) {
        result = papacc_test_capacity_reuse_wrap_and_shutdown();
    }
    if (result == 0) {
        result = papacc_test_malformed_state();
    }
    return result;
}
