(ns datomic-tuto.core-test
  (:require [clojure.test :refer :all]
            [datomic-tuto.core :refer :all]
            [datomic.api :as d]
            ))

(defn create-empty-in-memory-db []
  (let [uri "datomic:mem://pet-owners-test-db"]
    (d/delete-database uri)
    (d/create-database uri)
    (let [conn (d/connect uri)
          schema (load-file "resources/datomic/schema.edn")]
      (d/transact conn schema)
      conn)))

(deftest add-one-owner
  (testing "Adding one owner should allow us to find that onwer"
    (is (=
         #{["John"]}
         (with-redefs [conn (create-empty-in-memory-db)]
           (do
             (add-pet-owner "John")
             (find-all-pet-owners)))))))

(deftest add-multiple-owner
  (testing "Adding one owner should allow us to find all those onwers"
    (is (=
         #{["John"] ["Paul"] ["George"]}
         (with-redefs [conn (create-empty-in-memory-db)]
           (do
             (add-pet-owner "John")
             (add-pet-owner "Paul")
             (add-pet-owner "George")
             (find-all-pet-owners)))))))

(deftest add-one-owner-n-one-pet
  (testing "Adding one owner with one pet should allow us to find that pet for that owner"
    (is (=
         #{["Salt"]}
         (with-redefs [conn (create-empty-in-memory-db)]
           (do
             (add-pet-owner "John")
             (add-pet "Salt" "John")
             (find-pets-for-owner "John")))))))

(deftest add-multiple-owner-n-pet
  (testing "Adding multiple owners and pets should allow us to find the pets for a particular owner"
    (is (=
         #{["Martha"] ["Jet"]}
         (with-redefs [conn (create-empty-in-memory-db)]
           (do
             (add-pet-owner "John")
             (add-pet "Salt" "John")
             (add-pet "Pepper" "John")
             (add-pet-owner "Paul")
             (add-pet "Martha" "Paul")
             (add-pet "Jet" "Paul")
             (find-pets-for-owner "Paul")))))))
