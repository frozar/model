(ns Solution
  (:gen-class))

(defn dbg
  [arg0 & args]
  (binding [*out* *err*]
    (println "DBG" arg0 (clojure.string/join " " args))))

(defn read-input
  [i acc]
  (if (= i 0)
    acc
    (let [line (read-line)]
      (recur (dec i) (conj acc (vec (map #(- (int %) 48) line)))))))

(defn get-elt
  [puzzle i j]
  (nth (nth puzzle i) j))

(defn set-elt
  [puzzle i j elt]
  (let [new-vec (assoc (nth puzzle i) j elt)]
    (assoc puzzle i new-vec)))

(defn get-empty-square
  [puzzle]
  (first (filter identity (for [i (range 0 3), j (range 0 3)] (if (= -2 (get-elt puzzle i j)) [i j])))))

(def knight-move
  (vector [-2 -1] [-2 1] [-1 -2] [-1 2] [1 -2] [1 2] [2 -1] [2 1]))

(def solved-puzzle
  (vector [1 2 3] [4 5 6] [7 8 -2]))

(defn in?
  "true if coll contains elt"
  [coll elt]
  (some #(= elt %) coll))

(defn diff
  [initial-pos dest-pos]
  [(- (first initial-pos) (first dest-pos)) (- (second initial-pos) (second dest-pos))])

(defn get-knight-complaint-pos
  [dest-pos]
  (vec (filter
        (fn [pos] (in? knight-move (diff pos dest-pos)))
        (for [i (range 0 3), j (range 0 3)] [i j]))))

(defn swap
  [puzzle pos0 pos1]
  (let [i0 (first pos0)
        j0 (second pos0)
        i1 (first pos1)
        j1 (second pos1)
        elt0 (get-elt puzzle i0 j0)
        elt1 (get-elt puzzle i1 j1)]
    (set-elt (set-elt puzzle i0 j0 elt1) i1 j1 elt0)))

(defn get-next-puzzles
  [puzzle res]
  (let [pos-empty-square (get-empty-square puzzle)
        next-moves (get-knight-complaint-pos pos-empty-square)
        next-puzzles (vec (map #(swap puzzle pos-empty-square %) next-moves))
        ]
    (vec (filter identity (map #(if (not (get res %)) %) next-puzzles)))))

(defn run
  [next-puzzles depth res]
  (cond 
    (empty? next-puzzles) -1
    (reduce #(or %1 %2) false (map #(= solved-puzzle %) next-puzzles)) depth
    :else
    (let [res (reduce #(assoc %1 %2 depth) res next-puzzles)
          next-puzzles (mapcat #(get-next-puzzles % res) next-puzzles)]
      (recur next-puzzles (inc depth) res))))

(defn -main [& args]
  (def puzzle (read-input 3 []))
  (dbg "input" puzzle)
  (def depth 0)
  (def res {})
  (def next-puzzles (vector puzzle))

  (def min-move (run next-puzzles depth res))
  (println min-move))
