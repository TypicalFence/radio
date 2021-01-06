#lang racket
(require uuid)
(require json)

(define make-jsonrpc-request 
        (lambda (id method params) 
          (jsexpr->string (hash 'id id 'method method 'params params 'jsonrpc "2.0"))))
          
(define make-request 
        (lambda (method params) 
          (make-jsonrpc-request (uuid-string) method params)))

;(define playRequest (make-request "switch_station" (hash 'id "plazaone")))
(define playRequest (make-request "switch_station" (hash 'id "cyberia")))
(define stopRequest (make-request "stop" 'null))
(define songRequest (make-request "current_song" 'null))
(define stateRequest (make-request "player_state" 'null))

(define-values (in out) (tcp-connect "localhost" 8069))
(display stateRequest out)
(flush-output out)
(close-output-port out)
(read-json in)
