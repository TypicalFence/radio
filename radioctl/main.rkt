#lang racket
(require racket/random
         json
         (only-in file/sha1
                  bytes->hex-string))

; not really an actual uuid but unique enough for now
(define uuid 
  (lambda () 
    (bytes->hex-string 
      (crypto-random-bytes 16))))

(define make-jsonrpc-request 
        (lambda (id method params) 
          (jsexpr->string (hash 'id id 'method method 'params params 'jsonrpc "2.0"))))
          
(define make-request 
        (lambda (method params) 
          (make-jsonrpc-request (uuid) method params)))

;(define playRequest (make-request "switch_station" (hash 'id "plazaone")))
(define playRequest (make-request "switch_station" (hash 'id "cyberia")))
(define stopRequest (make-request "stop" 'null))
(define songRequest (make-request "current_song" 'null))
(define stateRequest (make-request "player_state" 'null))


(define rpc-call 
  (lambda (request) 
    (define-values (in out) (tcp-connect "localhost" 8069))
    (display request out)
    (flush-output out)
    (close-output-port out)
    (read-json in)))

; check cmd args
(when (not (> (vector-length (current-command-line-arguments)) 0))
      (print "specify command")
      (exit))

(define cmd (vector-ref (current-command-line-arguments) 0))

(match cmd 
    ["play" (rpc-call playRequest)]
    ["stop" (rpc-call stopRequest)]
    ["song" (rpc-call songRequest)]
    ["state" (rpc-call stateRequest)]
    [_ (println "command not found")])

