.data

.text
.globl Add
.globl Multiply
.globl main


Add:
    # Function: Add (Scope Level: 1)
    addi $sp, $sp, -100
    sw $ra, 0($sp)
    lw $t0, 104($sp)
    sw $t0, 4($sp)  # Store param x (int) at scope 1
    lw $t0, 108($sp)
    sw $t0, 8($sp)  # Store param y (int) at scope 1
    # Declared result at scope 1, offset 12
    lw $t0, 4($sp)
    lw $t1, 8($sp)
    add $t0, $t0, $t1
    sw $t0, 12($sp)
    lw $t0, 12($sp)
    move $v0, $t0
    lw $ra, 0($sp)
    addi $sp, $sp, 100
    jr $ra

Multiply:
    # Function: Multiply (Scope Level: 1)
    addi $sp, $sp, -100
    sw $ra, 0($sp)
    lw $t0, 104($sp)
    sw $t0, 4($sp)  # Store param x (int) at scope 1
    lw $t0, 108($sp)
    sw $t0, 8($sp)  # Store param y (int) at scope 1
    # Declared result at scope 1, offset 12
    lw $t1, 4($sp)
    lw $t2, 8($sp)
    mul $t1, $t1, $t2
    sw $t1, 12($sp)
    lw $t0, 12($sp)
    move $v0, $t0
    lw $ra, 0($sp)
    addi $sp, $sp, 100
    jr $ra

main:
    # Function: main (Scope Level: 1)
    addi $sp, $sp, -400
    addi $sp, $sp, -100
    sw $ra, 0($sp)
    # Declared x at scope 1, offset 4
    li $t1, 10
    sw $t1, 4($sp)
    # If statement
    lw $t0, 4($sp)
    li $t1, 5
    sgt $t0, $t0, $t1
    beqz $t0, else_0
    lw $t0, 4($sp)
    # Print integer
    move $a0, $t0
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    # Declared y at scope 1, offset 8
    # Call function: Add
    li $t0, 4
    sw $t0, 4($sp)
    li $t0, 3
    sw $t0, 8($sp)
    jal Add
    move $t0, $v0
    sw $t0, 8($sp)
    lw $t0, 8($sp)
    # Print integer
    move $a0, $t0
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    j end_if_0
else_0:
    li $t0, 0
    # Print integer
    move $a0, $t0
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
end_if_0:
    # If statement
    lw $t0, 4($sp)
    li $t1, 5
    slt $t0, $t0, $t1
    beqz $t0, else_1
    lw $t0, 4($sp)
    # Print integer
    move $a0, $t0
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    j end_if_1
else_1:
    li $t0, 0
    # Print integer
    move $a0, $t0
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
end_if_1:
    li.s $f0, 2.000000
    mov.s $f4, $f0
    addi $sp, $sp, -4
    swc1 $f4, 0($sp)
    li $t0, 1
    mtc1 $t0, $f2
    cvt.s.w $f2, $f2
    lwc1 $f4, 0($sp)
    addi $sp, $sp, 4
    add.s $f0, $f4, $f2
    mov.s $f4, $f0
    addi $sp, $sp, -4
    swc1 $f4, 0($sp)
    li.s $f0, 3.000000
    mov.s $f4, $f0
    addi $sp, $sp, -4
    swc1 $f4, 0($sp)
    li $t0, 4
    mtc1 $t0, $f2
    cvt.s.w $f2, $f2
    lwc1 $f4, 0($sp)
    addi $sp, $sp, 4
    mul.s $f0, $f4, $f2
    mov.s $f2, $f0
    lwc1 $f4, 0($sp)
    addi $sp, $sp, 4
    div.s $f0, $f4, $f2
    # Print float
    mov.s $f12, $f0
    li $v0, 2
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    li $t0, 0
    move $v0, $t0
    lw $ra, 0($sp)
    addi $sp, $sp, 100
    jr $ra
