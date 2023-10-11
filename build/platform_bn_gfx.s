
@{{BLOCK(platform_bn_gfx)

@=======================================================================
@
@	platform_bn_gfx, 32x16@4, 
@	+ palette 16 entries, not compressed
@	+ 8 tiles not compressed
@	Total size: 32 + 256 = 288
@
@	Time-stamp: 2023-09-13, 14:10:45
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global platform_bn_gfxTiles		@ 256 unsigned chars
	.hidden platform_bn_gfxTiles
platform_bn_gfxTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x40000000,0x45000000,0x45800000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x22222200,0x44444422,0x88888444,0x11111888
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00444422,0x44444444,0x44488888,0x88811111
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000054,0x00000854
	.word 0x88133000,0x13333330,0x13366330,0x33111633,0x73111133,0x17311330,0x17733770,0x66677000
	.word 0x11111111,0x11111111,0x77777771,0x33333371,0x33333371,0x33333371,0x11111111,0x66666666
	.word 0x11111111,0x11111111,0x17777777,0x13333333,0x13333333,0x13333333,0x11111111,0x66666666
	.word 0x00033188,0x03333331,0x03366331,0x33111633,0x73111133,0x07311331,0x07733771,0x00077666

	.section .rodata
	.align	2
	.global platform_bn_gfxPal		@ 32 unsigned chars
	.hidden platform_bn_gfxPal
platform_bn_gfxPal:
	.hword 0x7C1F,0x7FE0,0x7FFF,0x501F,0x3DEF,0x2108,0x7E60,0x4817
	.hword 0x7FF0,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(platform_bn_gfx)
