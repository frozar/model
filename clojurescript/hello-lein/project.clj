(defproject hello-seymore "0.1.0-SNAPSHOT"
  :dependencies [[org.clojure/clojure "1.8.0"]
                 [org.clojure/clojurescript "1.9.908"]
                 [cljsjs/react "15.2.1-1"]
                 [cljsjs/react-dom "15.2.1-1"]
                 [sablono "0.7.4"]
                 ]
  :plugins [[lein-figwheel "0.5.13"]]
  :clean-targets ^{:protect false} [:target-path "out" "resources/public/cljs"]
  :cljsbuild {
    :builds [{:id "dev"
              :source-paths ["src"]
              :figwheel true
              :compiler {:main "hello-seymore.core"
                         :asset-path "cljs/out"
                         :output-to  "resources/public/cljs/main.js"
                         :output-dir "resources/public/cljs/out"}
             }]
   }
   :figwheel {
     :css-dirs ["resources/public/css"]
   })
