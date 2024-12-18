(ns reitit-sample.core ; example.server
  (:require [reitit.ring :as ring]
            [reitit.coercion.spec]
            [reitit.swagger :as swagger]
            [reitit.swagger-ui :as swagger-ui]
            [reitit.ring.coercion :as coercion]
            [reitit.dev.pretty :as pretty]
            [reitit.ring.middleware.muuntaja :as muuntaja]
            [reitit.ring.middleware.exception :as exception]
            [reitit.ring.middleware.multipart :as multipart]
            [reitit.ring.middleware.parameters :as parameters]
            [reitit.ring.middleware.dev :as dev]
            [reitit.ring.spec :as spec]
            [spec-tools.spell :as spell]
            [ring.adapter.jetty :as jetty]
            [muuntaja.core :as m]
            [clojure.java.io :as io]
            [fipp.edn :refer [pprint] :rename {pprint fipp}]))

(defn fibo [n]
  (if (<= n 1)
    1
    (+ (fibo (dec n)) (fibo (dec (dec n))))))

(def app
  (ring/ring-handler
   (ring/router
    [["/swagger.json"
      {:get {:no-doc true
             :swagger {:info {:title "Simple fibonacci application"
                              :description "with reitit-ring"}}
             :handler (swagger/create-swagger-handler)}}]

     ["/fibo"
      {:swagger {:tags ["fibo"]}
       :get {:summary "plus with spec query parameters"
             :parameters {:query {:n int?}}
             :responses {200 {:body {:total int?}}}
             :handler (fn [{{{:keys [n]} :query} :parameters}]
                        {:status 200
                         :body {:total (fibo n)}})}
       :post {:summary "plus with spec body parameters"
              :parameters {:body {:n int?}}
              :responses {200 {:body {:total int?}}}
              :handler (fn [{{{:keys [n]} :body} :parameters}]
                         {:status 200
                          :body {:total (fibo n)}})}}]]

    {;;:reitit.middleware/transform dev/print-request-diffs ;; pretty diffs
     ;;:validate spec/validate ;; enable spec validation for route data
     ;;:reitit.spec/wrap spell/closed ;; strict top-level validation
     :exception pretty/exception
     :data {:coercion reitit.coercion.spec/coercion
            :muuntaja m/instance
            :middleware [;; swagger feature
                         swagger/swagger-feature
                         ;; query-params & form-params
                         parameters/parameters-middleware
                         ;; content-negotiation
                         muuntaja/format-negotiate-middleware
                         ;; encoding response body
                         muuntaja/format-response-middleware
                         ;; exception handling
                         exception/exception-middleware
                         ;; decoding request body
                         muuntaja/format-request-middleware
                         ;; coercing response bodys
                         coercion/coerce-response-middleware
                         ;; coercing request parameters
                         coercion/coerce-request-middleware
                         ;; multipart
                         multipart/multipart-middleware]}})
   (ring/routes
    (swagger-ui/create-swagger-ui-handler
     {:path "/"
      :config {:validatorUrl nil
               :operationsSorter "alpha"}})
    (ring/create-default-handler))))

(defn start []
  (jetty/run-jetty #'app {:port 3000, :join? false})
  (println "server running in port 3000"))

(comment
  (start))
