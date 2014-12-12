/* 
 * Copyleft (C) 2009
 * 	Rodrigo Rubira Branco (BSDaemon) 
 *	<rodrigo_branco *noSPAM* research.coseinc.com>
 *
 * This shellcode will create a syscall function inside the SPU
 * Since some really needed mneumonicts cannot avoid NULL-bytes, it must be used in a polymorphized way
 * Follow the website contained in the article for updates and an already polymorphized version
 *
 * It's needed to provide to other portions of the code support to not-linked in system calls
 *
 * Don't be lazy, use spu-as to test
 * 
*/
	.extern errno /* we need this to define errno globally */
	.global	replace_me

	.text
	.align 2

replace_me: /* replace with the name of the syscall to be used - common eg. open, close, write */
	ilhu	$2, 0 << 8		/* replace 0 with the syscall number */
	stqd	$3, -16*0-32($sp)	/* replace 0 with the number of parameters - this will get the first parameter */
	stqd	$4, -16*0-16($sp)	/* replace 0 with the number of parameters - this will get the second parameter */
	/* repeat the above number of parameters times - don't forget to adjust the sp (0($sp),16($sp),32($sp)...)*/
	lqr	$6, copy_message
	ai	$3, $sp, -16*(0+2)	/* replace 0 with the number of parameters */
	ila	$4, 0x3FFFF		/* to get the address */
	selb    $2, $2, $3, $4		/* to know the address & command */
	cwd	$5, 4($sp)
	shufb	$2, $2, $6, $5		/* align message */
	ai	$3, $sp, -32
	stqd	$2, -32($sp)
	sync

	/* To return something */
	bisl	$2, $3
	lqd     $3, -16*(0+2)($sp)	/* replace 0 with the number of parameters - return using register 3 as expected according the ABI */
	shlqbyi	$2, $3, 12		/* define errno */
	stqr	$2, errno		
	bi      $0			/* return */
	
	.balignl	16, 0
copy_message:
	stop	0x2101			/* Awake the PPE to execute the syscall */
	.word	0			/* to define the message */
	bi	$2			/* return */
