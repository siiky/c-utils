#include <stdbool.h>
#include <theft.h>

#include "vec.h"

bool qc_vec_append_test_all       (void);
bool qc_vec_as_mut_slice_test_all (void);
bool qc_vec_elem_test_all         (void);
bool qc_vec_filter_test_all       (void);
bool qc_vec_find_test_all         (void);
bool qc_vec_get_nth_test_all      (void);
bool qc_vec_pop_test_all          (void);
bool qc_vec_push_test_all         (void);
bool qc_vec_remove_test_all       (void);
bool qc_vec_swap_remove_test_all  (void);

#include <common.h>

QC_MKTEST_ALL(qc_vec_test_all,
        qc_vec_append_test_all,
        qc_vec_elem_test_all,
        qc_vec_filter_test_all,
        qc_vec_find_test_all,
        qc_vec_get_nth_test_all,
        qc_vec_pop_test_all,
        qc_vec_push_test_all,
        qc_vec_remove_test_all,
        qc_vec_swap_remove_test_all,
        );
