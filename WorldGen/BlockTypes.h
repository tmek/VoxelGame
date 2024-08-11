#pragma once
#include <cstdint>
#include ".\VoxelCore\VoxelDefines.h"

struct BlockColor
{
    unsigned char Red;
    unsigned char Green;
    unsigned char Blue;
};

extern const BlockColor BlockColors[];

// Natural Blocks
const BlockType AIR = 0;
const BlockType STONE = 1;
const BlockType GRASS = 2;
const BlockType DIRT = 3;
const BlockType COBBLESTONE = 4;
const BlockType SAND = 5;
const BlockType GRAVEL = 6;
const BlockType GOLD_ORE = 7;
const BlockType IRON_ORE = 8;
const BlockType COAL_ORE = 9;
const BlockType LOG = 10;
const BlockType LEAVES = 11;
const BlockType BEDROCK = 12;
const BlockType WATER = 13;
const BlockType LAVA = 14;
const BlockType SANDSTONE = 15;
const BlockType CLAY = 16;
const BlockType OBSIDIAN = 17;

// Plant Blocks
const BlockType GRASS_BLOCK = 18;
const BlockType TALL_GRASS = 19;
const BlockType FERN = 20;
const BlockType DEAD_BUSH = 21;
const BlockType DANDELION = 22;
const BlockType POPPY = 23;
const BlockType BLUE_ORCHID = 24;
const BlockType ALLIUM = 25;
const BlockType AZURE_BLUET = 26;
const BlockType RED_TULIP = 27;
const BlockType ORANGE_TULIP = 28;
const BlockType WHITE_TULIP = 29;
const BlockType PINK_TULIP = 30;
const BlockType OXEYE_DAISY = 31;
const BlockType SUNFLOWER = 32;
const BlockType LILAC = 33;
const BlockType ROSE_BUSH = 34;
const BlockType PEONY = 35;
const BlockType LILY_PAD = 36;
const BlockType CACTUS = 37;
const BlockType SUGAR_CANE = 38;

// Manufactured Blocks
const BlockType PLANKS = 39;
const BlockType SANDSTONE_SMOOTH = 40;
const BlockType BRICKS = 41;
const BlockType STONE_BRICKS = 42;
const BlockType STONE_BRICKS_MOSSY = 43;
const BlockType STONE_BRICKS_CRACKED = 44;
const BlockType STONE_BRICKS_CHISELED = 45;
const BlockType STONE_SLAB = 46;
const BlockType COBBLESTONE_SLAB = 47;
const BlockType BRICK_SLAB = 48;
const BlockType STONE_BRICK_SLAB = 49;
const BlockType QUARTZ_SLAB = 50;
const BlockType NETHER_BRICK_SLAB = 51;
const BlockType NETHER_BRICK = 52;
const BlockType QUARTZ_BLOCK = 53;
const BlockType QUARTZ_BLOCK_CHISELED = 54;
const BlockType QUARTZ_BLOCK_PILLAR = 55;
const BlockType GLASS = 56;
const BlockType GLOWSTONE = 57;
const BlockType REDSTONE_BLOCK = 58;
const BlockType IRON_BLOCK = 59;
const BlockType GOLD_BLOCK = 60;
const BlockType DIAMOND_BLOCK = 61;
const BlockType EMERALD_BLOCK = 62;

// Utility Blocks
const BlockType TORCH = 63;
const BlockType FURNACE = 64;
const BlockType CRAFTING_TABLE = 65;
const BlockType CHEST = 66;
const BlockType ENCHANTMENT_TABLE = 67;
const BlockType ANVIL = 68;
const BlockType BREWING_STAND = 69;
const BlockType BEACON = 70;
const BlockType ENDER_CHEST = 71;
const BlockType DISPENSER = 72;
const BlockType DROPPER = 73;
const BlockType HOPPER = 74;

// Redstone Components
const BlockType REDSTONE_WIRE = 75;
const BlockType REDSTONE_TORCH = 76;
const BlockType REDSTONE_REPEATER = 77;
const BlockType REDSTONE_COMPARATOR = 78;
const BlockType LEVER = 79;
const BlockType BUTTON = 80;
const BlockType PRESSURE_PLATE = 81;
const BlockType PISTON = 82;
const BlockType STICKY_PISTON = 83;
const BlockType TRIPWIRE_HOOK = 84;
const BlockType TRIPWIRE = 85;
const BlockType OBSERVER = 86;

