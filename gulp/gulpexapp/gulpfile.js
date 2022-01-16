const gulp = require("gulp");
const imagemin = require("gulp-imagemin");
const uglify = require("gulp-uglify");
const sass = require("gulp-sass")(require("sass"));
const concat = require("gulp-concat");

// import gulp from "gulp";
// import imagemin from "gulp-imagemin";
// import uglify from "gulp-uglify";
// import sass from "gulp-sass";
// import babel from "gulp-babel";

// var pipeline = require("readable-stream").pipeline;
// import { pipeline } from "readable-stream";

// var pipeline = require("readable-stream").pipeline;
// import readableStream from "readable-stream";
// const { Stream: pipeline } = readableStream;
// console.log("uglify", uglify);
// console.log("readableStream", readableStream);
// console.log("pipeline", pipeline);

/*
  -- TOP LEVEL FUNCTION --
  gulp.task - Define tasks
  gulp.src - Point tofiles to use
  gulp.dest - Points to folder to output
  gulp.watch - Watch files and folders for changes
*/

// Logs Message
gulp.task("message", async function () {
  console.log("Gulp is running...");
});

// Copy ALL HTML files
gulp.task("copyHtml", async function () {
  gulp.src("src/*.html").pipe(gulp.dest("dist"));
});

// Optimize Images
gulp.task("imageMin", async () => {
  gulp.src("src/images/*").pipe(imagemin()).pipe(gulp.dest("dist/images"));
});

// Minify JS
gulp.task("minify", async function () {
  gulp.src("src/js/*.js").pipe(uglify()).pipe(gulp.dest("dist/js"));
});

// Compile Sass
gulp.task("sass", async function () {
  gulp
    .src("src/sass/*.scss")
    .pipe(sass().on("error", sass.logError))
    .pipe(gulp.dest("dist/css"));
});

// Scripts
gulp.task("scripts", async function () {
  gulp
    .src("src/js/*.js")
    .pipe(concat("main.js"))
    .pipe(uglify())
    .pipe(gulp.dest("dist/js"));
});

gulp.task(
  "default",
  gulp.series("message", "copyHtml", "imageMin", "sass", "scripts")
);

gulp.task("watch", async function () {
  gulp.watch("src/js/*.js", gulp.series("scripts"));
  gulp.watch("src/image/*", gulp.series("imageMin"));
  gulp.watch("src/sass/*.cscc", gulp.series("sass"));
  gulp.watch("src/*.html", gulp.series("copyHtml"));
});
