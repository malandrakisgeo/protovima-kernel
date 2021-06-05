[bits 32]
[extern main]
call main
.hltloop:
        hlt
        jmp .hltloop
;jmp $ 
