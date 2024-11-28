<?php

/* PHP Thumbnail AutoIndex
 * by Tomasz Sterna / smoku / <tomek@smoczy.net>
 * Upgraded by tobozo : png and jpeg filters, cache system
 * License: GPL
 */
define (VERSION, "1.1.2");

/*****************************************************************/
$font = 2;

/* added by tobozo 06-04-03 */
$thumbnails_folder = "./.thumbnails/";

function pngThumbnail($file) {
    global $font;

    if (function_exists('imagecreatetruecolor')) {
        $im = imagecreatetruecolor(112, 112);
        $white = imagecolorclosest ( $im, 255, 255, 255);
        $frame = imagecolorclosest ( $im, 58, 58, 58);
        $black = imagecolorclosest ( $im, 0, 0, 0);
    } else {
        $im = imagecreate(112, 112);
        $white = imagecolorallocate ($im, 255, 255, 255);
        $frame = imagecolorallocate ( $im, 58, 58, 58);
        $black = imagecolorallocate ( $im, 0, 0, 0);
    }
    imagefilledrectangle ( $im, 0, 0, 112, 112, $black);

    $size = @getimagesize($file);
    $name = ereg_replace('(.*)\.[^\.]*$', '\1', $file);
    switch($size[2]) {
        case '':
                $name = $file;
            if (is_dir($file)) {
                $si = imgFolder();
                $size[0] = 94;
                $size[1] = 94;
            } else {
                $ri = imgTXT(mime_content_type($file));
                $si = $ri['image'];
                $size[0] = $ri['width'];
                $size[1] = $ri['width'];
            }
            break;
        case 2:
            $si = imagecreatefromjpeg($file);
            break;
        case 3:
            $si = imagecreatefrompng($file);
            break;
        case 1:
            if (function_exists('imagecreatefromgif')){
                $si = imagecreatefromgif($file);
                break;
            }
        default :
            $arr = split('/', $size['mime']);
            $ri = imgTXT(strtoupper($arr[1]));
            $si = $ri['image'];
            $size[0] = $ri['width'];
            $size[1] = $ri['width'];
    }
    $im_w = $size[0];
    $im_h = $size[1];
    if ( $im_w > $im_h ) {
        $dx = 9;
        $dw = 94;
        $dh = floor(94 * $im_h/$im_w);
        $dy = floor((96 - $dh)/2);
    } else {
        $dy = 1;
        $dh = 94;
        $dw = floor(94 * $im_w/$im_h);
        $dx = floor((112 - $dw)/2);
    }
    if (function_exists('imagecopyresampled')) {
        imagecopyresampled( $im, $si, $dx, $dy, 0, 0, $dw, $dh, $im_w, $im_h);
    } else {
        imagecopyresized( $im, $si, $dx, $dy, 0, 0, $dw, $dh, $im_w, $im_h);
    }
    imagerectangle ( $im, 8, 0, 103, 95, $frame);
    $px = (112 - imagefontwidth($font) * strlen($name)) / 2;
    if ($px < 0) $px = 0;
    imagestring($im, $font, $px, 98, $name, $white);

    imageinterlace($im, 1);
    imagepng($im);
    imagedestroy($im);
    imagedestroy($si);
    /* commented by tobozo 06-04-03 */
    //exit();
}
function imgTXT($text) {
    $font = 5;
    $txw = imagefontwidth(5) * strlen($text);
    $txh = imagefontheight(5);
    if (function_exists('imagecreatetruecolor')) {
        $img = imagecreatetruecolor($txw, $txw);
        $grey = imagecolorclosest ( $img, 46, 46, 46);
        $black = imagecolorclosest ( $img, 0, 0, 0);
    } else {
        $img = imagecreate($txw, $txw);
        $grey = imagecolorallocate ( $img, 46, 46, 46);
        $black = imagecolorallocate ( $img, 0, 0, 0);
    }
    imagefilledrectangle ( $img, 0, 0, $txw, $txw, $grey);
    $py = ($txw - $txh) / 2;
    imagestring($img, 5, 0, $py, $text, $black);
    $ret = array();
    $ret['image'] = $img;
    $ret['width'] = $txw;
    return $ret;
}
function imgFolder() {
    $a = array(
        18,24,
        38,24,
        43,29,
        76,29,
        76,70,
        18,70
    );
    if (function_exists('imagecreatetruecolor')) {
        $img = imagecreatetruecolor(94, 94);
        $black = imagecolorclosest ( $img, 0, 0, 0);
        $yellow = imagecolorclosest ( $img, 250, 240, 0);
        $white = imagecolorclosest ( $img, 255, 255, 255);
    } else {
        $img = imagecreate(94, 94);
        $black = imagecolorallocate ( $img, 0, 0, 0);
        $yellow = imagecolorallocate ( $img, 250, 240, 0);
        $white = imagecolorallocate ( $img, 255, 255, 255);
    }
    imagefilledrectangle ( $img, 0, 0, 94, 94, $white);
    if (function_exists('imagesetthickness')) {
        imagesetthickness($img, 1);
    }
    imagefilledpolygon ($img, $a, 6, $yellow);
    imagepolygon ($img, $a, 6, $black);
    imageline ($img, 40, 32, 76, 32, $black);
    imageline ($img, 18, 34, 34, 34, $black);
    imageline ($img, 34, 34, 40, 32, $black);
    return $img;
}
function ThLink($file, $link='') {
    global $font;
    
    $stdimg = 'border="0" width="112" height="112" hspace="4" vspace="4"';

    $ext = end(explode('.', $file));
    if (eregi("gif", $ext)) {
      if($link!='') {
        return "<a href=\"$file\"><img src=\"$link\" alt=\"$file/\" $stdimg></a>";  
        }
      else {
        return "<a href=\"$file\"><img src=\"?$file\" alt=\"$file/\" $stdimg></a>";
        }
      }
      
    if (is_dir($file)) {
        return "<a href=\"$file/\"><img src=\"?$file\" alt=\"$file/\" $stdimg></a>";
    } else {
        $size = filesize($file);
        $suffix = '';
        if ($size > 4000) {
            $size = floor($size/1024);
            $suffix = 'k';
        }
        if ($size > 2000) {
            $size = floor($size/1024);
            $suffix = 'M';
        }
        if (($imsize = @getimagesize($file)) && ($imsize[2] == 2 || $imsize[2] == 3)) {
            $name = ereg_replace('(.*)\.[^\.]*$', '\1', $file);
            if (imagefontwidth($font) * strlen($name) > 112) {
                $name .= "\n";
            } else {
                $name = '';
            }
            /* added by tobozo 06-04-03 */
            print "<a href=\"$file\">";
            if($link!='') {
              print "<img src=\"$link\" alt=\"$file\" title=\"";
              }
            else {
              print "<img src=\"?$file\" alt=\"$file\" title=\"";
              }
            print "$name";
            print "Dimensions: {$imsize[0]} x {$imsize[1]}\n";
            print "Type: {$imsize['mime']}\n";
            print "Size: $size {$suffix}B\" $stdimg></a>";
        } else {
            if (imagefontwidth($font) * strlen($file) > 112) {
                $name .= "$file\n";
            } else {
                $name = '';
            }
            /* added by tobozo 06-04-03 */
            print "<a href=\"$file\">";
            if($link!='') {
              print "<img src=\"$link\" alt=\"$file\" title=\"";
              }
            else {
              print "<img src=\"?$file\" alt=\"$file\" title=\"";
              }
            print "$name";
            print "Type: ".mime_content_type($file)."\n";
            print "Size: $size {$suffix}B\" $stdimg></a>";
        }
    }
}

