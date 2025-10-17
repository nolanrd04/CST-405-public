.data

.text
.globl main


Function:
    # Function: Function
    addi $sp, $sp, -100
    sw $ra, 0($sp)
    lw $t0, 104($sp)
    sw $t0, 4($sp)  # Store param a
    lw $t0, 108($sp)
    sw $t0, 8($sp)  # Store param b
    lw $t0, 0($sp)
    lw $t1, 4($sp)
    add $t0, $t0, $t1
    move $v0, $t0
    lw $ra, 0($sp)
    addi $sp, $sp, 100
    jr $ra

main:
    # Function: main
    addi $sp, $sp, -400
    addi $sp, $sp, -100
    sw $ra, 0($sp)
    # Declared c at offset 8
    # Call function: Function
    li $t0, 4
    sw $t0, 4($sp)
    li $t0, 3
    sw $t0, 8($sp)
    jal Function
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
    li $t0, 0
    move $v0, $t0
    lw $ra, 0($sp)
    addi $sp, $sp, 100
    jr $ra
