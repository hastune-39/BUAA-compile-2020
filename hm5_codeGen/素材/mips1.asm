.data
thisisaenter: .asciiz "\n"
.text
#变量存储基准:
#$gp
li $k0, 269549568  #局部变量基地址
li $k1, 270598144  #临时变量基地址
##############start############## 

#全局变量初始化:
li $t1, 19971231
sw $t1, 4000($gp)
li $t1, 19981013
sw $t1, 4004($gp)
li $t1, 1000000007
sw $t1, 4008($gp)
li $t1, 0
sw $t1, 4012($gp)

#跳转到主函数:
j main
myscanf: 
#保存返回地址: 
sw $ra, 4($k0)
addiu $s0, $0, 0
li $v0, 5
syscall
move $s0, $v0
#函数返回值为: 
move $a0, $s0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
myprintf: 
#保存返回地址: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s0,$t1
#写语句,在源代码第15行
li $v0, 1
move $a0, $s0
syscall
li $v0, 4
la $a0, thisisaenter
syscall
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
set: 
#保存返回地址: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s3,$t1
lw $t1, 12($k0)
move $s2,$t1
li $t1, 0
sw $t1, 16($k0)
addiu $s1, $0, 10
addiu $s0, $0, 0
li $t1, 1
sw $t1, 16($k0)
lw $t1, 16($k0)
sll $t3, $t1, 1
sw $t3, 20($k0)
lw $t1, 20($k0)
sll $t3, $t1, 1
sw $t3, 24($k0)
lw $t1, 24($k0)
sll $t3, $t1, 1
sw $t3, 28($k0)
lw $t1, 28($k0)
sll $t3, $t1, 1
sw $t3, 32($k0)
lw $t1, 32($k0)
sll $t3, $t1, 1
sw $t3, 36($k0)
lw $t1, 36($k0)
sll $t3, $t1, 1
sw $t3, 40($k0)
lw $t1, 40($k0)
sll $t3, $t1, 1
sw $t3, 44($k0)
lw $t1, 44($k0)
sll $t3, $t1, 1
sw $t3, 48($k0)
lw $t1, 48($k0)
sll $t3, $t1, 1
sw $t3, 52($k0)
lw $t1, 52($k0)
sll $t3, $t1, 1
sw $t3, 56($k0)
while_b1: 

#while语句,在源代码第36行
li $t1, 30
bge $s1, $t1, while_end3
addiu $t3, $s1, 1
move $s1, $t3
sll $t3, $s1, 2
addiu $t0, $t3, 16
sw $t0, 200($k1)
addiu $t3, $s1, -1
sll $t3, $t3, 2
addiu $t0, $t3, 16
sw $t0, 220($k1)
lw $t1, 220($k1)
addu $t1, $t1, $k0
lw $t1, 0($t1)
sll $t3, $t1, 1
lw $t0, 200($k1)
addu $t0, $t0, $k0
sw $t3, 0($t0)
j while_b1
while_end3: 
li $t2, 30
div $s3, $t2
mflo $t3 

