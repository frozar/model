(set-env!
 :source-paths #{"src/cljs"}
 :resource-paths #{"html"}

 :dependencies '[[adzerk/boot-cljs "2.1.5"]
                 [pandeiro/boot-http "0.8.3"] ;; add http dependency
                 [adzerk/boot-reload "0.6.0"] ;; add boot-reload
                 [adzerk/boot-cljs-repl "0.4.0"]
                 ;; Dependancies for boot-cljs-repl
                 [cider/piggieback "0.3.9"]
                 [weasel "0.7.0"]
                 [nrepl "0.4.5"]
                 ])

(require '[adzerk.boot-cljs :refer [cljs]]
         '[pandeiro.boot-http :refer [serve]] ;; make serve task visible
         '[adzerk.boot-reload :refer [reload]] ;; make reload visible
         '[adzerk.boot-cljs-repl :refer [cljs-repl start-repl]]) ;; make cljs-repl visible