// Decorative Blocks
const BlockType CARPET = 87;
const BlockType WOOL = 88;
const BlockType GLASS_PANE = 89;
const BlockType IRON_BARS = 90;
const BlockType STAINED_GLASS = 91;
const BlockType STAINED_GLASS_PANE = 92;
const BlockType BANNER = 93;
const BlockType FLOWER_POT = 94;
const BlockType PAINTING = 95;

// Miscellaneous
const BlockType SPONGE = 96;
const BlockType WEB = 97;
const BlockType SNOW = 98;
const BlockType ICE = 99;
const BlockType PACKED_ICE = 100;
const BlockType SEA_LANTERN = 101;
const BlockType PRISMARINE = 102;
const BlockType PRISMARINE_BRICKS = 103;
const BlockType DARK_PRISMARINE = 104;
const BlockType SHULKER_BOX = 105;
const BlockType BARRIER = 106;
const BlockType END_PORTAL_FRAME = 107;

// Nether & End Blocks
const BlockType NETHERRACK = 108;
const BlockType NETHER_QUARTZ_ORE = 109;
const BlockType NETHER_BRICK_FENCE = 110;
const BlockType NETHER_WART_BLOCK = 111;
const BlockType MAGMA_BLOCK = 112;
const BlockType END_STONE = 113;
const BlockType END_STONE_BRICKS = 114;
const BlockType PURPUR_BLOCK = 115;
const BlockType PURPUR_PILLAR = 116;
const BlockType PURPUR_STAIRS = 117;
const BlockType END_ROD = 118;

// Logs & Leaves Variants
const BlockType OAK_LOG = 119;
const BlockType SPRUCE_LOG = 120;
const BlockType BIRCH_LOG = 121;
const BlockType JUNGLE_LOG = 122;
const BlockType ACACIA_LOG = 123;
const BlockType DARK_OAK_LOG = 124;
const BlockType OAK_LEAVES = 125;
const BlockType SPRUCE_LEAVES = 126;
const BlockType BIRCH_LEAVES = 127;
const BlockType JUNGLE_LEAVES = 128;
const BlockType ACACIA_LEAVES = 129;
const BlockType DARK_OAK_LEAVES = 130;

// Fences & Gates
const BlockType OAK_FENCE = 131;
const BlockType SPRUCE_FENCE = 132;
const BlockType BIRCH_FENCE = 133;
const BlockType JUNGLE_FENCE = 134;
const BlockType ACACIA_FENCE = 135;
const BlockType DARK_OAK_FENCE = 136;
const BlockType NETHER_BRICK_FENCE_REDEFINITION = 137;
const BlockType OAK_FENCE_GATE = 138;
const BlockType SPRUCE_FENCE_GATE = 139;
const BlockType BIRCH_FENCE_GATE = 140;
const BlockType JUNGLE_FENCE_GATE = 141;
const BlockType ACACIA_FENCE_GATE = 142;
const BlockType DARK_OAK_FENCE_GATE = 143;

// Stairs
const BlockType OAK_STAIRS = 144;
const BlockType COBBLESTONE_STAIRS = 145;
const BlockType BRICK_STAIRS = 146;
const BlockType STONE_BRICK_STAIRS = 147;
const BlockType NETHER_BRICK_STAIRS = 148;
const BlockType SANDSTONE_STAIRS = 149;
const BlockType QUARTZ_STAIRS = 150;
const BlockType SPRUCE_STAIRS = 151;
const BlockType BIRCH_STAIRS = 152;
const BlockType JUNGLE_STAIRS = 153;
const BlockType ACACIA_STAIRS = 154;
const BlockType DARK_OAK_STAIRS = 155;

// Miscellaneous Stones
const BlockType DIORITE = 156;
const BlockType ANDESITE = 157;
const BlockType GRANITE = 158;
const BlockType POLISHED_DIORITE = 159;
const BlockType POLISHED_ANDESITE = 160;
const BlockType POLISHED_GRANITE = 161;

