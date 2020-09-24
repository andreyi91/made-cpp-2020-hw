.text

.global count_det

count_det:
	push {r4-r7}
	
	ldrsb r1, [r0], #4
	mov r6, r1
	ldrsb r3, [r0], #4
	mov r5, r3
	ldrsb r2, [r0], #4
	mov r4, r2
	ldrsb r7, [r0], #4
	mul r2, r2, r7
	mul r5, r5, r7
	ldrsb r7, [r0], #4
	mul r1, r1, r7
	mul r4, r4, r7
	ldrsb r7, [r0], #4
	mul r3, r3, r7
	mul r6, r6, r7
	ldrsb r7, [r0], #4
	mul r3, r3, r7
 	mul r4, r4, r7
 	ldrsb r7, [r0], #4
 	mul r2, r2, r7
 	mul r6, r6, r7
	ldrsb r7, [r0], #4
	mul r1, r1, r7
	mul r5, r5, r7

	add r0, r1, r2
	add r0, r3
	sub r0, r4
	sub r0, r5
	sub r0, r6
	
	pop {r4-r7}
	bx lr	
