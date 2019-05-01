(import
  chicken.base
  chicken.foreign
  chicken.format
  chicken.process-context
  scheme
  srfi-1
  )

#>
#include <stdbool.h>
bool qc_bs_test_all (void);
bool qc_map_test_all (void);
bool qc_vec_test_all (void);
<#

(define *TESTS*
  `(
    (bs  . ,(foreign-lambda bool "qc_bs_test_all"))
    (map . ,(foreign-lambda bool "qc_map_test_all"))
    (vec . ,(foreign-lambda bool "qc_vec_test_all"))
    ))

(define (usage cmd)
  (print
    "Usage:\n"
    "\t" cmd "\n"
    "\t\tTest all modules\n"
    "\t" cmd " x MOD...\n"
    "\t\tTest all modules but the ones specified\n"
    "\t" cmd " o MOD...\n"
    "\t\tTest only the specified modules\n"
    "\n"
    "Modules available for testing: "
    (map car *TESTS*))
  1)

(define (run-all tests)
  (fold (lambda (e acc) (+ acc (if ((cdr e)) 0 1))) 0 tests))

(define (run-only tests mods)
  (run-all (filter (lambda (test) (member (symbol->string (car test)) mods)) tests)))

(define (run-except tests mods)
  (run-all (filter (lambda (test) (not (member (symbol->string (car test)) mods))) tests)))

(define (main args)
  (let ((usg    (lambda (_) (usage (program-name))))
        (all    (lambda (_) (run-all    *TESTS*)))
        (only   (lambda (_) (run-only   *TESTS* (cdr args))))
        (except (lambda (_) (run-except *TESTS* (cdr args)))))
    (exit
      (cond
        ((= (length args) 1)       => usg)
        ((null? args)              => all)
        ((string=? "o" (car args)) => only)
        ((string=? "x" (car args)) => except)
        (else                      => usg)))))

(main (command-line-arguments))