// Terracotta & Glazed Terracotta
const BlockType TERRACOTTA = 162;
const BlockType WHITE_TERRACOTTA = 163;
const BlockType ORANGE_TERRACOTTA = 164;
const BlockType MAGENTA_TERRACOTTA = 165;
const BlockType LIGHT_BLUE_TERRACOTTA = 166;
const BlockType YELLOW_TERRACOTTA = 167;
const BlockType LIME_TERRACOTTA = 168;
const BlockType PINK_TERRACOTTA = 169;
const BlockType GRAY_TERRACOTTA = 170;
const BlockType LIGHT_GRAY_TERRACOTTA = 171;
const BlockType CYAN_TERRACOTTA = 172;
const BlockType PURPLE_TERRACOTTA = 173;
const BlockType BLUE_TERRACOTTA = 174;
const BlockType BROWN_TERRACOTTA = 175;
const BlockType GREEN_TERRACOTTA = 176;
const BlockType RED_TERRACOTTA = 177;
const BlockType BLACK_TERRACOTTA = 178;
const BlockType WHITE_GLAZED_TERRACOTTA = 179;
const BlockType ORANGE_GLAZED_TERRACOTTA = 180;
const BlockType MAGENTA_GLAZED_TERRACOTTA = 181;
const BlockType LIGHT_BLUE_GLAZED_TERRACOTTA = 182;
const BlockType YELLOW_GLAZED_TERRACOTTA = 183;
const BlockType LIME_GLAZED_TERRACOTTA = 184;
const BlockType PINK_GLAZED_TERRACOTTA = 185;
const BlockType GRAY_GLAZED_TERRACOTTA = 186;
const BlockType LIGHT_GRAY_GLAZED_TERRACOTTA = 187;
const BlockType CYAN_GLAZED_TERRACOTTA = 188;
const BlockType PURPLE_GLAZED_TERRACOTTA = 189;
const BlockType BLUE_GLAZED_TERRACOTTA = 190;
const BlockType BROWN_GLAZED_TERRACOTTA = 191;
const BlockType GREEN_GLAZED_TERRACOTTA = 192;
const BlockType RED_GLAZED_TERRACOTTA = 193;
const BlockType BLACK_GLAZED_TERRACOTTA = 194;

// Concrete & Concrete Powder
const BlockType WHITE_CONCRETE = 195;
const BlockType ORANGE_CONCRETE = 196;
const BlockType MAGENTA_CONCRETE = 197;
const BlockType LIGHT_BLUE_CONCRETE = 198;
const BlockType YELLOW_CONCRETE = 199;
const BlockType LIME_CONCRETE = 200;
const BlockType PINK_CONCRETE = 201;
const BlockType GRAY_CONCRETE = 202;
const BlockType LIGHT_GRAY_CONCRETE = 203;
const BlockType CYAN_CONCRETE = 204;
const BlockType PURPLE_CONCRETE = 205;
const BlockType BLUE_CONCRETE = 206;
const BlockType BROWN_CONCRETE = 207;
const BlockType GREEN_CONCRETE = 208;
const BlockType RED_CONCRETE = 209;
const BlockType BLACK_CONCRETE = 210;
const BlockType WHITE_CONCRETE_POWDER = 211;
const BlockType ORANGE_CONCRETE_POWDER = 212;
const BlockType MAGENTA_CONCRETE_POWDER = 213;
const BlockType LIGHT_BLUE_CONCRETE_POWDER = 214;
const BlockType YELLOW_CONCRETE_POWDER = 215;
const BlockType LIME_CONCRETE_POWDER = 216;
const BlockType PINK_CONCRETE_POWDER = 217;
const BlockType GRAY_CONCRETE_POWDER = 218;
const BlockType LIGHT_GRAY_CONCRETE_POWDER = 219;
const BlockType CYAN_CONCRETE_POWDER = 220;
const BlockType PURPLE_CONCRETE_POWDER = 221;
const BlockType BLUE_CONCRETE_POWDER = 222;
const BlockType BROWN_CONCRETE_POWDER = 223;
const BlockType GREEN_CONCRETE_POWDER = 224;
const BlockType RED_CONCRETE_POWDER = 225;
const BlockType BLACK_CONCRETE_POWDER = 226;

// Miscellaneous Items
const BlockType HAY_BLOCK = 227;
const BlockType COAL_BLOCK = 228;
const BlockType MELON_BLOCK = 229;
const BlockType PUMPKIN = 230;
const BlockType JACK_O_LANTERN = 231;

// Others
const BlockType MYCELIUM = 232;
const BlockType PODZOL = 233;
const BlockType SNOW_LAYER = 234;
const BlockType TALL_SEAGRASS = 235;
const BlockType KELP = 236;
const BlockType BAMBOO = 237;
