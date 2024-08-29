// Copyright

#pragma once

#include <cstdint>

#include "Block/Block.h"


struct BlockColor
{
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
};

extern const BlockColor BlockColors[];

#define BlockFromType(BlockType) (BlockType) 

// Natural Blocks
constexpr BlockType AIR = BlockFromType(0);
constexpr BlockType STONE = BlockFromType(1);
constexpr BlockType GRASS = BlockFromType(2);
constexpr BlockType DIRT = BlockFromType(3);
constexpr BlockType COBBLESTONE = BlockFromType(4);
constexpr BlockType SAND = BlockFromType(5);
constexpr BlockType GRAVEL = BlockFromType(6);
constexpr BlockType GOLD_ORE = BlockFromType(7);
constexpr BlockType IRON_ORE = BlockFromType(8);
constexpr BlockType COAL_ORE = BlockFromType(9);
constexpr BlockType LOG = BlockFromType(10);
constexpr BlockType LEAVES = BlockFromType(11);
constexpr BlockType BEDROCK = BlockFromType(12);
constexpr BlockType WATER = BlockFromType(13);
constexpr BlockType LAVA = BlockFromType(14);
constexpr BlockType SANDSTONE = BlockFromType(15);
constexpr BlockType CLAY = BlockFromType(16);
constexpr BlockType OBSIDIAN = BlockFromType(17);

// Plant Blocks
constexpr BlockType GRASS_BLOCK = BlockFromType(18);
constexpr BlockType TALL_GRASS = BlockFromType(19);
constexpr BlockType FERN = BlockFromType(20);
constexpr BlockType DEAD_BUSH = BlockFromType(21);
constexpr BlockType DANDELION = BlockFromType(22);
constexpr BlockType POPPY = BlockFromType(23);
constexpr BlockType BLUE_ORCHID = BlockFromType(24);
constexpr BlockType ALLIUM = BlockFromType(25);
constexpr BlockType AZURE_BLUET = BlockFromType(26);
constexpr BlockType RED_TULIP = BlockFromType(27);
constexpr BlockType ORANGE_TULIP = BlockFromType(28);
constexpr BlockType WHITE_TULIP = BlockFromType(29);
constexpr BlockType PINK_TULIP = BlockFromType(30);
constexpr BlockType OXEYE_DAISY = BlockFromType(31);
constexpr BlockType SUNFLOWER = BlockFromType(32);
constexpr BlockType LILAC = BlockFromType(33);
constexpr BlockType ROSE_BUSH = BlockFromType(34);
constexpr BlockType PEONY = BlockFromType(35);
constexpr BlockType LILY_PAD = BlockFromType(36);
constexpr BlockType CACTUS = BlockFromType(37);
constexpr BlockType SUGAR_CANE = BlockFromType(38);

// Manufactured Blocks
constexpr BlockType PLANKS = BlockFromType(39);
constexpr BlockType SANDSTONE_SMOOTH = BlockFromType(40);
constexpr BlockType BRICKS = BlockFromType(41);
constexpr BlockType STONE_BRICKS = BlockFromType(42);
constexpr BlockType STONE_BRICKS_MOSSY = BlockFromType(43);
constexpr BlockType STONE_BRICKS_CRACKED = BlockFromType(44);
constexpr BlockType STONE_BRICKS_CHISELED = BlockFromType(45);
constexpr BlockType STONE_SLAB = BlockFromType(46);
constexpr BlockType COBBLESTONE_SLAB = BlockFromType(47);
constexpr BlockType BRICK_SLAB = BlockFromType(48);
constexpr BlockType STONE_BRICK_SLAB = BlockFromType(49);
constexpr BlockType QUARTZ_SLAB = BlockFromType(50);
constexpr BlockType NETHER_BRICK_SLAB = BlockFromType(51);
constexpr BlockType NETHER_BRICK = BlockFromType(52);
constexpr BlockType QUARTZ_BLOCK = BlockFromType(53);
constexpr BlockType QUARTZ_BLOCK_CHISELED = BlockFromType(54);
constexpr BlockType QUARTZ_BLOCK_PILLAR = BlockFromType(55);
constexpr BlockType GLASS = BlockFromType(56);
constexpr BlockType GLOWSTONE = BlockFromType(57);
constexpr BlockType REDSTONE_BLOCK = BlockFromType(58);
constexpr BlockType IRON_BLOCK = BlockFromType(59);
constexpr BlockType GOLD_BLOCK = BlockFromType(60);
constexpr BlockType DIAMOND_BLOCK = BlockFromType(61);
constexpr BlockType EMERALD_BLOCK = BlockFromType(62);

