(ns Solution
  (:gen-class))

(defn dbg
  [arg0 & args]
  (binding [*out* *err*]
    (println "DBG" arg0 (clojure.string/join " " args))))

(defn read-input
  []
  (read-line))

;; (defn rewrite
;;   "Recursively rewrites the infix-expr as a prefix expression, according to
;;    the operator precedence rules"
;;   [infix-expr]
;;   (cond
;;     (not (seq? infix-expr))
;;     (resolve-alias infix-expr)

;;     (and (seq? (first infix-expr)) (= (count infix-expr) 1))
;;     (rewrite (first infix-expr))

;;     (empty? (rest infix-expr))
;;     (first infix-expr)

;;     :else
;;     (let [infix-expr (map resolve-alias infix-expr)]
;;       (loop [ops operator-precedence]
;;         (if-let [op (first ops)]
;;           (let [idx (.lastIndexOf ^java.util.List infix-expr op)]
;;             (if (pos? idx)
;;               (let [[expr1 [op & expr2]] (split-at idx infix-expr)]
;;                 (list op (rewrite expr1) (rewrite expr2)))
;;               (recur (next ops))))

;;           (if (empty-arglist? infix-expr)
;;             (list (rewrite (first infix-expr)))
;;             (list (rewrite (first infix-expr)) (rewrite (next infix-expr)))))))))

(def operators-priority
  {\( 3 \) 3 \* 2 \+ 1 \- 1 \> 0})

(def operators
  (into #{} (keys operators-priority)))

(defn operator?
  [ch]
  (contains? operators ch))

(defn get-idx-ops
  [expr]
  (let [tmp-expr (map operator? (seq expr))]
    (filter identity (map-indexed (fn [idx v] (if v idx)) tmp-expr))))

(defn split-expr-by-ops
  ([expr]
   (let [idx-ops (get-idx-ops expr)
         idx-split (reverse (interleave idx-ops (map inc idx-ops)))]
     (split-expr-by-ops expr idx-split [])))
  ([expr idx-split acc]
   (cond
     (empty? idx-split)
     (filter (fn [s] (not (empty? s))) (map (fn [coll] (apply str coll)) (reverse (conj acc expr))))

     :else
     (let [new-expr (first (split-at (first idx-split) expr))
           segment (last (split-at (first idx-split) expr))]
       (recur new-expr (rest idx-split) (conj acc segment))))))

(defn split-expr-by-parentheses
  ([expr] (split-expr-by-parentheses (split-expr-by-ops expr) []))
  ([expr acc]
   (dbg "expr" (seq expr))
   expr))

(defn -main [& args]
  (let [expr (read-input)]
    (dbg "expr" expr)
    ;; (def splitted-expr (split-expr expr))
    ;; (dbg "splitted-expr" (seq splitted-expr))

    (dbg "split-expr-by-parentheses" (split-expr-by-parentheses "2+(2*3)-6"))
    
    ; Write answer to stdout
    (println "answer")))
