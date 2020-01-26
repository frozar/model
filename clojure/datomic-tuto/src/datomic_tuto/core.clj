(ns datomic-tuto.core
  (:require
   ;; [datomic.client.api :as d]
   [datomic.api :as d]))

;; ----- Databse connection -----

;; (def conn nil)

(def db-uri "datomic:dev://localhost:4334/pet-owners-db")
(d/delete-database db-uri)
(d/create-database db-uri)
(def conn (d/connect db-uri))
(def schema (load-file "resources/datomic/schema.edn"))
(d/transact conn schema)

;; configuration for the datomic client api [datomic.client.api :as d]
;; (def cfg {:server-type :peer-server
;;           :access-key "myaccesskey"
;;           :secret "mysecret"
;;           :endpoint "localhost:8998"
;;           :validate-hostnames false})
;; (def client (d/client cfg))
;; (d/delete-database client {:db-name "pet-owners-db"})
;; (d/create-database client {:db-name "pet-owners-db"})
;; (def conn (d/connect client {:db-name "pet-owners-db"}))

;; ----- Helper functions -----

(defn find-pet-owner-id [owner-name]
  (ffirst (d/q '[:find ?eid
                 :in $ ?owner-name
                 :where [?eid :owner/name ?owner-name]]
               (d/db conn)
               owner-name)))

;; ----- Query functions -----

(defn add-pet-owner [owner-name]
  (d/transact conn [{:owner/name owner-name}])
  )

(defn find-all-pet-owners []
  (d/q '[:find ?owner-name
         :where [_ :owner/name ?owner-name]]
       (d/db conn)))

(defn add-pet
  [pet-name owner-name]
  (d/transact conn [{:db/id "temp-id-new-pet"
                     :pet/name pet-name}
                    {:db/id (find-pet-owner-id owner-name)
                     :owner/pets "temp-id-new-pet"}
                    ]))

(defn find-pets-for-owner [owner-name]
  (d/q '[:find ?pet-name
         :in $ ?owner-name
         :where
         [?eid :owner/name ?owner-name]
         [?eid :owner/pets ?pet]
         [?pet :pet/name ?pet-name]]
       (d/db conn)
       owner-name))