// Utility Blocks
constexpr BlockType TORCH = BlockFromType(63);
constexpr BlockType FURNACE = BlockFromType(64);
constexpr BlockType CRAFTING_TABLE = BlockFromType(65);
constexpr BlockType CHEST = BlockFromType(66);
constexpr BlockType ENCHANTMENT_TABLE = BlockFromType(67);
constexpr BlockType ANVIL = BlockFromType(68);
constexpr BlockType BREWING_STAND = BlockFromType(69);
constexpr BlockType BEACON = BlockFromType(70);
constexpr BlockType ENDER_CHEST = BlockFromType(71);
constexpr BlockType DISPENSER = BlockFromType(72);
constexpr BlockType DROPPER = BlockFromType(73);
constexpr BlockType HOPPER = BlockFromType(74);

// Redstone Components
constexpr BlockType REDSTONE_WIRE = BlockFromType(75);
constexpr BlockType REDSTONE_TORCH = BlockFromType(76);
constexpr BlockType REDSTONE_REPEATER = BlockFromType(77);
constexpr BlockType REDSTONE_COMPARATOR = BlockFromType(78);
constexpr BlockType LEVER = BlockFromType(79);
constexpr BlockType BUTTON = BlockFromType(80);
constexpr BlockType PRESSURE_PLATE = BlockFromType(81);
constexpr BlockType PISTON = BlockFromType(82);
constexpr BlockType STICKY_PISTON = BlockFromType(83);
constexpr BlockType TRIPWIRE_HOOK = BlockFromType(84);
constexpr BlockType TRIPWIRE = BlockFromType(85);
constexpr BlockType OBSERVER = BlockFromType(86);

// Decorative Blocks
constexpr BlockType CARPET = BlockFromType(87);
constexpr BlockType WOOL = BlockFromType(88);
constexpr BlockType GLASS_PANE = BlockFromType(89);
constexpr BlockType IRON_BARS = BlockFromType(90);
constexpr BlockType STAINED_GLASS = BlockFromType(91);
constexpr BlockType STAINED_GLASS_PANE = BlockFromType(92);
constexpr BlockType BANNER = BlockFromType(93);
constexpr BlockType FLOWER_POT = BlockFromType(94);
constexpr BlockType PAINTING = BlockFromType(95);

// Miscellaneous
constexpr BlockType SPONGE = BlockFromType(96);
constexpr BlockType WEB = BlockFromType(97);
constexpr BlockType SNOW = BlockFromType(98);
constexpr BlockType ICE = BlockFromType(99);
constexpr BlockType PACKED_ICE = BlockFromType(100);
constexpr BlockType SEA_LANTERN = BlockFromType(101);
constexpr BlockType PRISMARINE = BlockFromType(102);
constexpr BlockType PRISMARINE_BRICKS = BlockFromType(103);
constexpr BlockType DARK_PRISMARINE = BlockFromType(104);
constexpr BlockType SHULKER_BOX = BlockFromType(105);
constexpr BlockType BARRIER = BlockFromType(106);
constexpr BlockType END_PORTAL_FRAME = BlockFromType(107);

// Nether & End Blocks
constexpr BlockType NETHERRACK = BlockFromType(108);
constexpr BlockType NETHER_QUARTZ_ORE = BlockFromType(109);
constexpr BlockType NETHER_BRICK_FENCE = BlockFromType(110);
constexpr BlockType NETHER_WART_BLOCK = BlockFromType(111);
constexpr BlockType MAGMA_BLOCK = BlockFromType(112);
constexpr BlockType END_STONE = BlockFromType(113);
constexpr BlockType END_STONE_BRICKS = BlockFromType(114);
constexpr BlockType PURPUR_BLOCK = BlockFromType(115);
constexpr BlockType PURPUR_PILLAR = BlockFromType(116);
constexpr BlockType PURPUR_STAIRS = BlockFromType(117);
constexpr BlockType END_ROD = BlockFromType(118);

// Logs & Leaves Variants
constexpr BlockType OAK_LOG = BlockFromType(119);
constexpr BlockType SPRUCE_LOG = BlockFromType(120);
constexpr BlockType BIRCH_LOG = BlockFromType(121);
constexpr BlockType JUNGLE_LOG = BlockFromType(122);
constexpr BlockType ACACIA_LOG = BlockFromType(123);
constexpr BlockType DARK_OAK_LOG = BlockFromType(124);
constexpr BlockType OAK_LEAVES = BlockFromType(125);
constexpr BlockType SPRUCE_LEAVES = BlockFromType(126);
constexpr BlockType BIRCH_LEAVES = BlockFromType(127);
constexpr BlockType JUNGLE_LEAVES = BlockFromType(128);
constexpr BlockType ACACIA_LEAVES = BlockFromType(129);
constexpr BlockType DARK_OAK_LEAVES = BlockFromType(130);