#if条件语句,源代码第42行
li $t1, 1000
blt $t3, $t1, else4
#函数返回值为: 
li $a0, 0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else4: #if退出标签，源代码第43行
li $t2, 30
div $s3, $t2
mflo $t3 
sll $t3, $t3, 2
addiu $t0, $t3, 0
sw $t0, 264($k1)
li $t2, 30
div $s3, $t2
mflo $t3 
li $t2, 30
mul $t3, $t3, $t2
subu $t3, $s3, $t3
sll $t3, $t3, 2
addiu $t0, $t3, 16
sw $t0, 292($k1)
lw $t1, 264($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
lw $t2, 292($k1)
addu $t2, $t2, $k0
lw $t2, 0($t2)
div $t1, $t2
mflo $t3 
li $t2, 30
div $s3, $t2
mflo $t4 
sll $t4, $t4, 2
addiu $t0, $t4, 0
sw $t0, 316($k1)
li $t2, 30
div $s3, $t2
mflo $t4 
li $t2, 30
mul $t4, $t4, $t2
subu $t4, $s3, $t4
sll $t4, $t4, 2
addiu $t0, $t4, 16
sw $t0, 344($k1)
lw $t1, 316($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
lw $t2, 344($k1)
addu $t2, $t2, $k0
lw $t2, 0($t2)
div $t1, $t2
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4

#if条件语句,源代码第43行
beq $t3, $s2, else5
li $t2, 30
div $s3, $t2
mflo $t3 
sll $t3, $t3, 2
addiu $t0, $t3, 0
sw $t0, 388($k1)
li $t2, 30
div $s3, $t2
mflo $t3 
li $t2, 30
mul $t3, $t3, $t2
subu $t3, $s3, $t3
sll $t3, $t3, 2
addiu $t0, $t3, 16
sw $t0, 416($k1)
lw $t1, 388($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
lw $t2, 416($k1)
addu $t2, $t2, $k0
lw $t2, 0($t2)
div $t1, $t2
mflo $t3 
li $t2, 30
div $s3, $t2
mflo $t4 
sll $t4, $t4, 2
addiu $t0, $t4, 0
sw $t0, 440($k1)
li $t2, 30
div $s3, $t2
mflo $t4 
li $t2, 30
mul $t4, $t4, $t2
subu $t4, $s3, $t4
sll $t4, $t4, 2
addiu $t0, $t4, 16
sw $t0, 468($k1)
lw $t1, 440($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
lw $t2, 468($k1)
addu $t2, $t2, $k0
lw $t2, 0($t2)
div $t1, $t2
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4

#if条件语句,源代码第44行
li $t1, 0
bne $t3, $t1, else6

#if条件语句,源代码第45行
li $t1, 1
bne $s2, $t1, else7
li $t2, 30
div $s3, $t2
mflo $t3 
li $t2, 30
mul $t3, $t3, $t2
subu $t3, $s3, $t3
sll $t3, $t3, 2
addiu $t0, $t3, 16
sw $t0, 528($k1)
lw $t1, 528($k1)
addu $t1, $t1, $k0
lw $t1, 0($t1)
move $s0, $t1
else7: #if退出标签，源代码第48行
else6: #if退出标签，源代码第48行
li $t2, 30
div $s3, $t2
mflo $t3 
sll $t3, $t3, 2
addiu $t0, $t3, 0
sw $t0, 552($k1)
li $t2, 30
div $s3, $t2
mflo $t3 
li $t2, 30
mul $t3, $t3, $t2
subu $t3, $s3, $t3
sll $t3, $t3, 2
addiu $t0, $t3, 16
sw $t0, 580($k1)
lw $t1, 552($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
lw $t2, 580($k1)
addu $t2, $t2, $k0
lw $t2, 0($t2)
div $t1, $t2
mflo $t3 
li $t2, 30
div $s3, $t2
mflo $t4 
sll $t4, $t4, 2
addiu $t0, $t4, 0
sw $t0, 604($k1)
li $t2, 30
div $s3, $t2
mflo $t4 
li $t2, 30
mul $t4, $t4, $t2
subu $t4, $s3, $t4
sll $t4, $t4, 2
addiu $t0, $t4, 16
sw $t0, 632($k1)
lw $t1, 604($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
lw $t2, 632($k1)
addu $t2, $t2, $k0
lw $t2, 0($t2)
div $t1, $t2
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4

#if条件语句,源代码第48行
li $t1, 1
bne $t3, $t1, else8

#if条件语句,源代码第49行
li $t1, 0
bne $s2, $t1, else9
li $t2, 30
div $s3, $t2
mflo $t3 
li $t2, 30
mul $t3, $t3, $t2
subu $t3, $s3, $t3
sll $t3, $t3, 2
addiu $t0, $t3, 16
sw $t0, 692($k1)
lw $t2, 692($k1)
addu $t2, $t2, $k0
lw $t2, 0($t2)
subu $t3, $s0, $t2
move $s0, $t3
else9: #if退出标签，源代码第51行
else8: #if退出标签，源代码第51行
else5: #if退出标签，源代码第53行
li $t2, 30
div $s3, $t2
mflo $t3 
sll $t3, $t3, 2
addiu $t0, $t3, 0
sw $t0, 712($k1)
li $t2, 30
div $s3, $t2
mflo $t3 
sll $t3, $t3, 2
addiu $t0, $t3, 0
sw $t0, 732($k1)
lw $t1, 732($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
addu $t3, $t1, $s0
lw $t0, 712($k1)
addu $t0, $t0, $gp
sw $t3, 0($t0)
#函数返回值为: 
li $a0, 0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
rand: 
#保存返回地址: 
sw $ra, 4($k0)
lw $t1, 4012($gp)
lw $t2, 4000($gp)
mul $t3, $t1, $t2
lw $t2, 4004($gp)
addu $t3, $t3, $t2
sw $t3, 4012($gp)
lw $t1, 4012($gp)
lw $t2, 4008($gp)
div $t1, $t2
mflo $t3 
lw $t2, 4008($gp)
mul $t3, $t3, $t2
lw $t1, 4012($gp)
subu $t3, $t1, $t3
sw $t3, 4012($gp)

#if条件语句,源代码第61行
lw $t0, 4012($gp)
li $t1, 0
bge $t0, $t1, else10
lw $t1, 4008($gp)
lw $t2, 4012($gp)
addu $t3, $t1, $t2
sw $t3, 4012($gp)
else10: #if退出标签，源代码第62行
#函数返回值为: 
lw $a0, 4012($gp)
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
main: 
addiu $s4, $0, 0
addiu $s3, $0, 0
addiu $s0, $0, 0
addiu $s0, $0, 0
addiu $s2, $0, 0
addiu $s1, $0, 0
addiu $s0, $0, 0
while_b11: 

#while语句,在源代码第72行
li $t1, 1000
bge $s0, $t1, while_end13
sll $t3, $s0, 2
addiu $t0, $t3, 0
sw $t0, 828($k1)
li $t1, 0
lw $t0, 828($k1)
addu $t0, $t0, $gp
sw $t1, 0($t0)
addiu $t3, $s0, 1
move $s0, $t3
j while_b11
while_end13: 
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s1, -4($sp)
sw $s2, -8($sp)
addu $sp,$sp,-8
jal myscanf
lw $s1, 4($sp)
lw $s2, 0($sp)
addu $sp,$sp,8
#取得有返回值函数调用结果,在源代码第76行
sw $a0, 848($k1)
lw $t1, 848($k1)
move $s4, $t1
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s1, -4($sp)
sw $s2, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal myscanf
lw $s1, 8($sp)
lw $s2, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
#取得有返回值函数调用结果,在源代码第77行
sw $a0, 856($k1)
lw $t1, 856($k1)
sw $t1, 4012($gp)
while_b14: 

#while语句,在源代码第78行
li $t1, 0
ble $s4, $t1, while_end16
addiu $t3, $s4, -1
move $s4, $t3
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s1, -4($sp)
sw $s2, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal rand
lw $s1, 8($sp)
lw $s2, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
#取得有返回值函数调用结果,在源代码第80行
sw $a0, 880($k1)
lw $t1, 880($k1)
move $s0, $t1
li $t2, 300000
div $s0, $t2
mflo $t3 
li $t2, 300000
mul $t3, $t3, $t2
subu $t3, $s0, $t3
move $s3, $t3
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s1, -4($sp)
sw $s2, -8($sp)
sw $s3, -12($sp)
sw $s4, -16($sp)
addu $sp,$sp,-16
jal rand
lw $s1, 12($sp)
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,16
#取得有返回值函数调用结果,在源代码第82行
sw $a0, 904($k1)
lw $t1, 904($k1)
move $s0, $t1
li $t2, 2
div $s0, $t2
mflo $t3 
sll $t3, $t3, 1
subu $t3, $s0, $t3
move $s0, $t3
sw $s3, 36($k0)
sw $s0, 40($k0)
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s1, -4($sp)
sw $s2, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal set
lw $s1, 8($sp)
lw $s2, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
j while_b14
while_end16: 
while_b17: 

#while语句,在源代码第86行
li $t1, 1000
bge $s1, $t1, while_end19
sll $t3, $s1, 2
addiu $t0, $t3, 0
sw $t0, 956($k1)
sll $t3, $s1, 2
addiu $t0, $t3, 0
sw $t0, 968($k1)
lw $t1, 968($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
li $t2, 100000
div $t1, $t2
mflo $t3 
li $t2, 100000
mul $t3, $t3, $t2
lw $t1, 956($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
subu $t3, $t1, $t3
addu $t3, $s2, $t3
move $s2, $t3
li $t2, 100000
div $s2, $t2
mflo $t3 
li $t2, 100000
mul $t3, $t3, $t2
subu $t3, $s2, $t3
move $s2, $t3
addiu $t3, $s1, 1
move $s1, $t3
j while_b17
while_end19: 
sw $s2, 36($k0)
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
addu $sp,$sp,0
jal myprintf
addu $sp,$sp,0
#主函数退出: 
li $v0, 10
syscall
#主函数退出: 
li $v0, 10
syscall
