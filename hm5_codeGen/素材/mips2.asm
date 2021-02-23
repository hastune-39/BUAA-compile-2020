.data
thisisaenter: .asciiz "\n"
.text
#变量存储基准:
#$gp
li $k0, 269549568
li $k1, 270598144
##############start############## 

#全局变量初始化:

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
#写语句,在源代码第17行
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
checkrange: 
#保存返回地址: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s0,$t1
while_b1: 

#while语句,在源代码第23行
li $t1, 1073741824
ble $s0, $t1, while_end3
addiu $t3, $s0, -1073741824
move $s0, $t3
j while_b1
while_end3: 
while_b4: 

#while语句,在源代码第25行
li $t1, 0
bge $s0, $t1, while_end6
addiu $t3, $s0, 1073741824
move $s0, $t3
j while_b4
while_end6: 
#函数返回值为: 
move $a0, $s0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
reduce: 
#保存返回地址: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s4,$t1
lw $t1, 12($k0)
move $s3,$t1
lw $t1, 16($k0)
move $s2,$t1
addiu $s1, $0, 0
addiu $s0, $0, 0

#if条件语句,源代码第33行
li $t1, 0
bne $s4, $t1, else7
addu $t3, $s3, $s2
sw $t3, 36($k0)
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s2, -4($sp)
sw $s3, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal checkrange
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
#取得有返回值函数调用结果,在源代码第34行
sw $a0, 64($k1)
#函数返回值为: 
lw $a0, 64($k1)
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else7: #if退出标签，源代码第35行

#if条件语句,源代码第35行
li $t1, 1
bne $s4, $t1, else8
li $t1, 0
move $s1, $t1
li $t1, 1
move $s0, $t1
while_b9: 

#while语句,在源代码第39行
li $t1, 1073741824
bge $s0, $t1, while_end11
div $s3, $s0
mflo $t3 
div $s3, $s0
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4
div $s2, $s0
mflo $t4 
div $s2, $s0
mflo $t5 
li $t2, 2
div $t5, $t2
mflo $t5 
sll $t5, $t5, 1
subu $t4, $t4, $t5

#if条件语句,源代码第41行
bne $t3, $t4, else12
sll $t3, $s1, 1
move $s1, $t3
j end13
else12: #else标签，源代码第43行
sll $t3, $s1, 1
addiu $t3, $t3, 1
move $s1, $t3
end13: 
sll $t3, $s0, 1
move $s0, $t3
j while_b9
while_end11: 
#函数返回值为: 
move $a0, $s1
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else8: #if退出标签，源代码第49行

#if条件语句,源代码第49行
li $t1, 2
bne $s4, $t1, else14

#if条件语句,源代码第51行
ble $s3, $s2, else15
#函数返回值为: 
move $a0, $s3
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
j end16
else15: #else标签，源代码第53行
#函数返回值为: 
move $a0, $s2
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
end16: 
else14: #if退出标签，源代码第56行

#if条件语句,源代码第56行
li $t1, 3
bne $s4, $t1, else17
li $t1, 0
move $s1, $t1
li $t1, 1
move $s0, $t1
while_b18: 

#while语句,在源代码第60行
li $t1, 1073741824
bge $s0, $t1, while_end20
div $s3, $s0
mflo $t3 
div $s3, $s0
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4

#if条件语句,源代码第62行
li $t1, 1
bne $t3, $t1, else21
sll $t3, $s1, 1
addiu $t3, $t3, 1
move $s1, $t3
j end22
else21: #else标签，源代码第64行
div $s2, $s0
mflo $t3 
div $s2, $s0
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4

#if条件语句,源代码第66行
li $t1, 1
bne $t3, $t1, else23
sll $t3, $s1, 1
addiu $t3, $t3, 1
move $s1, $t3
else23: #if退出标签，源代码第70行
sll $t3, $s1, 1
move $s1, $t3
end22: 
sll $t3, $s0, 1
move $s0, $t3
j while_b18
while_end20: 
#函数返回值为: 
move $a0, $s1
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else17: #if退出标签，源代码第76行

#if条件语句,源代码第76行
li $t1, 4
bne $s4, $t1, else24
li $t1, 0
move $s1, $t1
li $t1, 1
move $s0, $t1
while_b25: 

#while语句,在源代码第80行
li $t1, 1073741824
bge $s0, $t1, while_end27
div $s3, $s0
mflo $t3 
div $s3, $s0
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4

