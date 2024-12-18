<?php die(); /* XSS SHIELD, REMOVE THIS LINE */ ?>
<?
/*
 *  justify.class 1.1 18/02/2002
 *
 * PHP script to justify paragraph text on a whitespace basis
 * Text-processing script
 *
 * Copyrleft (c+) 2002 tobozo@users.sourceforge.net All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purposes and without
 * fee is hereby granted provided that this copyright notice
 * appears in all copies.
 *
 * Of course, this soft is provided "as is" without express or implied
 * warranty of any kind.
 *
 * usage : echo justify($some_text, $width); // $width = nb of columns
 *
 */



/* This works only on paragraphs
 * If more than one paragraph needs to be justified, just
 * call the function more than once ;0)
 */
function justify_text($text, $width=81) {
  $size   = strlen($text); // what's the size of the text ?
  if($size>6000) {
    return false;
    }

  // cleanup
  $text=stripslashes($text);
  $text=ereg_replace("\r", " ", $text);
  $text=ereg_replace("\t", " ", $text);
  $text=ereg_replace("\n", " ", $text);
  while(ereg("  ", $text)) {
    $text=str_replace("  ", " ", $text);
    }
  $size   = strlen($text); // what text size ?

  //  put words in array
  $string = explode(" ",$text);
  $words = count($string);
  $a=1;
  // strip lines smaller than $width to an array
  for ($i = 0 ; $i < $words ; $i++) {
    if(strlen($output[$a].$string[$i])>=$width) {
      $a++;
      }
    $output[$a] .= $string[$i]." ";
    };

  $i = -1;
  while($i<count($output)) {
    $v=ltrim(chop($output[$i]));
    $len = strlen($v)+1;
    $i++;
    $page.=justify_line($v, $width-1)."\n";

    }
  return $page.$output[count($output)]."\n";
  }



/* This justifies only one line (provided strlen($text) is smaller than $width
 * If the line is longer than expected, it is stripped and returned as is.
 */
function justify_line($text, $width) {
  $width++;
  $text = ltrim(chop($text));
  $len  = strlen($text);

  if($width<=10 || width>132) { // limitation to 132 cols
    return false;
    }

  if($len>=$width) {
    return substr($text,0, $width);
    }

  $neededspaces = $width - $len;
  $availablespaces = explode(" ", $text);
  $availablespaces = count($availablespaces)-1;
  $ratio = $availablespaces / $neededspaces;

  $usablespaces = "";

  if($ratio!=0) {
    if($neededspaces>=$availablespaces) {
      while(($width>=$len)  && ($neededspaces>=0)) {
        $i = 0;// used as pointer inside the string
        while(($i<=$len)   && ($neededspaces>=0) ) { // parse all string
          $temp=substr($text,$i,1); // get one letter
          if($temp==" ") { // is it a space ?
            $text=substr($text,0,$i)."  ".substr($text,$i+1,$len);// insert another space
            $neededspaces--;// one space less
            $len = strlen($text);
            $i++;
            }
          $i++;
          }
        $len = strlen($text);
        }
      }
    else {
      while(($width>=$len)  && ($neededspaces>=0)) {
        $i = 0;// used as pointer inside the string
        while(($i<=$len)   && ($neededspaces>=0) ) { // parse all string
          $temp=substr($text,$i,1); // get one letter
          if($temp==" ") { // is it a space ?
            $text=substr($text,0,$i)."  ".substr($text,$i+1,$len);// insert another space
            $neededspaces--;// one space less
            $len = strlen($text);
            $i++;
            }
          $i++;
          }
        $len = strlen($text);
        }
      }
    }
  $out=$text;
  return $out;
  }


/* example code (provided with package */
$text   = $HTTP_POST_VARS[text];
$width  = $HTTP_POST_VARS[width];
$submit = $HTTP_POST_VARS[submit];

if($width>132 || $width<20) {
  $width=80;
  }

if($text && $submit) {
  echo "<pre>".justify_text($text, $width)."</pre>";
  }
else {
  ?><html>
 <head>
 <title>PHPJustify By tobozo (c+)</title>
</head>

 <body bgcolor="black" text="red" link="green" vlink="green">
<script language="Javascript">
<!--
function verform() {
  if(document.formx.text.value == ""){
    alert("You must paste or type some text!");
    document.formx.text.focus();
    return false;
    }
  if(document.formx.width.value == ""){
    alert("You must specify column width!");
    document.formx.width.focus();
    return false;
    }
  if(document.formx.width.value > 132){
    alert("That column width is too wide. Must be between 20 and 132 characters");
    document.formx.width.focus();
    return false;
    }
  if(document.formx.width.value < 20){
    alert("That column width is too narrow. Must be between 20 and 132 characters");
    document.formx.width.focus();
    return false;
    }
  if(document.formx.text.value.length > 5000){
    document.formx.text.value = document.formx.text.value.substring(0, 5000);
    alert("That text Field is too big. It has been trimmed.\nCheck it and Try Again.");
    document.formx.text.focus();
    return false;
    }
  }

// -->
</script>
<table border=0 cellpadding=10>
  <tr>
   <form name="formx" action="<? echo $PHP_SELF; ?>" method="POST" OnSubmit="return verform()">
   <td bgcolor="black" valign=top align=left>
    <img src=http://savate.madchat.org/images/savate.gif align=right valign=top>
    <font face="Helvetica,Arial" size=2>
    <p>
    <font color="green" size=5><strong>Justify Now!</strong></font>
     <p>
     Width of column, in characters: <input type=text name="width" size="4" value="80">
     <p>
     Paste text you want justified here (one paragraph at a time) :
     <br>
     <textarea name="text" cols="80" rows="10"></textarea>
     <p>
     <center>
     <p align="center">
     <input type="reset" value="Clear">
     <input type="submit" name="submit" value="Justify">
     </p>
     </center>
    </td>
   </form>
   </tr>
</table>
 </body>

 </html><?
  }
?>
