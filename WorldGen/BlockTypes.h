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

#define BlockFromType(BlockType) std::bit_cast<Block>(BlockType)

// Natural Blocks
constexpr Block AIR = BlockFromType(0);
constexpr Block STONE = BlockFromType(1);
constexpr Block GRASS = BlockFromType(2);
constexpr Block DIRT = BlockFromType(3);
constexpr Block COBBLESTONE = BlockFromType(4);
constexpr Block SAND = BlockFromType(5);
constexpr Block GRAVEL = BlockFromType(6);
constexpr Block GOLD_ORE = BlockFromType(7);
constexpr Block IRON_ORE = BlockFromType(8);
constexpr Block COAL_ORE = BlockFromType(9);
constexpr Block LOG = BlockFromType(10);
constexpr Block LEAVES = BlockFromType(11);
constexpr Block BEDROCK = BlockFromType(12);
constexpr Block WATER = BlockFromType(13);
constexpr Block LAVA = BlockFromType(14);
constexpr Block SANDSTONE = BlockFromType(15);
constexpr Block CLAY = BlockFromType(16);
constexpr Block OBSIDIAN = BlockFromType(17);

// Plant Blocks
constexpr Block GRASS_BLOCK = BlockFromType(18);
constexpr Block TALL_GRASS = BlockFromType(19);
constexpr Block FERN = BlockFromType(20);
constexpr Block DEAD_BUSH = BlockFromType(21);
constexpr Block DANDELION = BlockFromType(22);
constexpr Block POPPY = BlockFromType(23);
constexpr Block BLUE_ORCHID = BlockFromType(24);
constexpr Block ALLIUM = BlockFromType(25);
constexpr Block AZURE_BLUET = BlockFromType(26);
constexpr Block RED_TULIP = BlockFromType(27);
constexpr Block ORANGE_TULIP = BlockFromType(28);
constexpr Block WHITE_TULIP = BlockFromType(29);
constexpr Block PINK_TULIP = BlockFromType(30);
constexpr Block OXEYE_DAISY = BlockFromType(31);
constexpr Block SUNFLOWER = BlockFromType(32);
constexpr Block LILAC = BlockFromType(33);
constexpr Block ROSE_BUSH = BlockFromType(34);
constexpr Block PEONY = BlockFromType(35);
constexpr Block LILY_PAD = BlockFromType(36);
constexpr Block CACTUS = BlockFromType(37);
constexpr Block SUGAR_CANE = BlockFromType(38);

// Manufactured Blocks
constexpr Block PLANKS = BlockFromType(39);
constexpr Block SANDSTONE_SMOOTH = BlockFromType(40);
constexpr Block BRICKS = BlockFromType(41);
constexpr Block STONE_BRICKS = BlockFromType(42);
constexpr Block STONE_BRICKS_MOSSY = BlockFromType(43);
constexpr Block STONE_BRICKS_CRACKED = BlockFromType(44);
constexpr Block STONE_BRICKS_CHISELED = BlockFromType(45);
constexpr Block STONE_SLAB = BlockFromType(46);
constexpr Block COBBLESTONE_SLAB = BlockFromType(47);
constexpr Block BRICK_SLAB = BlockFromType(48);
constexpr Block STONE_BRICK_SLAB = BlockFromType(49);
constexpr Block QUARTZ_SLAB = BlockFromType(50);
constexpr Block NETHER_BRICK_SLAB = BlockFromType(51);
constexpr Block NETHER_BRICK = BlockFromType(52);
constexpr Block QUARTZ_BLOCK = BlockFromType(53);
constexpr Block QUARTZ_BLOCK_CHISELED = BlockFromType(54);
constexpr Block QUARTZ_BLOCK_PILLAR = BlockFromType(55);
constexpr Block GLASS = BlockFromType(56);
constexpr Block GLOWSTONE = BlockFromType(57);
constexpr Block REDSTONE_BLOCK = BlockFromType(58);
constexpr Block IRON_BLOCK = BlockFromType(59);
constexpr Block GOLD_BLOCK = BlockFromType(60);
constexpr Block DIAMOND_BLOCK = BlockFromType(61);
constexpr Block EMERALD_BLOCK = BlockFromType(62);

