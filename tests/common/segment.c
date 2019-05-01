typedef long long unsigned segment;

enum theft_alloc_res qc_segment_alloc (struct theft * t, void * env, void ** output)
{
    UNUSED(env);
    segment * x = malloc(sizeof(segment));
    if (x == NULL)
        return THEFT_ALLOC_SKIP;
    *x = (segment) theft_random_choice(t, ~((segment) 0x0));
    *output = x;
    return THEFT_ALLOC_OK;
}

void qc_segment_free (void * instance, void * env)
{
    UNUSED(env);
    free(instance);
}

void qc_segment_print (FILE * f, const void * instance, void * env)
{
    UNUSED(env);
    segment x = * (const segment *) instance;
    fprintf(f, "%llu", x);
}

const struct theft_type_info qc_segment_info = {
    .alloc = qc_segment_alloc,
    .free  = qc_segment_free,
    .print = qc_segment_print,
};
