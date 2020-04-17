(ns permut.core
  (:gen-class))

(defn except-nth [coll idx]
  (let [v (vec coll)]
    (if (< idx (count v))
      (vec (concat (subvec v 0 idx) (subvec v (inc idx))))
      v)))

(except-nth (range 3) 1)
;; => [0 2]

(defn permutation [v]
  (if (<= (count v) 1)
    (list v)

    (apply
     concat
     (for [idx (range (count v))]
       (map
        (fn [sub-permut] (vec (cons (get v idx) sub-permut)))
        (permutation (except-nth v idx))
        )))))

(permutation [42])
;; => ([42])

(permutation (vec (range 3)))
;; => ([0 1 2] [0 2 1] [1 0 2] [1 2 0] [2 0 1] [2 1 0])

(permutation (vec "ab"))
;; => ([\a \b] [\b \a])

(permutation ["foo" "bar" 42])
;; => (["foo" "bar" 42] ["foo" 42 "bar"] ["bar" "foo" 42] ["bar" 42 "foo"] [42 "foo" "bar"] [42 "bar" "foo"])

(defn -main
  []
  (prn (permutation ["foo" "bar" 42])))