// Utility Blocks
constexpr Block TORCH = BlockFromType(63);
constexpr Block FURNACE = BlockFromType(64);
constexpr Block CRAFTING_TABLE = BlockFromType(65);
constexpr Block CHEST = BlockFromType(66);
constexpr Block ENCHANTMENT_TABLE = BlockFromType(67);
constexpr Block ANVIL = BlockFromType(68);
constexpr Block BREWING_STAND = BlockFromType(69);
constexpr Block BEACON = BlockFromType(70);
constexpr Block ENDER_CHEST = BlockFromType(71);
constexpr Block DISPENSER = BlockFromType(72);
constexpr Block DROPPER = BlockFromType(73);
constexpr Block HOPPER = BlockFromType(74);

// Redstone Components
constexpr Block REDSTONE_WIRE = BlockFromType(75);
constexpr Block REDSTONE_TORCH = BlockFromType(76);
constexpr Block REDSTONE_REPEATER = BlockFromType(77);
constexpr Block REDSTONE_COMPARATOR = BlockFromType(78);
constexpr Block LEVER = BlockFromType(79);
constexpr Block BUTTON = BlockFromType(80);
constexpr Block PRESSURE_PLATE = BlockFromType(81);
constexpr Block PISTON = BlockFromType(82);
constexpr Block STICKY_PISTON = BlockFromType(83);
constexpr Block TRIPWIRE_HOOK = BlockFromType(84);
constexpr Block TRIPWIRE = BlockFromType(85);
constexpr Block OBSERVER = BlockFromType(86);

// Decorative Blocks
constexpr Block CARPET = BlockFromType(87);
constexpr Block WOOL = BlockFromType(88);
constexpr Block GLASS_PANE = BlockFromType(89);
constexpr Block IRON_BARS = BlockFromType(90);
constexpr Block STAINED_GLASS = BlockFromType(91);
constexpr Block STAINED_GLASS_PANE = BlockFromType(92);
constexpr Block BANNER = BlockFromType(93);
constexpr Block FLOWER_POT = BlockFromType(94);
constexpr Block PAINTING = BlockFromType(95);

// Miscellaneous
constexpr Block SPONGE = BlockFromType(96);
constexpr Block WEB = BlockFromType(97);
constexpr Block SNOW = BlockFromType(98);
constexpr Block ICE = BlockFromType(99);
constexpr Block PACKED_ICE = BlockFromType(100);
constexpr Block SEA_LANTERN = BlockFromType(101);
constexpr Block PRISMARINE = BlockFromType(102);
constexpr Block PRISMARINE_BRICKS = BlockFromType(103);
constexpr Block DARK_PRISMARINE = BlockFromType(104);
constexpr Block SHULKER_BOX = BlockFromType(105);
constexpr Block BARRIER = BlockFromType(106);
constexpr Block END_PORTAL_FRAME = BlockFromType(107);

// Nether & End Blocks
constexpr Block NETHERRACK = BlockFromType(108);
constexpr Block NETHER_QUARTZ_ORE = BlockFromType(109);
constexpr Block NETHER_BRICK_FENCE = BlockFromType(110);
constexpr Block NETHER_WART_BLOCK = BlockFromType(111);
constexpr Block MAGMA_BLOCK = BlockFromType(112);
constexpr Block END_STONE = BlockFromType(113);
constexpr Block END_STONE_BRICKS = BlockFromType(114);
constexpr Block PURPUR_BLOCK = BlockFromType(115);
constexpr Block PURPUR_PILLAR = BlockFromType(116);
constexpr Block PURPUR_STAIRS = BlockFromType(117);
constexpr Block END_ROD = BlockFromType(118);

// Logs & Leaves Variants
constexpr Block OAK_LOG = BlockFromType(119);
constexpr Block SPRUCE_LOG = BlockFromType(120);
constexpr Block BIRCH_LOG = BlockFromType(121);
constexpr Block JUNGLE_LOG = BlockFromType(122);
constexpr Block ACACIA_LOG = BlockFromType(123);
constexpr Block DARK_OAK_LOG = BlockFromType(124);
constexpr Block OAK_LEAVES = BlockFromType(125);
constexpr Block SPRUCE_LEAVES = BlockFromType(126);
constexpr Block BIRCH_LEAVES = BlockFromType(127);
constexpr Block JUNGLE_LEAVES = BlockFromType(128);
constexpr Block ACACIA_LEAVES = BlockFromType(129);
constexpr Block DARK_OAK_LEAVES = BlockFromType(130);

