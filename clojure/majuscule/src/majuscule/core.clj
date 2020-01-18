(ns majuscule.core
  (:gen-class))

(defn captalise-1-out-of-2
  [string]
  (->> string
       (map-indexed
        (fn [i letter]
          (if (odd? i)
            (-> letter str clojure.string/upper-case)
            (-> letter str clojure.string/lower-case)))
        )
       (apply str)))

(defn -main
  "Capitalise 1 out of 2 letter."
  [& args]
  (let [[arg_0 & rest-args] args]
    (println (captalise-1-out-of-2 arg_0))))
