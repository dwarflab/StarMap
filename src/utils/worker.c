/* Stellarium Web Engine - Copyright (c) 2022 - Stellarium Labs SRL
 *
 * This program is licensed under the terms of the GNU AGPL v3, or
 * alternatively under a commercial licence.
 *
 * The terms of the AGPL v3 license can be found in the main directory of this
 * repository.
 */

#include "worker.h"
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#ifndef HAVE_PTHREAD

#define MAX_THREADS 4

sem_t semaphore;
bool has_init_semaphore = false;

void worker_init_semaphore() {
    if (!has_init_semaphore) {
        sem_init(&semaphore, 0, MAX_THREADS);
        has_init_semaphore = true;
    }
}

void worker_init(worker_t *w, int (*fn)(worker_t *w))
{
    memset(w, 0, sizeof(*w));
    w->fn = fn;
    w->state = 0;
}

void* worker_run(void* arg) {
    worker_t* w = (worker_t*)arg;

    sem_wait(&semaphore);

    w->fn(w);
    w->state = 2;

    sem_post(&semaphore);
    return NULL;
}

int worker_iter(worker_t *w)
{
    if (w->state == 2) {
        return 1;
    }
    else if (w->state == 1) {
        return 0;
    }
    else {
        clock_t start = clock();

        w->state = 1;

        pthread_t work_th;
        pthread_create(&work_th, NULL, worker_run, w);
        pthread_detach(work_th);

        clock_t end = clock();
        double elapsed_time = (double)(end - start);
        printf("Execution time: %f milseconds\n", elapsed_time);

        return 0;
    }
}

bool worker_is_running(worker_t *w)
{
    return w->state == 1;
}

#endif
