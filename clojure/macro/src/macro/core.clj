(ns macro.core
  (:require [clojure.string :as string]
            [clojure.repl])
  (:gen-class))

;; (defn add [a b]
;;   (+ a b))

;; (defmacro BANG
;;   "Use this macro when you pine for the notation of your childhood"
;;   [func-name]
;;   (let [
;;         func-name-banged
;;         (symbol (str func-name "-42!"))
;;         arglists
;;         (-> func-name resolve meta :arglists first)
;;         ]
;;     `(defn ~func-name-banged [~@arglists]
;;        (+ 42 (~func-name ~@arglists)))
;;     )
;;   )

;; (ns bubble.state
;;   (:require [bubble.bubble :as bubble]
;;             [reagent.core :as reagent]))

(defn initial-application-state []
  {
   :bubbles [{:id "root"}]
   :links []
   })

(defonce appstate
  (atom (initial-application-state)))

(defmacro BANG
  "Define the side-effect version of a given function 'func-name'"
  [func-name]
  (let [func-name-banged (symbol (str func-name "!"))]
    `(defn ~func-name-banged [~'arg]
       (swap! appstate (fn [~'appstate_arg] (~func-name ~'appstate_arg ~'arg))))))

(macroexpand-1 '(BANG add-bubble))
;; => (clojure.core/defn add-bubble! [arg] (clojure.core/swap! macro.core/appstate (clojure.core/fn [appstate_arg] (add-bubble appstate_arg arg))))
(macroexpand '(BANG add-bubble))
;; => (def add-bubble! (clojure.core/fn ([arg] (clojure.core/swap! macro.core/appstate (clojure.core/fn [appstate_arg] (add-bubble appstate_arg arg))))))

(defn- add-bubble [appstate bubble]
  (update appstate :bubbles conj bubble))

(add-bubble @appstate {:id "toto"})

(defn add-bubble! [bubble]
  (swap! appstate (fn [appstate_arg] (add-bubble appstate_arg bubble))))


;; => Generate the add-42! function
;; (BANG add)

(meta (var string/split))
(-> (ns-publics 'clojure.string)
     keys)
(type string/ends-with?)
(var string/ends-with?)
(symbol string/ends-with?)

(meta string/ends-with?)
(meta 'string/ends-with?)
(meta (var string/ends-with?))

(clojure.repl/doc string/split)

(defn -main
  "Capitalise 1 out of 2 letter."
  [& args]
  (println "Hello:" (add-42! 1 10)))
