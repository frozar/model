// Compiled by ClojureScript 1.10.339 {}
goog.provide('hello_world.core');
goog.require('cljs.core');
goog.require('goog.dom');
cljs.core.println.call(null,"This text is printed from src/hello_world/core.cljs. Go ahead and edit it and see reloading in action.");
hello_world.core.multiply = (function hello_world$core$multiply(a,b){
return (a * b);
});
if((typeof hello_world !== 'undefined') && (typeof hello_world.core !== 'undefined') && (typeof hello_world.core.app_state !== 'undefined')){
} else {
hello_world.core.app_state = cljs.core.atom.call(null,new cljs.core.PersistentArrayMap(null, 1, [new cljs.core.Keyword(null,"text","text",-1790561697),"Hello world!"], null));
}
hello_world.core.get_app_element = (function hello_world$core$get_app_element(){
return goog.dom.getElement("app");
});
hello_world.core.on_reload = (function hello_world$core$on_reload(){
return null;
});

//# sourceMappingURL=core.js.map
