.data
thisisaenter: .asciiz "\n"
.text
#�����洢��׼:
#$gp
li $k0, 269549568
li $k1, 270598144
##############start############## 

#ȫ�ֱ�����ʼ��:

#��ת��������:
j main
myscanf: 
#���淵�ص�ַ: 
sw $ra, 4($k0)
addiu $s0, $0, 0
li $v0, 5
syscall
move $s0, $v0
#��������ֵΪ: 
move $a0, $s0
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
myprintf: 
#���淵�ص�ַ: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s0,$t1
#д���,��Դ�����17��
li $v0, 1
move $a0, $s0
syscall
li $v0, 4
la $a0, thisisaenter
syscall
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
checkrange: 
#���淵�ص�ַ: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s0,$t1
while_b1: 

#while���,��Դ�����23��
li $t1, 1073741824
ble $s0, $t1, while_end3
addiu $t3, $s0, -1073741824
move $s0, $t3
j while_b1
while_end3: 
while_b4: 

#while���,��Դ�����25��
li $t1, 0
bge $s0, $t1, while_end6
addiu $t3, $s0, 1073741824
move $s0, $t3
j while_b4
while_end6: 
#��������ֵΪ: 
move $a0, $s0
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
reduce: 
#���淵�ص�ַ: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s4,$t1
lw $t1, 12($k0)
move $s3,$t1
lw $t1, 16($k0)
move $s2,$t1
addiu $s1, $0, 0
addiu $s0, $0, 0

#if�������,Դ�����33��
li $t1, 0
bne $s4, $t1, else7
addu $t3, $s3, $s2
sw $t3, 36($k0)
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
sw $s2, -4($sp)
sw $s3, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal checkrange
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
#ȡ���з���ֵ�������ý��,��Դ�����34��
sw $a0, 64($k1)
#��������ֵΪ: 
lw $a0, 64($k1)
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
else7: #if�˳���ǩ��Դ�����35��

#if�������,Դ�����35��
li $t1, 1
bne $s4, $t1, else8
li $t1, 0
move $s1, $t1
li $t1, 1
move $s0, $t1
while_b9: 

#while���,��Դ�����39��
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

#if�������,Դ�����41��
bne $t3, $t4, else12
sll $t3, $s1, 1
move $s1, $t3
j end13
else12: #else��ǩ��Դ�����43��
sll $t3, $s1, 1
addiu $t3, $t3, 1
move $s1, $t3
end13: 
sll $t3, $s0, 1
move $s0, $t3
j while_b9
while_end11: 
#��������ֵΪ: 
move $a0, $s1
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
else8: #if�˳���ǩ��Դ�����49��

#if�������,Դ�����49��
li $t1, 2
bne $s4, $t1, else14

#if�������,Դ�����51��
ble $s3, $s2, else15
#��������ֵΪ: 
move $a0, $s3
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
j end16
else15: #else��ǩ��Դ�����53��
#��������ֵΪ: 
move $a0, $s2
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
end16: 
else14: #if�˳���ǩ��Դ�����56��

#if�������,Դ�����56��
li $t1, 3
bne $s4, $t1, else17
li $t1, 0
move $s1, $t1
li $t1, 1
move $s0, $t1
while_b18: 

#while���,��Դ�����60��
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

#if�������,Դ�����62��
li $t1, 1
bne $t3, $t1, else21
sll $t3, $s1, 1
addiu $t3, $t3, 1
move $s1, $t3
j end22
else21: #else��ǩ��Դ�����64��
div $s2, $s0
mflo $t3 
div $s2, $s0
mflo $t4 
li $t2, 2
div $t4, $t2
mflo $t4 
sll $t4, $t4, 1
subu $t3, $t3, $t4

#if�������,Դ�����66��
li $t1, 1
bne $t3, $t1, else23
sll $t3, $s1, 1
addiu $t3, $t3, 1
move $s1, $t3
else23: #if�˳���ǩ��Դ�����70��
sll $t3, $s1, 1
move $s1, $t3
end22: 
sll $t3, $s0, 1
move $s0, $t3
j while_b18
while_end20: 
#��������ֵΪ: 
move $a0, $s1
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
else17: #if�˳���ǩ��Դ�����76��

