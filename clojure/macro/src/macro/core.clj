(ns macro.core
  (:require [clojure.string :as string])
  (:gen-class))

(defn add [a b]
  (+ a b))

(defmacro BANG
  "Use this macro when you pine for the notation of your childhood"
  [func-name]
  (let [
        func-name-banged
        (symbol (str func-name "-42!"))

        arglists
        (-> func-name resolve meta :arglists first)
        ]
    `(defn ~func-name-banged [~@arglists]
       (+ 42 (~func-name ~@arglists)))
    )
  )

;; => Generate the add-42! function
(BANG add)

(defn -main
  "Capitalise 1 out of 2 letter."
  [& args]
  (println "Hello:" (add-42! 1 10)))