// Fences & Gates
constexpr Block OAK_FENCE = BlockFromType(131);
constexpr Block SPRUCE_FENCE = BlockFromType(132);
constexpr Block BIRCH_FENCE = BlockFromType(133);
constexpr Block JUNGLE_FENCE = BlockFromType(134);
constexpr Block ACACIA_FENCE = BlockFromType(135);
constexpr Block DARK_OAK_FENCE = BlockFromType(136);
constexpr Block NETHER_BRICK_FENCE_REDEFINITION = BlockFromType(137);
constexpr Block OAK_FENCE_GATE = BlockFromType(138);
constexpr Block SPRUCE_FENCE_GATE = BlockFromType(139);
constexpr Block BIRCH_FENCE_GATE = BlockFromType(140);
constexpr Block JUNGLE_FENCE_GATE = BlockFromType(141);
constexpr Block ACACIA_FENCE_GATE = BlockFromType(142);
constexpr Block DARK_OAK_FENCE_GATE

 = BlockFromType(143);

// Stairs
constexpr Block OAK_STAIRS = BlockFromType(144);
constexpr Block COBBLESTONE_STAIRS = BlockFromType(145);
constexpr Block BRICK_STAIRS = BlockFromType(146);
constexpr Block STONE_BRICK_STAIRS = BlockFromType(147);
constexpr Block NETHER_BRICK_STAIRS = BlockFromType(148);
constexpr Block SANDSTONE_STAIRS = BlockFromType(149);
constexpr Block QUARTZ_STAIRS = BlockFromType(150);
constexpr Block SPRUCE_STAIRS = BlockFromType(151);
constexpr Block BIRCH_STAIRS = BlockFromType(152);
constexpr Block JUNGLE_STAIRS = BlockFromType(153);
constexpr Block ACACIA_STAIRS = BlockFromType(154);
constexpr Block DARK_OAK_STAIRS = BlockFromType(155);

// Miscellaneous Stones
constexpr Block DIORITE = BlockFromType(156);
constexpr Block ANDESITE = BlockFromType(157);
constexpr Block GRANITE = BlockFromType(158);
constexpr Block POLISHED_DIORITE = BlockFromType(159);
constexpr Block POLISHED_ANDESITE = BlockFromType(160);
constexpr Block POLISHED_GRANITE = BlockFromType(161);

// Terracotta & Glazed Terracotta
constexpr Block TERRACOTTA = BlockFromType(162);
constexpr Block WHITE_TERRACOTTA = BlockFromType(163);
constexpr Block ORANGE_TERRACOTTA = BlockFromType(164);
constexpr Block MAGENTA_TERRACOTTA = BlockFromType(165);
constexpr Block LIGHT_BLUE_TERRACOTTA = BlockFromType(166);
constexpr Block YELLOW_TERRACOTTA = BlockFromType(167);
constexpr Block LIME_TERRACOTTA = BlockFromType(168);
constexpr Block PINK_TERRACOTTA = BlockFromType(169);
constexpr Block GRAY_TERRACOTTA = BlockFromType(170);
constexpr Block LIGHT_GRAY_TERRACOTTA = BlockFromType(171);
constexpr Block CYAN_TERRACOTTA = BlockFromType(172);
constexpr Block PURPLE_TERRACOTTA = BlockFromType(173);
constexpr Block BLUE_TERRACOTTA = BlockFromType(174);
constexpr Block BROWN_TERRACOTTA = BlockFromType(175);
constexpr Block GREEN_TERRACOTTA = BlockFromType(176);
constexpr Block RED_TERRACOTTA = BlockFromType(177);
constexpr Block BLACK_TERRACOTTA = BlockFromType(178);
constexpr Block WHITE_GLAZED_TERRACOTTA = BlockFromType(179);
constexpr Block ORANGE_GLAZED_TERRACOTTA = BlockFromType(180);
constexpr Block MAGENTA_GLAZED_TERRACOTTA = BlockFromType(181);
constexpr Block LIGHT_BLUE_GLAZED_TERRACOTTA = BlockFromType(182);
constexpr Block YELLOW_GLAZED_TERRACOTTA = BlockFromType(183);
constexpr Block LIME_GLAZED_TERRACOTTA = BlockFromType(184);
constexpr Block PINK_GLAZED_TERRACOTTA = BlockFromType(185);
constexpr Block GRAY_GLAZED_TERRACOTTA = BlockFromType(186);
constexpr Block LIGHT_GRAY_GLAZED_TERRACOTTA = BlockFromType(187);
constexpr Block CYAN_GLAZED_TERRACOTTA = BlockFromType(188);
constexpr Block PURPLE_GLAZED_TERRACOTTA = BlockFromType(189);
constexpr Block BLUE_GLAZED_TERRACOTTA = BlockFromType(190);
constexpr Block BROWN_GLAZED_TERRACOTTA = BlockFromType(191);
constexpr Block GREEN_GLAZED_TERRACOTTA = BlockFromType(192);
constexpr Block RED_GLAZED_TERRACOTTA = BlockFromType(193);
constexpr Block BLACK_GLAZED_TERRACOTTA = BlockFromType(194);