#if条件语句,源代码第82行
li $t1, 1
bne $t3, $t1, else28
div $s2, $s0
mflo $t3 
div $s2, $s0
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4

#if条件语句,源代码第84行
li $t1, 1
bne $t3, $t1, else29
sll $t3, $s1, 1
addiu $t3, $t3, 1
move $s1, $t3
j end30
else29: #else标签，源代码第86行
sll $t3, $s1, 1
move $s1, $t3
end30: 
j end31
else28: #else标签，源代码第89行
sll $t3, $s1, 1
move $s1, $t3
end31: 
sll $t3, $s0, 1
move $s0, $t3
j while_b25
while_end27: 
#函数返回值为: 
move $a0, $s1
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else24: #if退出标签，源代码第95行
#函数返回值为: 
li $a0, 0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
getvalue: 
#保存返回地址: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s3,$t1
lw $t1, 12($k0)
move $s2,$t1
lw $t1, 16($k0)
move $s1,$t1
lw $t1, 20($k0)
move $s0,$t1

#if条件语句,源代码第100行
li $t1, 0
bge $s1, $t1, else32
#函数返回值为: 
li $a0, 0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else32: #if退出标签，源代码第102行

#if条件语句,源代码第102行
li $t1, 0
bge $s0, $t1, else33
#函数返回值为: 
li $a0, 0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else33: #if退出标签，源代码第104行

#if条件语句,源代码第104行
blt $s1, $s3, else34
#函数返回值为: 
li $a0, 0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else34: #if退出标签，源代码第106行

#if条件语句,源代码第106行
blt $s0, $s2, else35
#函数返回值为: 
li $a0, 0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
else35: #if退出标签，源代码第108行
mul $t3, $s1, $s2
addu $t3, $t3, $s0
sll $t3, $t3, 2
addiu $t0, $t3, 0
sw $t0, 584($k1)
#函数返回值为: 
lw $a0, 584($k1)
addu $a0, $a0, $gp
lw $a0, 0($a0)
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
convn: 
#保存返回地址: 
sw $ra, 4($k0)
lw $t1, 8($k0)
sw $t1, 8($k0)
lw $t1, 12($k0)
sw $t1, 12($k0)
lw $t1, 16($k0)
sw $t1, 16($k0)
lw $t1, 20($k0)
sw $t1, 20($k0)
li $t1, 0
sw $t1, 24($k0)
addiu $s7, $0, 0
addiu $s6, $0, 0
addiu $s5, $0, 0
addiu $s4, $0, 0
addiu $s3, $0, 1
addiu $s2, $0, 1
addiu $s1, $0, 1
addiu $s0, $0, 1
while_b36: 

#while语句,在源代码第120行
li $t1, 1
bne $s3, $t1, while_end38
li $t1, 0
move $s7, $t1
while_b39: 

#while语句,在源代码第123行
li $t1, 1
bne $s2, $t1, while_end41
li $t1, 0
move $s4, $t1
lw $t1, 20($k0)
li $t2, 2
div $t1, $t2
mflo $t3 
lw $t1, 24($k0)
subu $t3, $t1, $t3
move $s6, $t3
while_b42: 

#while语句,在源代码第127行
li $t1, 1
bne $s1, $t1, while_end44
lw $t1, 20($k0)
li $t2, 2
div $t1, $t2
mflo $t3 
subu $t3, $s7, $t3
move $s5, $t3
while_b45: 

