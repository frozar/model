(ns Solution
  (:gen-class))

(defn dbg
  [arg0 & args]
  (binding [*out* *err*]
    (println "DBG" arg0 (clojure.string/join " " args))))

(defn get-positive-house
  [houses]
  (filter #(< 0 (get houses %)) (range (count houses))))

(defn assemble
  [acc e1]
  (vec (set (mapcat (fn [e1] (mapcat (fn [e0] (map #(concat % e0) e1)) acc)) e1))))

(defn abs "(abs n) is the absolute value of n" [n]
  (cond
   (not (number? n)) (throw (IllegalArgumentException.
                             "abs requires a number"))
   (neg? n) (- n)
   :else n))

(defn legal?
  [elt]
  (if (<= (count elt) 1)
    true
    (let [beg-elt (take (dec (count elt)) elt)
          end-elt (drop 1 elt)
          diff-elt (map #(abs (- (get % 1) (get % 0) )) (map #(vector %1 %2) beg-elt end-elt))]
      (if (some #(= % 1) diff-elt)
        false
        true))))

(defn scored-item
  [get-score item]
  (vector (get-score item) item))

(defn get-sorted-list
  [get-score acc]
  (into () (for [[k v] (sort #(< (first %1) (first %2)) (map #(scored-item get-score %) acc))] v)))

(defn generate-legal-idx-set
  ([arg get-score]
   (generate-legal-idx-set (first arg) (rest arg) get-score))
  ([acc arg get-score]
   (if (empty? arg)
     acc
     (let [acc0 (assemble acc (take 1 arg))
           acc1 (filter legal? acc0)
           acc2 (take 2 (get-sorted-list get-score acc1))]
       (recur acc2 (rest arg) get-score)))
   )
  )

(defn -main [& args]
  (let [
        N (read)
        houses (vec (repeatedly N read))
        positive-idx (get-positive-house houses)
        init-list (map (fn [n] (conj [] [] (vector n))) positive-idx)
        get-score (partial (fn [houses v0] (reduce + (map (fn [idx] (get houses idx)) v0))) houses)
        quick-score (memoize get-score)
        legal-idx-set (generate-legal-idx-set init-list quick-score)
        ]
    (if (empty? legal-idx-set)
      (println 0)
      (let [result (apply max (map (fn [v0] (get-score v0)) legal-idx-set))]
        (println result)))
    )
  )

;; Solution from dbdr
;; (ns Solution (:gen-class))
;; (defn -main []
;;   (let [N (read) houses (repeatedly N read)]
;;     (def steal (memoize (fn [houses]
;;         (if (empty? houses) 0
;;             (max (+ (first houses) (steal (drop 2 houses)))
;;                  (steal (rest houses)))))))
;;     (println (steal houses))))

