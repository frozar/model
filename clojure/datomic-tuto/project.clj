(defproject datomic-tuto "0.1.0-SNAPSHOT"
  :description "Example of utilisation of Datomic"
  :url "https://github.com/frozar/model/clojure/datomic-tuto"
  :license {:name "EPL-2.0 OR GPL-2.0-or-later WITH Classpath-exception-2.0"
            :url "https://www.eclipse.org/legal/epl-2.0/"}
  :repositories {"my.datomic.com" {:url "https://my.datomic.com/repo"
                                   :creds :gpg}}
  :dependencies [[org.clojure/clojure "1.10.0"]
                 [com.datomic/client-pro "0.9.41"]
                 ;; [com.datomic/datomic-free "0.9.5697"]
                 [com.datomic/datomic-pro "0.9.6024"]
                 [fipp "0.6.22"]]
  :main ^:skip-aot datomic-tuto.core
  :target-path "target/%s"
  :profiles {:uberjar {:aot :all}})