// Fences & Gates
constexpr BlockType OAK_FENCE = BlockFromType(131);
constexpr BlockType SPRUCE_FENCE = BlockFromType(132);
constexpr BlockType BIRCH_FENCE = BlockFromType(133);
constexpr BlockType JUNGLE_FENCE = BlockFromType(134);
constexpr BlockType ACACIA_FENCE = BlockFromType(135);
constexpr BlockType DARK_OAK_FENCE = BlockFromType(136);
constexpr BlockType NETHER_BRICK_FENCE_REDEFINITION = BlockFromType(137);
constexpr BlockType OAK_FENCE_GATE = BlockFromType(138);
constexpr BlockType SPRUCE_FENCE_GATE = BlockFromType(139);
constexpr BlockType BIRCH_FENCE_GATE = BlockFromType(140);
constexpr BlockType JUNGLE_FENCE_GATE = BlockFromType(141);
constexpr BlockType ACACIA_FENCE_GATE = BlockFromType(142);
constexpr BlockType DARK_OAK_FENCE_GATE

 = BlockFromType(143);

// Stairs
constexpr BlockType OAK_STAIRS = BlockFromType(144);
constexpr BlockType COBBLESTONE_STAIRS = BlockFromType(145);
constexpr BlockType BRICK_STAIRS = BlockFromType(146);
constexpr BlockType STONE_BRICK_STAIRS = BlockFromType(147);
constexpr BlockType NETHER_BRICK_STAIRS = BlockFromType(148);
constexpr BlockType SANDSTONE_STAIRS = BlockFromType(149);
constexpr BlockType QUARTZ_STAIRS = BlockFromType(150);
constexpr BlockType SPRUCE_STAIRS = BlockFromType(151);
constexpr BlockType BIRCH_STAIRS = BlockFromType(152);
constexpr BlockType JUNGLE_STAIRS = BlockFromType(153);
constexpr BlockType ACACIA_STAIRS = BlockFromType(154);
constexpr BlockType DARK_OAK_STAIRS = BlockFromType(155);

// Miscellaneous Stones
constexpr BlockType DIORITE = BlockFromType(156);
constexpr BlockType ANDESITE = BlockFromType(157);
constexpr BlockType GRANITE = BlockFromType(158);
constexpr BlockType POLISHED_DIORITE = BlockFromType(159);
constexpr BlockType POLISHED_ANDESITE = BlockFromType(160);
constexpr BlockType POLISHED_GRANITE = BlockFromType(161);

// Terracotta & Glazed Terracotta
constexpr BlockType TERRACOTTA = BlockFromType(162);
constexpr BlockType WHITE_TERRACOTTA = BlockFromType(163);
constexpr BlockType ORANGE_TERRACOTTA = BlockFromType(164);
constexpr BlockType MAGENTA_TERRACOTTA = BlockFromType(165);
constexpr BlockType LIGHT_BLUE_TERRACOTTA = BlockFromType(166);
constexpr BlockType YELLOW_TERRACOTTA = BlockFromType(167);
constexpr BlockType LIME_TERRACOTTA = BlockFromType(168);
constexpr BlockType PINK_TERRACOTTA = BlockFromType(169);
constexpr BlockType GRAY_TERRACOTTA = BlockFromType(170);
constexpr BlockType LIGHT_GRAY_TERRACOTTA = BlockFromType(171);
constexpr BlockType CYAN_TERRACOTTA = BlockFromType(172);
constexpr BlockType PURPLE_TERRACOTTA = BlockFromType(173);
constexpr BlockType BLUE_TERRACOTTA = BlockFromType(174);
constexpr BlockType BROWN_TERRACOTTA = BlockFromType(175);
constexpr BlockType GREEN_TERRACOTTA = BlockFromType(176);
constexpr BlockType RED_TERRACOTTA = BlockFromType(177);
constexpr BlockType BLACK_TERRACOTTA = BlockFromType(178);
constexpr BlockType WHITE_GLAZED_TERRACOTTA = BlockFromType(179);
constexpr BlockType ORANGE_GLAZED_TERRACOTTA = BlockFromType(180);
constexpr BlockType MAGENTA_GLAZED_TERRACOTTA = BlockFromType(181);
constexpr BlockType LIGHT_BLUE_GLAZED_TERRACOTTA = BlockFromType(182);
constexpr BlockType YELLOW_GLAZED_TERRACOTTA = BlockFromType(183);
constexpr BlockType LIME_GLAZED_TERRACOTTA = BlockFromType(184);
constexpr BlockType PINK_GLAZED_TERRACOTTA = BlockFromType(185);
constexpr BlockType GRAY_GLAZED_TERRACOTTA = BlockFromType(186);
constexpr BlockType LIGHT_GRAY_GLAZED_TERRACOTTA = BlockFromType(187);
constexpr BlockType CYAN_GLAZED_TERRACOTTA = BlockFromType(188);
constexpr BlockType PURPLE_GLAZED_TERRACOTTA = BlockFromType(189);
constexpr BlockType BLUE_GLAZED_TERRACOTTA = BlockFromType(190);
constexpr BlockType BROWN_GLAZED_TERRACOTTA = BlockFromType(191);
constexpr BlockType GREEN_GLAZED_TERRACOTTA = BlockFromType(192);
constexpr BlockType RED_GLAZED_TERRACOTTA = BlockFromType(193);
constexpr BlockType BLACK_GLAZED_TERRACOTTA = BlockFromType(194);

