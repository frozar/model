(ns core
  (:require
   ["firebase/app" :as firebase]
   ["firebase/firestore"]
   [reagent.dom :as rdom]
   ))

(def config
  #js {:apiKey "AIzaSyARzybnP5Ko1ZkNaUxf_CXujIcO1wfpj0w",
       :authDomain "shadow-firebase-f4ad5.firebaseapp.com",
       :databaseURL "https://shadow-firebase-f4ad5.firebaseio.com",
       :projectId "shadow-firebase-f4ad5",
       :storageBucket "shadow-firebase-f4ad5.appspot.com",
       :messagingSenderId "1092080899106",
       :appId "1:1092080899106:web:68302bf51bd9a64687a67b",
       :measurementId "G-P7H2ZEP4JN"
       })

;; This variable is not used in the rest on the code. This is a trick to
;; initialise firebase only once during development session.
(defonce fb-init-dummy
  (firebase/initializeApp config))

(defn fb-write [data-to-save]
  (.. firebase
   (firestore)
   (doc "samples/sandwichData")
   (set data-to-save)
   (then (fn [] (js/console.log "Status saved!")))
   (catch (fn [error] (js/console.log "Got en error: " error)))))

(defn click-handler []
  (let [text (-> "#latestHotDogStatus"
                 js/document.querySelector
                 .-value)
        data-to-save (clj->js {:hotDogStatus text})]
    (fb-write data-to-save)))

(defn basic-form []
  [:<>
   [:h1 {:id "hotDogOutput"} "Hot dog status:"]
   [:input {:type "textfield" :id "latestHotDogStatus"}]
   [:button
    {:id "saveButton"
     :on-click click-handler}
    "Save"]
   [:button {:id "loadButton"} "Load"]]
  )

(defn mount-root [component]
  (rdom/render [component] (.getElementById js/document "app")))

(defn ^:dev/after-load reload! []
  (mount-root basic-form))

(defn init! []
  (reload!))
