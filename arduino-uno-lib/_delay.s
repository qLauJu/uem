.global delay_1us
.global delay_1ms

delay_1us:
    ldi  r18, 5
L1: dec  r18
    brne L1
    RET


delay_1ms:
    ldi  r18, 21
    ldi  r19, 199
L2: dec  r19
    brne L2
    dec  r18
    brne L2
    RET