// Concrete & Concrete Powder
constexpr BlockType WHITE_CONCRETE = BlockFromType(195);
constexpr BlockType ORANGE_CONCRETE = BlockFromType(196);
constexpr BlockType MAGENTA_CONCRETE = BlockFromType(197);
constexpr BlockType LIGHT_BLUE_CONCRETE = BlockFromType(198);
constexpr BlockType YELLOW_CONCRETE = BlockFromType(199);
constexpr BlockType LIME_CONCRETE = BlockFromType(200);
constexpr BlockType PINK_CONCRETE = BlockFromType(201);
constexpr BlockType GRAY_CONCRETE = BlockFromType(202);
constexpr BlockType LIGHT_GRAY_CONCRETE = BlockFromType(203);
constexpr BlockType CYAN_CONCRETE = BlockFromType(204);
constexpr BlockType PURPLE_CONCRETE = BlockFromType(205);
constexpr BlockType BLUE_CONCRETE = BlockFromType(206);
constexpr BlockType BROWN_CONCRETE = BlockFromType(207);
constexpr BlockType GREEN_CONCRETE = BlockFromType(208);
constexpr BlockType RED_CONCRETE = BlockFromType(209);
constexpr BlockType BLACK_CONCRETE = BlockFromType(210);
constexpr BlockType WHITE_CONCRETE_POWDER = BlockFromType(211);
constexpr BlockType ORANGE_CONCRETE_POWDER = BlockFromType(212);
constexpr BlockType MAGENTA_CONCRETE_POWDER = BlockFromType(213);
constexpr BlockType LIGHT_BLUE_CONCRETE_POWDER = BlockFromType(214);
constexpr BlockType YELLOW_CONCRETE_POWDER = BlockFromType(215);
constexpr BlockType LIME_CONCRETE_POWDER = BlockFromType(216);
constexpr BlockType PINK_CONCRETE_POWDER = BlockFromType(217);
constexpr BlockType GRAY_CONCRETE_POWDER = BlockFromType(218);
constexpr BlockType LIGHT_GRAY_CONCRETE_POWDER = BlockFromType(219);
constexpr BlockType CYAN_CONCRETE_POWDER = BlockFromType(220);
constexpr BlockType PURPLE_CONCRETE_POWDER = BlockFromType(221);
constexpr BlockType BLUE_CONCRETE_POWDER = BlockFromType(222);
constexpr BlockType BROWN_CONCRETE_POWDER = BlockFromType(223);
constexpr BlockType GREEN_CONCRETE_POWDER = BlockFromType(224);
constexpr BlockType RED_CONCRETE_POWDER = BlockFromType(225);
constexpr BlockType BLACK_CONCRETE_POWDER = BlockFromType(226);

// Miscellaneous Items
constexpr BlockType HAY_BLOCK = BlockFromType(227);
constexpr BlockType COAL_BLOCK = BlockFromType(228);
constexpr BlockType MELON_BLOCK = BlockFromType(229);
constexpr BlockType PUMPKIN = BlockFromType(230);
constexpr BlockType JACK_O_LANTERN = BlockFromType(231);

// Others
constexpr BlockType MYCELIUM = BlockFromType(232);
constexpr BlockType PODZOL = BlockFromType(233);
constexpr BlockType SNOW_LAYER = BlockFromType(234);
constexpr BlockType TALL_SEAGRASS = BlockFromType(235);
constexpr BlockType KELP = BlockFromType(236);
constexpr BlockType BAMBOO = BlockFromType(237);
