# @(#)cshrc 1.11 89/11/29 SMI
umask 022


setenv TODDHOST `hostname -s`
alias setprompt set prompt='"[%{\033[37m%}"$TODDHOST":["%n"]:"%/"]%{\033[0m%} "'
setprompt

alias cd 'chdir \!* && setprompt'

#set path=(/bin /usr/bin /usr/sbin /usr/ucb /etc /usr/etc /usr/local/bin .)
if ( $?prompt ) then
        set history=32
endif



