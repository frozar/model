(ns Solution
  (:gen-class))

(defn dbg
  [arg0 & args]
  (binding [*out* *err*]
    (println "DBG" arg0 (clojure.string/join " " args))))

;; from: https://rosettacode.org/wiki/Sieve_of_Eratosthenes#Clojure
;; "Returns a lazy sequence of prime numbers less than lim"
(def primes-to (memoize (fn
  [lim]
  (let [max-i (int (/ (- lim 1) 2))
        refs (boolean-array max-i true)
        root (/ (dec (int (Math/sqrt lim))) 2)]
    (do (doseq [i (range 1 (inc root))
                :when (aget refs i)]
          (doseq [j (range (* (+ i i) (inc i)) max-i (+ i i 1))]
            (aset refs j false)))
        (cons 2 (map #(+ % % 1) (filter #(aget refs %) (range 1 max-i)))))))))

(def prime-decompose (memoize (fn
  [n l-primes acc]
   (if (or (= 1 n) (empty? l-primes))
     acc
     (let [prime (first l-primes)]
       (if (= 0 (mod n prime))
         (let [update-acc (conj acc prime)
               update-n (quot n prime)]
           (recur update-n l-primes update-acc))
         (recur n (rest l-primes) acc))
       )))))

(defn update-l-primes
  [l-primes max-prime-nb]
  (let [idx (.indexOf l-primes max-prime-nb)]
    (subvec l-primes idx (count l-primes))))

(def factorial-decompose
  (memoize
   (fn
     [decomp l-primes res-dict]
     (if (and (empty? (:positive decomp)) (empty? (:negative decomp)))
       res-dict
       (let [primerise
             (memoize (fn
                        [coll l-primes]
                        (sort > (mapcat #(prime-decompose % l-primes []) coll))))
             delete-idx
             (memoize (fn
                        [vect idx]
                        (vec (concat (subvec vect 0 idx) (subvec vect (inc idx) (count vect))))))
             delete-duplicate
             (memoize (fn
                        [large-coll small-coll res]
                        (if (empty? small-coll)
                          (assoc res :large large-coll)
                          (let [head (first small-coll)
                                tail (rest small-coll)
                                idx (.indexOf large-coll head)
                                ]
                            (if (not= -1 idx)
                              (let [updated-large-coll (delete-idx large-coll idx)]
                                (recur updated-large-coll tail res))
                              (let [updated-small-key (conj (res :small) head)]
                                (recur large-coll tail (assoc res :small updated-small-key))))))))
             simplify-decomp
             (memoize (fn
                        [decomp]
                        (if (<= (count (decomp :negative)) (count (decomp :positive)))
                          (let [large-coll (decomp :positive)
                                small-coll (decomp :negative)
                                res-dict (delete-duplicate large-coll small-coll {:large [] :small []})]
                            {:positive (res-dict :large) :negative (res-dict :small)})
                          (let [large-coll (decomp :negative)
                                small-coll (decomp :positive)
                                res-dict (delete-duplicate large-coll small-coll {:large [] :small []})]
                            {:positive (res-dict :small) :negative (res-dict :large)}))))
             get-max-prime-nb
             (memoize (fn
                        [decomp]
                        (if (and (empty? (decomp :positive)) (empty? (decomp :negative)))
                          0
                          (if (empty? (decomp :positive))
                            (apply max (decomp :negative))
                            (if (empty? (decomp :negative))
                              (apply max (decomp :positive))
                              (max (apply max (:positive decomp)) (apply max (:negative decomp))))))))
             max-prime-nb (get-max-prime-nb decomp)
             updated-l-primes (update-l-primes l-primes max-prime-nb)
             ]
         (if (<= (count (decomp :negative)) (count (decomp :positive)))
           (let [head-factorial (first (:positive decomp))
                 ;; primerised-factorial (primerise (range head-factorial 1 -1) updated-l-primes)
                 ;; wk-decomp (assoc decomp :negative (vec (sort > (concat (decomp :negative) primerised-factorial))))
                 ;; simplified-decomp (simplify-decomp wk-decomp)
                 simplified-decomp (simplify-decomp (assoc decomp :negative (vec (sort > (concat (decomp :negative) (primerise (range head-factorial 1 -1) updated-l-primes))))))
                 update-res-dict (assoc res-dict head-factorial (inc (get res-dict head-factorial 0)))
                 ]
             (recur simplified-decomp updated-l-primes update-res-dict)
             )

           (let [head-factorial (first (:negative decomp))
                 ;; primerised-factorial (primerise (range head-factorial 1 -1) updated-l-primes)
                 ;; wk-decomp (assoc decomp :positive (vec (sort > (concat (decomp :positive) primerised-factorial))))
                 ;; simplified-decomp (simplify-decomp wk-decomp)
                 simplified-decomp (simplify-decomp (assoc decomp :positive (vec (sort > (concat (decomp :positive) (primerise (range head-factorial 1 -1) updated-l-primes))))))
                 update-res-dict (assoc res-dict head-factorial (dec (get res-dict head-factorial 0)))
                 ]
             (recur simplified-decomp updated-l-primes update-res-dict))))))))

(defn print-solution
  [res-dict]
  (println (clojure.string/join " " (map #(apply str (vector % "#" (res-dict %))) (sort > (keys res-dict)))))
  )

(defn -main [& args]
  (let [n (read-line)
        argv (clojure.string/split n #"/")]
    (dbg "input" n)
    (if (= 1 (count argv))
      (let [input-int (Integer/parseInt (first argv))
            l-primes (vec (reverse (primes-to (+ 2 input-int))))
            prime-decomposition (prime-decompose input-int l-primes [])
            res-dict (factorial-decompose {:positive prime-decomposition :negative []} l-primes {})
            ]
        (print-solution res-dict)
        )

      (let [input-numerator (Integer/parseInt (first argv))
            input-denominator (Integer/parseInt (second argv))
            max-nb (max input-numerator input-denominator)
            l-primes (vec (reverse (primes-to (+ 2 max-nb))))
            numerator-prime-decomposition (prime-decompose input-numerator l-primes [])
            denominator-prime-decomposition (prime-decompose input-denominator l-primes [])
            res-dict (factorial-decompose {:positive numerator-prime-decomposition :negative denominator-prime-decomposition} l-primes {})
            ]
        (print-solution res-dict)        
        )
      )))
