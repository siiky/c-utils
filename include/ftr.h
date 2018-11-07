#ifndef _FTR_H
#define _FTR_H

#include <stdbool.h>

struct ftr {
    bool done;
    void * (* proc) (void *);
    void * args;
    void * ret;
};

bool   ftr_clean (struct ftr * self);
bool   ftr_delay (struct ftr * self, void * (* proc) (void *), void * args);
void * ftr_force (struct ftr * self);

# ifdef _FTR_H_IMPLEMENTATION

#include <stddef.h>

bool   ftr_delay (struct ftr * self, void * (* proc) (void *), void * args)
{
    if (self == NULL || proc == NULL)
        return false;
    ftr_clean(self);
    self->done = false;
    self->proc = proc;
    self->args = args;
    return true;
}

void * ftr_force (struct ftr * self)
{
    if (self == NULL)
        return NULL;

    if (!self->done) {
        self->ret = self->proc(self->args);
        self->done = true;
    }

    return self->ret;
}

bool ftr_clean (struct ftr * self)
{
    if (self != NULL)
        *self = (struct ftr) {0};
    return true;
}

# endif /* _FTR_H_IMPLEMENTATION */
#endif /* _FTR_H */
