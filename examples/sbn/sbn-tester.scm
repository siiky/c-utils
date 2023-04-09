(import
  (chicken io)
  (chicken port)
  (chicken process)
  (chicken string)

  matchable)

(define n->s (cute number->string <> 16))
(define op->s ->string)

(define run-test-case
  (match-lambda
    ((tc-number (op a b) '-> expected)
     (let ((op (op->s op))
           (a (n->s a))
           (b (n->s b))
           (expected (n->s expected)))
       (define args `(,op ,a ,b ,expected))
       (with-output-to-port
         (current-error-port)
         (lambda ()
           (display (conc "Running " tc-number ": " `(,op ,a ,b) " = " expected " ... "))
           (receive (pid normal-exit? exit-status/signal) (process-wait (process-run "./sbn-tester" `(,op ,a ,b ,expected)))
             (cond
               ((and normal-exit? (= exit-status/signal 0))
                (print "OK"))
               ((and normal-exit? (= exit-status/signal 1))
                (print "UNIMPLEMENTED")) ; Ignore this result, keep going
               (normal-exit?
                 (print "KO")
                 (exit 1))
               (else
                 (print "EXITED W/ SIGNAL " exit-status/signal)
                 (exit 1))))))))))

(load "arithmetic-test.64.expected" run-test-case)
