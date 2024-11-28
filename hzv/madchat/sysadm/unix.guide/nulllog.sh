#!/bin/sh
# script d'anonymite sous linux (a faire seulement quand le sys est stable...)
# transforme les logs en liens symboliques "-> /dev/null"
# executer as root

 USER=azrael
 # autres fichiers a linker ->/dev/null selon votre config :
 EXTRA=.mc/history
 USERSOFT0=.lftp/cwd_history
 USERSOFT1=.lftp/rl_history
 COOKIES=.phoenix/default/c9yke7dc.slt/cookies.txt
 
for i in \
# ajouter des lignes selon ses daemons, base sur une slack :
/var/log/XFree86.0.log.old \
/var/log/XFree86.0.log \
/var/log/cron \
/var/log/debug \
/var/log/maillog \
/var/log/lastlog \
/var/log/messages \
/var/log/secure \
/var/log/syslog \
/var/log/wtmp \
/var/log/faillog \
/root/.bash_history \
/root/$EXTRA \
/home/$USER/.bash_history \
/home/$USER/$EXTRA \
/home/$USER/$COOKIES \
/home/$USER/$USERSOFT0 \
/home/$USER/$USERSOFT1 \
; do
	echo ln -s /dev/null $i;
	rm $i 2>/dev/null;
	ln -s /dev/null $i 2>/dev/null;
done



echo -e "\n   a ajouter dans la sequence de shutdown (rc.K sur une slack) :\
	 \n      rm -rf /var/spool/lpd/*      \
	 \n      rm -rf /tmp/*                \
         \n                                   \
	 \n   a gerer aussi : les mails, icq-history, fichiers perso, etc..\
	"

