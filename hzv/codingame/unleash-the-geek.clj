(ns Player
  (:gen-class))

;; (def DBG true)
(def DBG false)

(defn dbg
  ([arg0]
   (if DBG
     (binding [*out* *err*]
       (println "DBG" arg0))))
  ([arg0 arg1]
   (if DBG
     (binding [*out* *err*]
       (print "DBG" arg0 " ")
       (println arg1)))))

(defn in?
  [coll item]
  (some #(= item %) coll))

(defn read-ores-holes
  [width height]
  (loop [i 0 j 0 ores {} holes {}]
    (if (< i height)
      (if (< j width)
        (let [ore (read) hole (read)
              ore-val (if (= ore '?) -1 ore)]
          (recur i (inc j) (assoc-in ores [i j] ore-val) (assoc-in holes [i j] hole)))
        (recur (inc i) 0 ores holes))
      (do
        [(mapv (fn [[i couples]] (mapv second (sort-by first couples))) (sort-by first ores))
         (mapv (fn [[i couples]] (mapv second (sort-by first couples))) (sort-by first holes))]
        ))))

;; type: 0 for your robot, 1 for other robot, 2 for radar, 3 for trap
(def TYPE_ROBOT 0)
(def TYPE_OTHER 1)
(def TYPE_RADAR 2)
(def TYPE_TRAP  3)
;; item: if this entity is a robot, the item it is carrying
;; (-1 for NONE, 2 for RADAR, 3 for TRAP, 4 for ORE)
(def NONE -1)
(def RADAR 2)
(def TRAP  3)
(def ORE   4)

(def NB_ROBOT 5)

(defn read-entities
  [entityCount]
  (loop [i 0 entities []]
    (if (< i entityCount)
      (let [id (read) type (read) x (read) y (read) item (read)
            entity {:id id :type type :x x :y y :item item}]
        (recur (inc i) (conj entities entity)))
      entities
      )))

(defn read-input
  [width height]
  (let [myScore (read)
        opponentScore (read)
        [ores holes] (read-ores-holes width height)
        entityCount (read)
        radarCooldown (read)
        trapCooldown (read)
        entities (read-entities entityCount)
        game {:myScore myScore
              :opponentScore opponentScore
              :ores ores
              :holes holes
              :entityCount entityCount
              :radarCooldown radarCooldown
              :trapCooldown trapCooldown
              :entities entities
              }]
    game
    ))

(defn print-grid
  [v]
  (binding [*out* *err*]
    (loop [v v]
      (if (not (empty? v))
        (do
          (let [line (mapv #(if (= % -1) "X" %) (first v))]
            (println line))
          (recur (rest v)))))))

(defn print-entities
  [v]
  (binding [*out* *err*]
    (loop [v v]
      (if (not (empty? v))
        (do
          (println (first v))
          (recur (rest v)))))))

(defn print-game
  ([game]
   (print-game game false))
  ([game full]
   (binding [*out* *err*]
     (println "myScore" (:myScore game))
     (println "opponentScore" (:opponentScore game))
     (println "entityCount" (:entityCount game))
     (println "radarCooldown" (:radarCooldown game))
     (println "trapCooldown" (:trapCooldown game))
     (println "entities:")
     (print-entities (:entities game))
     
     (if full
       (do
         (println "ores:")
         (print-grid (:ores game))
         (println "holes:")
         (print-grid (:holes game)))))))

(defn get-x-positions
  [ores width height pred]
  (filterv identity
          (for [y (range height) x (range width)]
            (if (pred (nth (nth ores y) x))
              [x y]))))

(defn get-ore-positions
  [ores width height]
  (get-x-positions ores width height #(< 0 %)))

;; (defn get-ore-positions
;;   [ores width height]
;;   (filterv identity
;;           (for [y (range height) x (range width)]
;;             (if (#(< 0 %) (nth (nth ores y) x))
;;               [x y]))))

;; (defn get-unknown-positions
;;   [ores width height]
;;   (get-x-positions ores width height #(= -1 %)))

(defn get-robots
  [game]
  (->> (:entities game)
       (filter #(= (:type %) TYPE_ROBOT))))

(defn get-robot-radar
  [game]
  (->> (get-robots game)
       (filter #(= (:item %) RADAR))
       (first)))

(defn get-robots-ore
  [game]
  (->> (get-robots game)
       (filter #(= (:item %) ORE))))

(defn get-robot-id
  [robot]
  (mod (:id robot) NB_ROBOT))

(defn get-robots-available
  [game actions]
  (def res (->> (get-robots game)
       (filterv #(= (:item %) NONE))
       (filterv #(= (actions (get-robot-id %)) "WAIT"))))
  ;; (dbg "res" res)
  res)

(defn get-robot-nearest-middle-headquarter
  [game]
  (let [wk-robots (->> (:entities game)
                    (filter #(= (:type %) TYPE_ROBOT))
                    (sort-by :x))
        min-x (:x (first wk-robots))
        robots (filter #(= (:x %) min-x) wk-robots)
        ]
    (if (= 1 (count robots))
      (first robots)
      (let [wk-robots (->> robots
                           (sort-by #(Math/abs (- 7 (:y %)))))]
        (first wk-robots)))))

(defn dist 
  [robot cell]
  (+ (Math/abs (- (:x robot) (first cell)))
     (Math/abs (- (:y robot) (second cell)))))

(defn get-nearest-cell
  [robot cells]
  (->> cells
       (sort-by #(dist robot %) )
       (first)))

(defn get-robot-pos
  [robot]
  [(:x robot) (:y robot)]
  )

(defn get-actions
  [game width height]
    ;; (dbg "ore-positions" ore-positions)
    ;; (dbg "get-unknown-positions" (get-unknown-positions (:ores game) width height))

  (def actions (into {} (for [i (range 5)] [i "WAIT"])))
  (dbg "-1 INIT" actions)

  (let [robots (get-robots-ore game)]
    (if (not (empty? robots))
      (def actions
              (reduce
               (fn [coll r] (assoc coll (get-robot-id r) (str "MOVE 0 " (:y r))))
               actions
               robots
               )
              )
      ))
  (dbg "0 has ORE" actions)

  (let [ore-positions (get-ore-positions (:ores game) width height)]
    (dbg "ore-positions" ore-positions)
    (if (empty? ore-positions)
      (let [robot-radar (get-robot-radar game)]
        (dbg "robot-radar" robot-radar)
        (if robot-radar
          (if (<= (dist robot-radar [6 7]) 1)
            (assoc actions (get-robot-id robot-radar) "DIG 6 7")
            (assoc actions (get-robot-id robot-radar) "MOVE 6 7"))
          (let [robot-nearest-middle-headquarter (get-robot-nearest-middle-headquarter game)
                id-robot (get-robot-id robot-nearest-middle-headquarter)]
            ;; (dbg "robot-nearest-middle-headquarter" robot-nearest-middle-headquarter)
            ;; (def actions (assoc actions id-robot "REQUEST RADAR"))
            (assoc actions id-robot "REQUEST RADAR")
            ;; (assoc res id-robot "REQUEST RADAR")
            )
          ))

      (do
        (dbg "ELSE")
        (let []
          (def targets-move
            (->> (get-robots-available game actions)
                 (map (fn [r] [r (get-nearest-cell r ore-positions)]))))
          (dbg "targets-move" targets-move)
          (def ready-to-dig (filter (fn [[r pos]] (<= (dist r pos) 1)) targets-move))
          (dbg "ready-to-dig" ready-to-dig)
          (if (not (empty? ready-to-dig))
            (def actions
                    (reduce
                     (fn [col [r pos]]
                       (assoc col (get-robot-id r) (str "DIG " (first pos) " " (second pos))))
                     actions
                     ready-to-dig
                     )
                    )
            )
          (dbg "1 actions" actions)

          (dbg "(get-robots-available)" (get-robots-available game actions))
          (def targets-move
            (->> targets-move
                 (filter (fn [[r target]]
                           (in? (get-robots-available game actions) r)))
                 ))
          (dbg "targets-move" targets-move)
          (def actions
                  (reduce
                   (fn [col [r pos]]
                     (assoc col (get-robot-id r) (str "MOVE " (first pos) " " (second pos))))
                   actions
                   targets-move
                   )
                  )
          (dbg "2 actions" actions)
          
          actions
          )
        )
      )))
        
      
(defn print-actions
  [actions]
  (let [actions_str (->> actions
                    (sort-by first)
                    (map second)
                    (clojure.string/join "\n"))]
    (println actions_str)))


;; Deliver more ore to hq (left side of the map) than your opponent. Use radars to find ore but beware of traps!

(defn -main [& args]
  (let [width (read) height (read)]
    (while true
      (let [game (read-input width height)]
        ;; (def actions (get-actions game width height))
        ;; (dbg "actions" actions)

        (def actions (into {} (for [i (range 5)] [i "WAIT"])))
        (print-actions actions)

        ))))