#if�������,Դ�����76��
li $t1, 4
bne $s4, $t1, else24
li $t1, 0
move $s1, $t1
li $t1, 1
move $s0, $t1
while_b25: 

#while���,��Դ�����80��
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

#if�������,Դ�����82��
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

#if�������,Դ�����84��
li $t1, 1
bne $t3, $t1, else29
sll $t3, $s1, 1
addiu $t3, $t3, 1
move $s1, $t3
j end30
else29: #else��ǩ��Դ�����86��
sll $t3, $s1, 1
move $s1, $t3
end30: 
j end31
else28: #else��ǩ��Դ�����89��
sll $t3, $s1, 1
move $s1, $t3
end31: 
sll $t3, $s0, 1
move $s0, $t3
j while_b25
while_end27: 
#��������ֵΪ: 
move $a0, $s1
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
else24: #if�˳���ǩ��Դ�����95��
#��������ֵΪ: 
li $a0, 0
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
getvalue: 
#���淵�ص�ַ: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s3,$t1
lw $t1, 12($k0)
move $s2,$t1
lw $t1, 16($k0)
move $s1,$t1
lw $t1, 20($k0)
move $s0,$t1

#if�������,Դ�����100��
li $t1, 0
bge $s1, $t1, else32
#��������ֵΪ: 
li $a0, 0
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
else32: #if�˳���ǩ��Դ�����102��

#if�������,Դ�����102��
li $t1, 0
bge $s0, $t1, else33
#��������ֵΪ: 
li $a0, 0
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
else33: #if�˳���ǩ��Դ�����104��

#if�������,Դ�����104��
blt $s1, $s3, else34
#��������ֵΪ: 
li $a0, 0
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
else34: #if�˳���ǩ��Դ�����106��

#if�������,Դ�����106��
blt $s0, $s2, else35
#��������ֵΪ: 
li $a0, 0
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
else35: #if�˳���ǩ��Դ�����108��
mul $t3, $s1, $s2
addu $t3, $t3, $s0
sll $t3, $t3, 2
addiu $t0, $t3, 0
sw $t0, 584($k1)
#��������ֵΪ: 
lw $a0, 584($k1)
addu $a0, $a0, $gp
lw $a0, 0($a0)
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
convn: 
#���淵�ص�ַ: 
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

#while���,��Դ�����120��
li $t1, 1
bne $s3, $t1, while_end38
li $t1, 0
move $s7, $t1
while_b39: 

#while���,��Դ�����123��
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

#while���,��Դ�����127��
li $t1, 1
bne $s1, $t1, while_end44
lw $t1, 20($k0)
li $t2, 2
div $t1, $t2
mflo $t3 
subu $t3, $s7, $t3
move $s5, $t3
while_b45: 

#while���,��Դ�����130��
li $t1, 1
bne $s0, $t1, while_end47
lw $t1, 12($k0)
sw $t1, 68($k0)
lw $t1, 16($k0)
sw $t1, 72($k0)
sw $s6, 76($k0)
sw $s5, 80($k0)
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 60
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 60
sw $t0, 0($k0)
#��ת����
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
#ȡ���з���ֵ�������ý��,��Դ�����132��
sw $a0, 684($k1)
lw $t1, 8($k0)
sw $t1, 68($k0)
sw $s4, 72($k0)
lw $t1, 684($k1)
sw $t1, 76($k0)
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 60
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 60
sw $t0, 0($k0)
#��ת����
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
#ȡ���з���ֵ�������ý��,��Դ�����132��
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

#if�������,Դ�����134��
blt $s5, $t3, else48
li $t1, 0
move $s0, $t1
else48: #if�˳���ǩ��Դ�����138��
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

#if�������,Դ�����141��
blt $s6, $t3, else49
li $t1, 0
move $s1, $t1
else49: #if�˳���ǩ��Դ�����145��
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

#if�������,Դ�����149��
lw $t1, 16($k0)
blt $s7, $t1, else50
li $t1, 0
move $s2, $t1
else50: #if�˳���ǩ��Դ�����153��
j while_b39
while_end41: 
li $t1, 1
move $s2, $t1
lw $t1, 24($k0)
addiu $t3, $t1, 1
sw $t3, 24($k0)

