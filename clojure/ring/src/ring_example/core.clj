(ns ring-example.core
  (:require [ring.util.response :as response]
            [ring.middleware.params :as params])
  )

(defn fibo [n]
  (if (<= n 1)
    1
    (+ (fibo (dec n)) (fibo (dec (dec n))))))

(defn fibo-body [n]
  (str "Fibo( " n " ): " (fibo n)))

(defn handler [request]
  (let [n ((:query-params request) "n")]
    (if n
      (response/response (fibo-body (Integer. n)))
      (response/response "n parameter is missing.")
      ))
  )

(def app
  (params/wrap-params handler))
