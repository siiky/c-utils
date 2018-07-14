#ifndef _DEFS_H
#define _DEFS_H

#include <theft.h>

enum theft_alloc_res qc_vec_alloc (struct theft * t, void * env, void ** output);
void qc_vec_free (void * instance, void * env);
void qc_vec_print (FILE * f, const void * instance, void * env);

extern const struct theft_type_info qc_vec_info;

#endif /* _DEFS_H */