#if�������,Դ�����156��
lw $t0, 24($k0)
lw $t1, 12($k0)
blt $t0, $t1, else51
li $t1, 0
move $s3, $t1
else51: #if�˳���ǩ��Դ�����160��
j while_b36
while_end38: 
#��������ֵΪ: 
li $a0, 0
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
mymemmove: 
#���淵�ص�ַ: 
sw $ra, 4($k0)
lw $t1, 8($k0)
move $s1,$t1
addiu $s0, $0, 0
while_b52: 

#while���,��Դ�����167��
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
#�Ӻ����˳���,����ռ䲢׼������... 
lw $t0, 0($k0)
lw $ra, 4($k0)
subu $k0, $k0, $t0
subu $k1, $k1, 10000 #��ֵ���ʱ�ռ��С
jr $ra
main: 
addiu $s4, $0, 0
addiu $s3, $0, 0
addiu $s2, $0, 0
addiu $s1, $0, 0
addiu $s1, $0, 0
addiu $s0, $0, 0
addiu $s0, $0, 0
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
sw $s0, -4($sp)
addu $sp,$sp,-4
jal myscanf
lw $s0, 0($sp)
addu $sp,$sp,4
#ȡ���з���ֵ�������ý��,��Դ�����178��
sw $a0, 876($k1)
lw $t1, 876($k1)
move $s4, $t1
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
sw $s0, -4($sp)
sw $s4, -8($sp)
addu $sp,$sp,-8
jal myscanf
lw $s0, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,8
#ȡ���з���ֵ�������ý��,��Դ�����179��
sw $a0, 884($k1)
lw $t1, 884($k1)
move $s3, $t1
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
sw $s0, -4($sp)
sw $s3, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal myscanf
lw $s0, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
#ȡ���з���ֵ�������ý��,��Դ�����180��
sw $a0, 892($k1)
lw $t1, 892($k1)
move $s2, $t1
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
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
#ȡ���з���ֵ�������ý��,��Դ�����181��
sw $a0, 900($k1)
lw $t1, 900($k1)
move $s1, $t1
while_b55: 

#while���,��Դ�����182��
bge $s0, $s1, while_end57
sll $t3, $s0, 2
addiu $t0, $t3, 0
sw $t0, 920($k1)
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
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
#ȡ���з���ֵ�������ý��,��Դ�����184��
sw $a0, 928($k1)
lw $t1, 928($k1)
lw $t0, 920($k1)
addu $t0, $t0, $gp
sw $t1, 0($t0)
addiu $t3, $s0, 1
move $s0, $t3
j while_b55
while_end57: 
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
sw $s2, -4($sp)
sw $s3, -8($sp)
sw $s4, -12($sp)
addu $sp,$sp,-12
jal myscanf
lw $s2, 8($sp)
lw $s3, 4($sp)
lw $s4, 0($sp)
addu $sp,$sp,12
#ȡ���з���ֵ�������ý��,��Դ�����187��
sw $a0, 944($k1)
lw $t1, 944($k1)
move $s1, $t1
li $t1, 0
move $s0, $t1
while_b58: 

#while���,��Դ�����189��
bge $s0, $s1, while_end60
sll $t3, $s0, 2
addiu $t0, $t3, 8000
sw $t0, 968($k1)
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
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
#ȡ���з���ֵ�������ý��,��Դ�����191��
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

#while���,��Դ�����196��
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
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
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
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
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

#while���,��Դ�����204��
bge $s0, $t3, while_end66
sll $t3, $s0, 2
addiu $t0, $t3, 0
sw $t0, 1072($k1)
lw $t1, 1072($k1)
addu $t1, $t1, $gp
lw $t1, 0($t1)
sw $t1, 36($k0)
#׼�����뺯��,���ڷ���ռ�...
addu $k0, $k0, 28
addu $k1, $k1, 10000 #��ֵ���ʱ�ռ����
li $t0, 28
sw $t0, 0($k0)
#��ת����
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
#�������˳�: 
li $v0, 10
syscall
#�������˳�: 
li $v0, 10
syscall
