#!/usr/bin/env -S csi -s

(import
  (chicken io)
  (chicken port)
  (chicken process)
  (chicken string)

  matchable)

(define (decompose-digits n b)
  (let loop ((n n)
             (r '()))
    (if (< n b)
      (cons n r)
      (receive (q m) (quotient&remainder n b)
        (loop q (cons m r))))))

(define exit-codes
  `((0 . OK)
    (1 . UNIMPLEMENTED)
    (2 . OPFAIL)
    (3 . BADRESULT)))

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
             (if normal-exit?
               (let ((ret (alist-ref exit-status/signal exit-codes = 'UNEXPECTED)))
                 (print ret)
                 (unless (or (eq? ret 'OK) (eq? ret 'UNIMPLEMENTED))
                   (exit exit-status/signal)))
               (begin
                 (print "EXITED W/ SIGNAL " exit-status/signal)
                 (exit 1))))))))))

(define (op->fn op)
  (case op
    ((+) +)
    ((-) -)
    ((*) *)
    ((/) /)))

(define run-test-case/abs
  (match-lambda
    ((tc-number (op a b) '-> expected)
     (let ((fn (op->fn op))
           (op (op->s op))
           (a (abs a))
           (b (abs b)))
       (let ((a (n->s a))
             (b (n->s b))
             (expected (n->s (fn a b))))
         (define args `(,op ,a ,b ,expected))
         (with-output-to-port
           (current-error-port)
           (lambda ()
             (display (conc "Running " tc-number ": " `(,op ,a ,b) " = " expected " ... "))
             (receive (pid normal-exit? exit-status/signal) (process-wait (process-run "./sbn-tester" `(,op ,a ,b ,expected)))
               (if normal-exit?
                 (let ((ret (alist-ref exit-status/signal exit-codes = 'UNEXPECTED)))
                   (print ret)
                   (unless (or (eq? ret 'OK) (eq? ret 'UNIMPLEMENTED))
                     (exit exit-status/signal)))
                 (begin
                   (print "EXITED W/ SIGNAL " exit-status/signal)
                   (exit 1)))))))))))

(define ((cond-run runner wop) x)
  (match x
    ((tc-number (op a b) '-> expected)
     (when (eq? op wop)
       (runner x)))))

(load "arithmetic-test.64.expected" (cond-run run-test-case/abs '*))