// ----------------------------------------------------------------------------
if (isset($_SERVER['QUERY_STRING']) && ($_SERVER['QUERY_STRING'] !== '')) {
    $file = $_SERVER['QUERY_STRING'];
    $regs = array();
    while (ereg('%[0-9]{2}', $file, $regs)) {
        $file = str_replace($regs[0], chr(hexdec($regs[0])), $file);
    }
    $file = str_replace('/', '', $file);
    if (is_readable($file)) {

      /* added by tobozo 06-04-03 */
      
      $mtime = gmdate('D, d M Y H:i:s \G\M\T', filemtime($file));
      header('Content-type: image/png');
      header('Connection: close');
      header("Date: $mtime");
      header("Last-Modified: $mtime");
      
      if(!file_exists($thumbnails_folder.$file)) {
        /* added by tobozo 06-04-03 */
        ob_start(); 
        pngThumbnail($file);
        $image = ob_get_contents(); 
        ob_end_clean(); 
        $fp = fopen("$thumbnails_folder$file", "w");
        fputs($fp, $image);
        fclose($fp);
        
        echo $image;
        
        }
      else {
        /* added by tobozo 06-04-03 */
        include($thumbnails_folder.$file);
        }


        exit();
    }
} else {
    $path = split('/', $_SERVER['PHP_SELF']);
    $self = array_pop($path);
    $dir = join('/', $path);
    $currdir = array_pop($path);
    $parent = join('/', $path);;

print '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
 <head>
  <title>Index of '.$dir.'</title>
 </head>
<body BGCOLOR=#000000 TEXT=#808080 LINK=#008480 VLINK=#707070> 
<h1>Index of '.$dir.'</h1><pre><hr /><a href="'.$parent."/\"><img SRC=/.back.gif border=0>Parent Directory</a></pre>\n";


    if ($handle = opendir('.')) {
        while (false !== ($file = readdir($handle))) {
            if ($file != "." && $file != ".." && $file != $self && $file[0] != '.') {
                
                $ext = end(explode('.', $file));
                if(eregi("jpg|jpeg|png|gif", $ext)) {
                  if(file_exists($thumbnails_folder.$file)) {
                    print ThLink($file, $thumbnails_folder.$file);                  
                    }
                  else {
                    print ThLink($file);
                    }
                  }
                else {
                  if(is_dir($file)) {
                    if(file_exists($file.'/'.$self)) {
                      $link = $file.'/'.$self;
                      $extradirs .= "<IMG SRC=/.layout.gif ALT=[DIR]> <IMG SRC=/.iconsfoldergray.gif ALT=[DIR]> <A HREF=$link>$file/</A><br /> ";
                      }
                    else {
                      $extradirs .= "<IMG SRC=/.iconsfoldergray.gif ALT=[DIR]> <A HREF=$file>$file/</A><br /> ";
                      }
                    }
                  else {
                    $extralinks .= "<small>[<a href=$file>$file</a>]</small><br />";
                    }
                  }
            }
        }
        closedir($handle);
        echo '<br />'.$extradirs.'<br />'.$extralinks;
    }
    
    if (function_exists('imagecopyresampled')) {
        $gdver = 'libGD2';
    } else {
        $gdver = 'libGD1(aka "ugly thumbnails")';
    }

print '<hr />
<address><a href=./>Voir les descriptions des fichiers</a> Thumbnail AutoIndex/'.VERSION."-mAd PHP/$gdver at {$_SERVER['SERVER_NAME']}</address>
</body></html>";

}

exit;
