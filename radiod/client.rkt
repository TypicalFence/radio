#lang racket
(require uuid)
(require json)

(define make-jsonrpc-request 
        (lambda (id method params) 
          (jsexpr->string (hash 'id id 'method method 'params params 'jsonrpc "2.0"))))
          
(define make-request 
        (lambda (method params) 
          (make-jsonrpc-request (uuid-string) method params)))

(define playRequest (make-request "switch_station" (hash 'id "plazaone")))

(define-values (in out) (tcp-connect "localhost" 8069))
(display playRequest out)
(flush-output out)
