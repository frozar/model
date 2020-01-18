(ns factoriel-came-back.core
  (:gen-class))

(defn factoriel
  ([nb] (factoriel 1 nb))
  ([acc nb]
   (if (<= nb 1)
     acc
     (recur (bigint (* acc nb)) (dec nb)))))

(defn -main
  "Compute large factoriel."
  [& args]
  (let [[arg_0 & rest-args] args
        nb-arg-fact (Integer/parseInt arg_0)]
    (println (factoriel nb-arg-fact))))