#while语句,在源代码第130行
li $t1, 1
bne $s0, $t1, while_end47
lw $t1, 12($k0)
sw $t1, 68($k0)
lw $t1, 16($k0)
sw $t1, 72($k0)
sw $s6, 76($k0)
sw $s5, 80($k0)
#准备进入函数,正在分配空间...
addu $k0, $k0, 60
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 60
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s1, -8($sp)
sw $s2, -12($sp)
sw $s3, -16($sp)
sw $s4, -20($sp)
sw $s5, -24($sp)
sw $s6, -28($sp)
sw $s7, -32($sp)
addu $sp,$sp,-32
jal getvalue
lw $s0, 28($sp)
lw $s1, 24($sp)
lw $s2, 20($sp)
lw $s3, 16($sp)
lw $s4, 12($sp)
lw $s5, 8($sp)
lw $s6, 4($sp)
lw $s7, 0($sp)
addu $sp,$sp,32
#取得有返回值函数调用结果,在源代码第132行
sw $a0, 684($k1)
lw $t1, 8($k0)
sw $t1, 68($k0)
sw $s4, 72($k0)
lw $t1, 684($k1)
sw $t1, 76($k0)
#准备进入函数,正在分配空间...
addu $k0, $k0, 60
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 60
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s1, -8($sp)
sw $s2, -12($sp)
sw $s3, -16($sp)
sw $s5, -20($sp)
sw $s6, -24($sp)
sw $s7, -28($sp)
addu $sp,$sp,-28
jal reduce
lw $s0, 24($sp)
lw $s1, 20($sp)
lw $s2, 16($sp)
lw $s3, 12($sp)
lw $s5, 8($sp)
lw $s6, 4($sp)
lw $s7, 0($sp)
addu $sp,$sp,28
#取得有返回值函数调用结果,在源代码第132行
sw $a0, 688($k1)
lw $t1, 688($k1)
move $s4, $t1
addiu $t3, $s5, 1
move $s5, $t3
lw $t1, 20($k0)
li $t2, 2
div $t1, $t2
mflo $t3 
addu $t3, $s7, $t3

#if条件语句,源代码第134行
blt $s5, $t3, else48
li $t1, 0
move $s0, $t1
else48: #if退出标签，源代码第138行
j while_b45
while_end47: 
li $t1, 1
move $s0, $t1
addiu $t3, $s6, 1
move $s6, $t3
lw $t1, 20($k0)
li $t2, 2
div $t1, $t2
mflo $t3 
lw $t1, 24($k0)
addu $t3, $t1, $t3

#if条件语句,源代码第141行
blt $s6, $t3, else49
li $t1, 0
move $s1, $t1
else49: #if退出标签，源代码第145行
j while_b42
while_end44: 
li $t1, 1
move $s1, $t1
lw $t1, 24($k0)
lw $t2, 16($k0)
mul $t3, $t1, $t2
addu $t3, $t3, $s7
sll $t3, $t3, 2
addiu $t0, $t3, 4000
sw $t0, 772($k1)
lw $t0, 772($k1)
addu $t0, $t0, $gp
sw $s4, 0($t0)
addiu $t3, $s7, 1
move $s7, $t3

#if条件语句,源代码第149行
lw $t1, 16($k0)
blt $s7, $t1, else50
li $t1, 0
move $s2, $t1
else50: #if退出标签，源代码第153行
j while_b39
while_end41: 
li $t1, 1
move $s2, $t1
lw $t1, 24($k0)
addiu $t3, $t1, 1
sw $t3, 24($k0)

#if条件语句,源代码第156行
lw $t0, 24($k0)
lw $t1, 12($k0)
blt $t0, $t1, else51
li $t1, 0
move $s3, $t1
else51: #if退出标签，源代码第160行
j while_b36
while_end38: 
#函数返回值为: 
li $a0, 0
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
mymemmove: 
#保存返回地址: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s1,$t1
addiu $s0, $0, 0
while_b52: 

