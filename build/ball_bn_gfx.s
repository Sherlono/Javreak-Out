
@{{BLOCK(ball_bn_gfx)

@=======================================================================
@
@	ball_bn_gfx, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2023-09-13, 17:02:37
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global ball_bn_gfxTiles		@ 32 unsigned chars
	.hidden ball_bn_gfxTiles
ball_bn_gfxTiles:
	.word 0x00022000,0x02211220,0x02111120,0x21111112,0x21111112,0x02111120,0x02211220,0x00022000

	.section .rodata
	.align	2
	.global ball_bn_gfxPal		@ 32 unsigned chars
	.hidden ball_bn_gfxPal
ball_bn_gfxPal:
	.hword 0x7C1F,0x7FFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(ball_bn_gfx)
