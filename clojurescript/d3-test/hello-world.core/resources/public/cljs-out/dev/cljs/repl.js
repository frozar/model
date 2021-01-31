// Compiled by ClojureScript 1.10.339 {}
goog.provide('cljs.repl');
goog.require('cljs.core');
goog.require('cljs.spec.alpha');
cljs.repl.print_doc = (function cljs$repl$print_doc(p__18483){
var map__18484 = p__18483;
var map__18484__$1 = ((((!((map__18484 == null)))?(((((map__18484.cljs$lang$protocol_mask$partition0$ & (64))) || ((cljs.core.PROTOCOL_SENTINEL === map__18484.cljs$core$ISeq$))))?true:false):false))?cljs.core.apply.call(null,cljs.core.hash_map,map__18484):map__18484);
var m = map__18484__$1;
var n = cljs.core.get.call(null,map__18484__$1,new cljs.core.Keyword(null,"ns","ns",441598760));
var nm = cljs.core.get.call(null,map__18484__$1,new cljs.core.Keyword(null,"name","name",1843675177));
cljs.core.println.call(null,"-------------------------");

cljs.core.println.call(null,[cljs.core.str.cljs$core$IFn$_invoke$arity$1((function (){var temp__5457__auto__ = new cljs.core.Keyword(null,"ns","ns",441598760).cljs$core$IFn$_invoke$arity$1(m);
if(cljs.core.truth_(temp__5457__auto__)){
var ns = temp__5457__auto__;
return [cljs.core.str.cljs$core$IFn$_invoke$arity$1(ns),"/"].join('');
} else {
return null;
}
})()),cljs.core.str.cljs$core$IFn$_invoke$arity$1(new cljs.core.Keyword(null,"name","name",1843675177).cljs$core$IFn$_invoke$arity$1(m))].join(''));

if(cljs.core.truth_(new cljs.core.Keyword(null,"protocol","protocol",652470118).cljs$core$IFn$_invoke$arity$1(m))){
cljs.core.println.call(null,"Protocol");
} else {
}

if(cljs.core.truth_(new cljs.core.Keyword(null,"forms","forms",2045992350).cljs$core$IFn$_invoke$arity$1(m))){
var seq__18486_18508 = cljs.core.seq.call(null,new cljs.core.Keyword(null,"forms","forms",2045992350).cljs$core$IFn$_invoke$arity$1(m));
var chunk__18487_18509 = null;
var count__18488_18510 = (0);
var i__18489_18511 = (0);
while(true){
if((i__18489_18511 < count__18488_18510)){
var f_18512 = cljs.core._nth.call(null,chunk__18487_18509,i__18489_18511);
cljs.core.println.call(null,"  ",f_18512);


var G__18513 = seq__18486_18508;
var G__18514 = chunk__18487_18509;
var G__18515 = count__18488_18510;
var G__18516 = (i__18489_18511 + (1));
seq__18486_18508 = G__18513;
chunk__18487_18509 = G__18514;
count__18488_18510 = G__18515;
i__18489_18511 = G__18516;
continue;
} else {
var temp__5457__auto___18517 = cljs.core.seq.call(null,seq__18486_18508);
if(temp__5457__auto___18517){
var seq__18486_18518__$1 = temp__5457__auto___18517;
if(cljs.core.chunked_seq_QMARK_.call(null,seq__18486_18518__$1)){
var c__4351__auto___18519 = cljs.core.chunk_first.call(null,seq__18486_18518__$1);
var G__18520 = cljs.core.chunk_rest.call(null,seq__18486_18518__$1);
var G__18521 = c__4351__auto___18519;
var G__18522 = cljs.core.count.call(null,c__4351__auto___18519);
var G__18523 = (0);
seq__18486_18508 = G__18520;
chunk__18487_18509 = G__18521;
count__18488_18510 = G__18522;
i__18489_18511 = G__18523;
continue;
} else {
var f_18524 = cljs.core.first.call(null,seq__18486_18518__$1);
cljs.core.println.call(null,"  ",f_18524);


var G__18525 = cljs.core.next.call(null,seq__18486_18518__$1);
var G__18526 = null;
var G__18527 = (0);
var G__18528 = (0);
seq__18486_18508 = G__18525;
chunk__18487_18509 = G__18526;
count__18488_18510 = G__18527;
i__18489_18511 = G__18528;
continue;
}
} else {
}
}
break;
}
} else {
if(cljs.core.truth_(new cljs.core.Keyword(null,"arglists","arglists",1661989754).cljs$core$IFn$_invoke$arity$1(m))){
var arglists_18529 = new cljs.core.Keyword(null,"arglists","arglists",1661989754).cljs$core$IFn$_invoke$arity$1(m);
if(cljs.core.truth_((function (){var or__3949__auto__ = new cljs.core.Keyword(null,"macro","macro",-867863404).cljs$core$IFn$_invoke$arity$1(m);
if(cljs.core.truth_(or__3949__auto__)){
return or__3949__auto__;
} else {
return new cljs.core.Keyword(null,"repl-special-function","repl-special-function",1262603725).cljs$core$IFn$_invoke$arity$1(m);
}
})())){
cljs.core.prn.call(null,arglists_18529);
} else {
cljs.core.prn.call(null,((cljs.core._EQ_.call(null,new cljs.core.Symbol(null,"quote","quote",1377916282,null),cljs.core.first.call(null,arglists_18529)))?cljs.core.second.call(null,arglists_18529):arglists_18529));
}
} else {
}
}

if(cljs.core.truth_(new cljs.core.Keyword(null,"special-form","special-form",-1326536374).cljs$core$IFn$_invoke$arity$1(m))){
cljs.core.println.call(null,"Special Form");

cljs.core.println.call(null," ",new cljs.core.Keyword(null,"doc","doc",1913296891).cljs$core$IFn$_invoke$arity$1(m));

if(cljs.core.contains_QMARK_.call(null,m,new cljs.core.Keyword(null,"url","url",276297046))){
if(cljs.core.truth_(new cljs.core.Keyword(null,"url","url",276297046).cljs$core$IFn$_invoke$arity$1(m))){
return cljs.core.println.call(null,["\n  Please see http://clojure.org/",cljs.core.str.cljs$core$IFn$_invoke$arity$1(new cljs.core.Keyword(null,"url","url",276297046).cljs$core$IFn$_invoke$arity$1(m))].join(''));
} else {
return null;
}
} else {
return cljs.core.println.call(null,["\n  Please see http://clojure.org/special_forms#",cljs.core.str.cljs$core$IFn$_invoke$arity$1(new cljs.core.Keyword(null,"name","name",1843675177).cljs$core$IFn$_invoke$arity$1(m))].join(''));
}
} else {
if(cljs.core.truth_(new cljs.core.Keyword(null,"macro","macro",-867863404).cljs$core$IFn$_invoke$arity$1(m))){
cljs.core.println.call(null,"Macro");
} else {
}

if(cljs.core.truth_(new cljs.core.Keyword(null,"repl-special-function","repl-special-function",1262603725).cljs$core$IFn$_invoke$arity$1(m))){
cljs.core.println.call(null,"REPL Special Function");
} else {
}

cljs.core.println.call(null," ",new cljs.core.Keyword(null,"doc","doc",1913296891).cljs$core$IFn$_invoke$arity$1(m));

if(cljs.core.truth_(new cljs.core.Keyword(null,"protocol","protocol",652470118).cljs$core$IFn$_invoke$arity$1(m))){
var seq__18490_18530 = cljs.core.seq.call(null,new cljs.core.Keyword(null,"methods","methods",453930866).cljs$core$IFn$_invoke$arity$1(m));
var chunk__18491_18531 = null;
var count__18492_18532 = (0);
var i__18493_18533 = (0);
while(true){
if((i__18493_18533 < count__18492_18532)){
var vec__18494_18534 = cljs.core._nth.call(null,chunk__18491_18531,i__18493_18533);
var name_18535 = cljs.core.nth.call(null,vec__18494_18534,(0),null);
var map__18497_18536 = cljs.core.nth.call(null,vec__18494_18534,(1),null);
var map__18497_18537__$1 = ((((!((map__18497_18536 == null)))?(((((map__18497_18536.cljs$lang$protocol_mask$partition0$ & (64))) || ((cljs.core.PROTOCOL_SENTINEL === map__18497_18536.cljs$core$ISeq$))))?true:false):false))?cljs.core.apply.call(null,cljs.core.hash_map,map__18497_18536):map__18497_18536);
var doc_18538 = cljs.core.get.call(null,map__18497_18537__$1,new cljs.core.Keyword(null,"doc","doc",1913296891));
var arglists_18539 = cljs.core.get.call(null,map__18497_18537__$1,new cljs.core.Keyword(null,"arglists","arglists",1661989754));
cljs.core.println.call(null);

cljs.core.println.call(null," ",name_18535);

cljs.core.println.call(null," ",arglists_18539);

if(cljs.core.truth_(doc_18538)){
cljs.core.println.call(null," ",doc_18538);
} else {
}


var G__18540 = seq__18490_18530;
var G__18541 = chunk__18491_18531;
var G__18542 = count__18492_18532;
var G__18543 = (i__18493_18533 + (1));
seq__18490_18530 = G__18540;
chunk__18491_18531 = G__18541;
count__18492_18532 = G__18542;
i__18493_18533 = G__18543;
continue;
} else {
var temp__5457__auto___18544 = cljs.core.seq.call(null,seq__18490_18530);
if(temp__5457__auto___18544){
var seq__18490_18545__$1 = temp__5457__auto___18544;
if(cljs.core.chunked_seq_QMARK_.call(null,seq__18490_18545__$1)){
var c__4351__auto___18546 = cljs.core.chunk_first.call(null,seq__18490_18545__$1);
var G__18547 = cljs.core.chunk_rest.call(null,seq__18490_18545__$1);
var G__18548 = c__4351__auto___18546;
var G__18549 = cljs.core.count.call(null,c__4351__auto___18546);
var G__18550 = (0);
seq__18490_18530 = G__18547;
chunk__18491_18531 = G__18548;
count__18492_18532 = G__18549;
i__18493_18533 = G__18550;
continue;
} else {
var vec__18499_18551 = cljs.core.first.call(null,seq__18490_18545__$1);
var name_18552 = cljs.core.nth.call(null,vec__18499_18551,(0),null);
var map__18502_18553 = cljs.core.nth.call(null,vec__18499_18551,(1),null);
var map__18502_18554__$1 = ((((!((map__18502_18553 == null)))?(((((map__18502_18553.cljs$lang$protocol_mask$partition0$ & (64))) || ((cljs.core.PROTOCOL_SENTINEL === map__18502_18553.cljs$core$ISeq$))))?true:false):false))?cljs.core.apply.call(null,cljs.core.hash_map,map__18502_18553):map__18502_18553);
var doc_18555 = cljs.core.get.call(null,map__18502_18554__$1,new cljs.core.Keyword(null,"doc","doc",1913296891));
var arglists_18556 = cljs.core.get.call(null,map__18502_18554__$1,new cljs.core.Keyword(null,"arglists","arglists",1661989754));
cljs.core.println.call(null);

cljs.core.println.call(null," ",name_18552);

cljs.core.println.call(null," ",arglists_18556);

if(cljs.core.truth_(doc_18555)){
cljs.core.println.call(null," ",doc_18555);
} else {
}


var G__18557 = cljs.core.next.call(null,seq__18490_18545__$1);
var G__18558 = null;
var G__18559 = (0);
var G__18560 = (0);
seq__18490_18530 = G__18557;
chunk__18491_18531 = G__18558;
count__18492_18532 = G__18559;
i__18493_18533 = G__18560;
continue;
}
} else {
}
}
break;
}
} else {
}

if(cljs.core.truth_(n)){
var temp__5457__auto__ = cljs.spec.alpha.get_spec.call(null,cljs.core.symbol.call(null,[cljs.core.str.cljs$core$IFn$_invoke$arity$1(cljs.core.ns_name.call(null,n))].join(''),cljs.core.name.call(null,nm)));
if(cljs.core.truth_(temp__5457__auto__)){
var fnspec = temp__5457__auto__;
cljs.core.print.call(null,"Spec");

var seq__18504 = cljs.core.seq.call(null,new cljs.core.PersistentVector(null, 3, 5, cljs.core.PersistentVector.EMPTY_NODE, [new cljs.core.Keyword(null,"args","args",1315556576),new cljs.core.Keyword(null,"ret","ret",-468222814),new cljs.core.Keyword(null,"fn","fn",-1175266204)], null));
var chunk__18505 = null;
var count__18506 = (0);
var i__18507 = (0);
while(true){
if((i__18507 < count__18506)){
var role = cljs.core._nth.call(null,chunk__18505,i__18507);
var temp__5457__auto___18561__$1 = cljs.core.get.call(null,fnspec,role);
if(cljs.core.truth_(temp__5457__auto___18561__$1)){
var spec_18562 = temp__5457__auto___18561__$1;
cljs.core.print.call(null,["\n ",cljs.core.str.cljs$core$IFn$_invoke$arity$1(cljs.core.name.call(null,role)),":"].join(''),cljs.spec.alpha.describe.call(null,spec_18562));
} else {
}


var G__18563 = seq__18504;
var G__18564 = chunk__18505;
var G__18565 = count__18506;
var G__18566 = (i__18507 + (1));
seq__18504 = G__18563;
chunk__18505 = G__18564;
count__18506 = G__18565;
i__18507 = G__18566;
continue;
} else {
var temp__5457__auto____$1 = cljs.core.seq.call(null,seq__18504);
if(temp__5457__auto____$1){
var seq__18504__$1 = temp__5457__auto____$1;
if(cljs.core.chunked_seq_QMARK_.call(null,seq__18504__$1)){
var c__4351__auto__ = cljs.core.chunk_first.call(null,seq__18504__$1);
var G__18567 = cljs.core.chunk_rest.call(null,seq__18504__$1);
var G__18568 = c__4351__auto__;
var G__18569 = cljs.core.count.call(null,c__4351__auto__);
var G__18570 = (0);
seq__18504 = G__18567;
chunk__18505 = G__18568;
count__18506 = G__18569;
i__18507 = G__18570;
continue;
} else {
var role = cljs.core.first.call(null,seq__18504__$1);
var temp__5457__auto___18571__$2 = cljs.core.get.call(null,fnspec,role);
if(cljs.core.truth_(temp__5457__auto___18571__$2)){
var spec_18572 = temp__5457__auto___18571__$2;
cljs.core.print.call(null,["\n ",cljs.core.str.cljs$core$IFn$_invoke$arity$1(cljs.core.name.call(null,role)),":"].join(''),cljs.spec.alpha.describe.call(null,spec_18572));
} else {
}


var G__18573 = cljs.core.next.call(null,seq__18504__$1);
var G__18574 = null;
var G__18575 = (0);
var G__18576 = (0);
seq__18504 = G__18573;
chunk__18505 = G__18574;
count__18506 = G__18575;
i__18507 = G__18576;
continue;
}
} else {
return null;
}
}
break;
}
} else {
return null;
}
} else {
return null;
}
}
});

//# sourceMappingURL=repl.js.map