// Concrete & Concrete Powder
constexpr Block WHITE_CONCRETE = BlockFromType(195);
constexpr Block ORANGE_CONCRETE = BlockFromType(196);
constexpr Block MAGENTA_CONCRETE = BlockFromType(197);
constexpr Block LIGHT_BLUE_CONCRETE = BlockFromType(198);
constexpr Block YELLOW_CONCRETE = BlockFromType(199);
constexpr Block LIME_CONCRETE = BlockFromType(200);
constexpr Block PINK_CONCRETE = BlockFromType(201);
constexpr Block GRAY_CONCRETE = BlockFromType(202);
constexpr Block LIGHT_GRAY_CONCRETE = BlockFromType(203);
constexpr Block CYAN_CONCRETE = BlockFromType(204);
constexpr Block PURPLE_CONCRETE = BlockFromType(205);
constexpr Block BLUE_CONCRETE = BlockFromType(206);
constexpr Block BROWN_CONCRETE = BlockFromType(207);
constexpr Block GREEN_CONCRETE = BlockFromType(208);
constexpr Block RED_CONCRETE = BlockFromType(209);
constexpr Block BLACK_CONCRETE = BlockFromType(210);
constexpr Block WHITE_CONCRETE_POWDER = BlockFromType(211);
constexpr Block ORANGE_CONCRETE_POWDER = BlockFromType(212);
constexpr Block MAGENTA_CONCRETE_POWDER = BlockFromType(213);
constexpr Block LIGHT_BLUE_CONCRETE_POWDER = BlockFromType(214);
constexpr Block YELLOW_CONCRETE_POWDER = BlockFromType(215);
constexpr Block LIME_CONCRETE_POWDER = BlockFromType(216);
constexpr Block PINK_CONCRETE_POWDER = BlockFromType(217);
constexpr Block GRAY_CONCRETE_POWDER = BlockFromType(218);
constexpr Block LIGHT_GRAY_CONCRETE_POWDER = BlockFromType(219);
constexpr Block CYAN_CONCRETE_POWDER = BlockFromType(220);
constexpr Block PURPLE_CONCRETE_POWDER = BlockFromType(221);
constexpr Block BLUE_CONCRETE_POWDER = BlockFromType(222);
constexpr Block BROWN_CONCRETE_POWDER = BlockFromType(223);
constexpr Block GREEN_CONCRETE_POWDER = BlockFromType(224);
constexpr Block RED_CONCRETE_POWDER = BlockFromType(225);
constexpr Block BLACK_CONCRETE_POWDER = BlockFromType(226);

// Miscellaneous Items
constexpr Block HAY_BLOCK = BlockFromType(227);
constexpr Block COAL_BLOCK = BlockFromType(228);
constexpr Block MELON_BLOCK = BlockFromType(229);
constexpr Block PUMPKIN = BlockFromType(230);
constexpr Block JACK_O_LANTERN = BlockFromType(231);

// Others
constexpr Block MYCELIUM = BlockFromType(232);
constexpr Block PODZOL = BlockFromType(233);
constexpr Block SNOW_LAYER = BlockFromType(234);
constexpr Block TALL_SEAGRASS = BlockFromType(235);
constexpr Block KELP = BlockFromType(236);
constexpr Block BAMBOO = BlockFromType(237);
