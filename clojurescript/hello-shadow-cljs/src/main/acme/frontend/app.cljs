(ns acme.frontend.app
  (:require [reagent.core :as reagent]
            ;; just an example of js module import
            ;; no configuration in project.cljs is required to use a js package
            ;; but it must be available in the node_module directory, so
            ;; previously installed through npm command
            ;; ["md5.js" :as md5]
            ;; ["react-rough" :as reactrough]
            ["react-rough" :default reactrough]
            ["react-rough" :refer [Rectangle]]
            ))

;; (defn md5-hash [text]
;;   (-> (new md5)
;;       (.update text)
;;       (.digest "hex")))

(defn toy-fn []
  ;; (println (md5-hash "toto"))
  ;; (prn 2)
  )

(defn current-page []
  [:<>
   [:h1 "Roughjs example"]
   [:> reactrough
    [:> Rectangle {:x 15 :y 15 :width 90 :height 80 :fill "red"}]
    ]
   ]
  )

(defn mount-page [component]
  (reagent/render [component] (.getElementById js/document "root")))

(defn basic-page []
  (toy-fn)
  (mount-page current-page))

;; This metadata for the "reload!" function is there to manage the behavior of
;; the application during development. When a source file is modified, the
;; project is automatically recompiled and the page updated. Thanks to the
;; :dev/after-load metadata, one can manage the path of execution in this case.
;; documentation:
;; https://shadow-cljs.github.io/docs/UsersGuide.html#_lifecycle_hooks
(defn ^:dev/after-load reload! []
  (basic-page)
  )

(defn init! []
  (reload!)
  )
