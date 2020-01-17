(ns escalier.core
  (:gen-class))

(defn compute-ladder
  "Compute each step of the ladder"
  [nb-step]
  (let [ladder-steps (for [i (range nb-step -1 -1)
                           j (range 1 (inc nb-step))
                           :when (= (+ i j) nb-step)]
                       (apply str (concat (repeat i " ") (repeat j "#")))
                       )]
    (clojure.string/join "\n" ladder-steps)
    ))

(defn -main
  "Print a kind of ladder on stand output.
  For example:
  lein run 3
    #
   ##
  ###"
  [& args]
  (let [[arg_0 & rest-args] args
        nb-step (Integer/parseInt arg_0)]
    (println (compute-ladder nb-step))
    ))
