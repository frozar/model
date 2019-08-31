(ns Solution
  (:gen-class))

(defn dbg
  [arg0 & args]
  (binding [*out* *err*]
    (println "DBG" arg0 (clojure.string/join " " args))))

(defn pow-of-2
  "Return 2^n"
  ([n]
   (pow-of-2 1 n))
  ([res n]
   (if (<= n 0)
     res
     (recur (* 2 res) (dec n)))))

(defn pow-of-2*
  "Generate a seq of powof 2"
  ([] (pow-of-2* 0))
  ([n] (cons (pow-of-2 n) (lazy-seq (pow-of-2* (inc n))))))

(defn fermat-prime
  "Return a fermat prime number: 2^(2^m) + 1"
  [m] (+ (pow-of-2 (pow-of-2 m)) 1))

(defn fermat-prime*
  "Generate a seq of fermat prime number"
  ([] (fermat-prime* 0))
  ([n] (cons (fermat-prime n) (lazy-seq (fermat-prime* (inc n))))))

(defn int-to-bit
  "Return a int seq of the binary representation of an integer n"
  [n]
  (map #(- (int %) 48) (seq (Integer/toBinaryString n))))

(defn prod-fermat-prime
  "
  Given the binary representation of n, return the product of the associated fermat prime number.
  For example:
  n = 6 (base 10) = 110 (base 2)
  seq_prime_number = 3 5 17 257 ...
  res = (1 * 5) * (1 *  17) = 85
  "
  [n]
  (let [l1 (reverse (int-to-bit n))
        l2 (take (count l1) (fermat-prime*))]
    (reduce * 1 (filter #(not= 0 %) (map * l1 l2)))))

(defn prod-fermat-prime*
  "Generate a seq of product of fermat prime number"
  ([] (prod-fermat-prime* 1))
  ([n] (cons (prod-fermat-prime n) (lazy-seq (prod-fermat-prime* (inc n))))))

(defn buildable-polygones
  "
  Return a seq of int in [a, b] of buildable polygone.
  Every number of this list can be decomposed as: 2^k * (2^(2^m) + 1)
  With k >= 1.
  "
  [a b]
  (filter #(<= a %) 
          (apply concat 
                 (let
                     [range-fermat-prime (take-while #(<= % b) (prod-fermat-prime*))]
                   (map (fn [n]
                          (let 
                              [range-pow-of-2 (take-while #(<= (* n %) b) (pow-of-2*))]
                            (map #(* n %) range-pow-of-2) )
                          )
                        range-fermat-prime)))))

(defn possible-pow-of-2
  "Return a seq of pow of 2 involved in [a, b] segment."
  [a b]
  (let [range-pow-of-2 (take-while #(<= % b) (pow-of-2*))]
    (filter #(<= a %) range-pow-of-2)))

(defn -main [& args]
  (let [a (read) b (read)
        res (count (concat (possible-pow-of-2 a b) (buildable-polygones a b)))]
    (dbg "a" a "b" b)
    (println res)))