#while语句,在源代码第167行
bge $s0, $s1, while_end54
sll $t3, $s0, 2
addiu $t0, $t3, 0
sw $t0, 844($k1)
sll $t3, $s0, 2
addiu $t0, $t3, 4000
sw $t0, 860($k1)
lw $t1, 860($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
lw $t0, 844($k1)
addu $t0, $t0, $gp
sw $t1, 0($t0)
addiu $t3, $s0, 1
move $s0, $t3
j while_b52
while_end54: 
#从函数退出中,清除空间并准备跳回... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #奇怪的临时空间减小
jr $ra
main: 
addiu $s4, $0, 0
addiu $s3, $0, 0
addiu $s2, $0, 0
addiu $s1, $0, 0
addiu $s1, $0, 0
addiu $s0, $0, 0
addiu $s0, $0, 0
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
addu $sp,$sp,-4
jal myscanf
lw $s0, 0($sp)
addu $sp,$sp,4
#取得有返回值函数调用结果,在源代码第178行
sw $a0, 876($k1)
lw $t1, 876($k1)
move $s4, $t1
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s4, -8($sp)
addu $sp,$sp,-8
jal myscanf
lw $s0, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,8
#取得有返回值函数调用结果,在源代码第179行
sw $a0, 884($k1)
lw $t1, 884($k1)
move $s3, $t1
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s3, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal myscanf
lw $s0, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
#取得有返回值函数调用结果,在源代码第180行
sw $a0, 892($k1)
lw $t1, 892($k1)
move $s2, $t1
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s2, -8($sp)
sw $s3, -12($sp)
sw $s4, -16($sp)
addu $sp,$sp,-16
jal myscanf
lw $s0, 12($sp)
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,16
#取得有返回值函数调用结果,在源代码第181行
sw $a0, 900($k1)
lw $t1, 900($k1)
move $s1, $t1
while_b55: 

#while语句,在源代码第182行
bge $s0, $s1, while_end57
sll $t3, $s0, 2
addiu $t0, $t3, 0
sw $t0, 920($k1)
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s1, -8($sp)
sw $s2, -12($sp)
sw $s3, -16($sp)
sw $s4, -20($sp)
addu $sp,$sp,-20
jal myscanf
lw $s0, 16($sp)
lw $s1, 12($sp)
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,20
#取得有返回值函数调用结果,在源代码第184行
sw $a0, 928($k1)
lw $t1, 928($k1)
lw $t0, 920($k1)
addu $t0, $t0, $gp
sw $t1, 0($t0)
addiu $t3, $s0, 1
move $s0, $t3
j while_b55
while_end57: 
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s2, -4($sp)
sw $s3, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal myscanf
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
#取得有返回值函数调用结果,在源代码第187行
sw $a0, 944($k1)
lw $t1, 944($k1)
move $s1, $t1
li $t1, 0
move $s0, $t1
while_b58: 

#while语句,在源代码第189行
bge $s0, $s1, while_end60
sll $t3, $s0, 2
addiu $t0, $t3, 8000
sw $t0, 968($k1)
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s1, -8($sp)
sw $s2, -12($sp)
sw $s3, -16($sp)
sw $s4, -20($sp)
addu $sp,$sp,-20
jal myscanf
lw $s0, 16($sp)
lw $s1, 12($sp)
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,20
#取得有返回值函数调用结果,在源代码第191行
sw $a0, 976($k1)
lw $t1, 976($k1)
lw $t0, 968($k1)
addu $t0, $t0, $gp
sw $t1, 0($t0)
addiu $t3, $s0, 1
move $s0, $t3
j while_b58
while_end60: 
li $t1, 0
move $s0, $t1
while_b61: 

#while语句,在源代码第196行
bge $s0, $s1, while_end63
sll $t3, $s0, 2
addiu $t0, $t3, 8000
sw $t0, 1012($k1)
lw $t1, 1012($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
sw $t1, 36($k0)
sw $s3, 40($k0)
sw $s2, 44($k0)
sw $s4, 48($k0)
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s1, -8($sp)
sw $s2, -12($sp)
sw $s3, -16($sp)
sw $s4, -20($sp)
addu $sp,$sp,-20
jal convn
lw $s0, 16($sp)
lw $s1, 12($sp)
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,20
mul $t3, $s3, $s2
sw $t3, 36($k0)
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s1, -8($sp)
sw $s2, -12($sp)
sw $s3, -16($sp)
sw $s4, -20($sp)
addu $sp,$sp,-20
jal mymemmove
lw $s0, 16($sp)
lw $s1, 12($sp)
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,20
addiu $t3, $s0, 1
move $s0, $t3
j while_b61
while_end63: 
li $t1, 0
move $s0, $t1
while_b64: 
mul $t3, $s3, $s2

#while语句,在源代码第204行
bge $s0, $t3, while_end66
sll $t3, $s0, 2
addiu $t0, $t3, 0
sw $t0, 1072($k1)
lw $t1, 1072($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
sw $t1, 36($k0)
#准备进入函数,正在分配空间...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #奇怪的临时空间分配
li $t0, 28
sw $t0, 0($k0)
#跳转函数
sw $s0, -4($sp)
sw $s2, -8($sp)
sw $s3, -12($sp)
addu $sp,$sp,-12
jal myprintf
lw $s0, 8($sp)
lw $s2, 4($sp)
lw $s3, 0($sp)
addu $sp,$sp,12
addiu $t3, $s0, 1
move $s0, $t3
j while_b64
while_end66: 
#主函数退出: 
li $v0, 10
syscall
#主函数退出: 
li $v0, 10
syscall
