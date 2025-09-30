.data

.text
.globl main
main:
    # Allocate stack space
    addi $sp, $sp, -400

    # Declared array numbers of size 5 at offset 0
    li $t0, 0
    li $t1, 1
    sll  $t0, $t0, 2      # idx * 4
    addiu $t2, $sp, 0   # base of numbers
    addu  $t3, $t2, $t0  # element address
    sw    $t1, 0($t3)     # store value
    li $t0, 1
    li $t1, 2
    sll  $t0, $t0, 2      # idx * 4
    addiu $t2, $sp, 0   # base of numbers
    addu  $t3, $t2, $t0  # element address
    sw    $t1, 0($t3)     # store value
    li $t0, 2
    li $t1, 0
    sll  $t1, $t1, 2      # idx * 4
    addiu $t2, $sp, 0   # base of numbers
    addu  $t3, $t2, $t1  # element address
    lw    $t4, 0($t3)     # load array element
    li $t5, 1
    sll  $t5, $t5, 2      # idx * 4
    addiu $t6, $sp, 0   # base of numbers
    addu  $t7, $t6, $t5  # element address
    lw    $t0, 0($t7)     # load array element
    add $t4, $t4, $t0
    sll  $t0, $t0, 2      # idx * 4
    addiu $t5, $sp, 0   # base of numbers
    addu  $t6, $t5, $t0  # element address
    sw    $t4, 0($t6)     # store value
    li $t0, 2
    sll  $t0, $t0, 2      # idx * 4
    addiu $t1, $sp, 0   # base of numbers
    addu  $t2, $t1, $t0  # element address
    lw    $t3, 0($t2)     # load array element
    # Print integer
    move $a0, $t3
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    li $t0, 1
    # Storing value at array position 0
    addiu $t1, $sp, 20      # base address
    li    $t2, 0           # offset = 0 * 4
    addu  $t3, $t1, $t2    # element address
    sw    $t0, 0($t3)      # store value
    li $t0, 2
    # Storing value at array position 1
    addiu $t1, $sp, 20      # base address
    li    $t2, 4           # offset = 1 * 4
    addu  $t3, $t1, $t2    # element address
    sw    $t0, 0($t3)      # store value
    li $t0, 3
    # Storing value at array position 2
    addiu $t1, $sp, 20      # base address
    li    $t2, 8           # offset = 2 * 4
    addu  $t3, $t1, $t2    # element address
    sw    $t0, 0($t3)      # store value
    li $t0, 4
    # Storing value at array position 3
    addiu $t1, $sp, 20      # base address
    li    $t2, 12           # offset = 3 * 4
    addu  $t3, $t1, $t2    # element address
    sw    $t0, 0($t3)      # store value
    li $t0, 5
    # Storing value at array position 4
    addiu $t1, $sp, 20      # base address
    li    $t2, 16           # offset = 4 * 4
    addu  $t3, $t1, $t2    # element address
    sw    $t0, 0($t3)      # store value
    li $t0, 3
    sll  $t0, $t0, 2      # idx * 4
    addiu $t1, $sp, 20   # base of list
    addu  $t2, $t1, $t0  # element address
    lw    $t3, 0($t2)     # load array element
    # Print integer
    move $a0, $t3
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    # Declared array list2d of size 3 , 4 at offset 40
    li $t0, 0
    li $t1, 0
    li $t2, 6
    # 2D array assignment: list2d[i][j] = value
    li $t3, 4            # sizeY = 4
    mult $t0, $t3        # indexX * sizeY
    mflo $t4              # get result
    add $t4, $t4, $t1    # (indexX*sizeY + indexY)
    sll $t4, $t4, 2       # multiply by 4
    addiu $t5, $sp, 40    # base address
    addu $t6, $t5, $t4    # final address
    sw $t2, 0($t6)       # store value
    li $t0, 2
    li $t1, 2
    li $t2, 7
    # 2D array assignment: list2d[i][j] = value
    li $t3, 4            # sizeY = 4
    mult $t0, $t3        # indexX * sizeY
    mflo $t4              # get result
    add $t4, $t4, $t1    # (indexX*sizeY + indexY)
    sll $t4, $t4, 2       # multiply by 4
    addiu $t5, $sp, 40    # base address
    addu $t6, $t5, $t4    # final address
    sw $t2, 0($t6)       # store value
    li $t0, 2
    li $t1, 2
    # 2D array access: list2d[i][j]
    li $t2, 4           # sizeY = 4
    mult $t0, $t2       # multiply indexX * sizeY
    mflo $t3             # get result from LO register
    add $t4, $t3, $t1  # (indexX*sizeY + indexY)
    sll $t4, $t4, 2     # multiply by 4
    addiu $t5, $sp, 40   # base address
    addu $t6, $t5, $t4 # final address
    lw $t7, 0($t6)      # load array[i][j]
    # Print integer
    move $a0, $t7
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall
    li $t0, 0
    li $t1, 0
    # 2D array access: list2d[i][j]
    li $t2, 4           # sizeY = 4
    mult $t0, $t2       # multiply indexX * sizeY
    mflo $t3             # get result from LO register
    add $t4, $t3, $t1  # (indexX*sizeY + indexY)
    sll $t4, $t4, 2     # multiply by 4
    addiu $t5, $sp, 40   # base address
    addu $t6, $t5, $t4 # final address
    lw $t7, 0($t6)      # load array[i][j]
    # Print integer
    move $a0, $t7
    li $v0, 1
    syscall
    # Print newline
    li $v0, 11
    li $a0, 10
    syscall

    # Exit program
    addi $sp, $sp, 400
    li $v0, 10
    syscall
