#include <common.h>

bool qc_vec_append_test_all        (void);
bool qc_vec_as_mut_slice_test_all  (void);
bool qc_vec_elem_test_all          (void);
bool qc_vec_filter_test_all        (void);
bool qc_vec_find_test_all          (void);
bool qc_vec_foreach_range_test_all (void);
bool qc_vec_foreach_test_all       (void);
bool qc_vec_get_nth_test_all       (void);
bool qc_vec_insert_test_all        (void);
bool qc_vec_is_empty_test_all      (void);
bool qc_vec_iter_end_test_all      (void);
bool qc_vec_iter_next_test_all     (void);
bool qc_vec_iter_rev_test_all      (void);
bool qc_vec_iter_test_all          (void);
bool qc_vec_itering_test_all       (void);
bool qc_vec_map_test_all           (void);
bool qc_vec_pop_test_all           (void);
bool qc_vec_push_test_all          (void);
bool qc_vec_remove_test_all        (void);
bool qc_vec_swap_remove_test_all   (void);

QC_MKTEST_ALL(qc_vec_test_all,
        qc_vec_append_test_all,
        qc_vec_as_mut_slice_test_all,
        qc_vec_elem_test_all,
        qc_vec_filter_test_all,
        qc_vec_find_test_all,
        qc_vec_foreach_range_test_all,
        qc_vec_foreach_test_all,
        qc_vec_get_nth_test_all,
        qc_vec_insert_test_all,
        qc_vec_is_empty_test_all,
        qc_vec_iter_end_test_all,
        qc_vec_iter_next_test_all,
        qc_vec_iter_rev_test_all,
        qc_vec_iter_test_all,
        qc_vec_itering_test_all,
        qc_vec_map_test_all,
        qc_vec_pop_test_all,
        qc_vec_push_test_all,
        qc_vec_remove_test_all,
        qc_vec_swap_remove_test_all,
        );
