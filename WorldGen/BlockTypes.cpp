#include "BlockTypes.h"

const BlockColor BlockColors[] = {
    // Natural Blocks
    {255, 255, 255},  // Air (Invisible)
    {128, 128, 128},  // Stone
    {0, 128, 0},      // Grass // 0,255,0 grass original
    {139, 69, 19},    // Dirt
    {128, 128, 128},  // Cobblestone
    {237, 201, 175},  // Sand
    {112, 128, 144},  // Gravel
    {255, 223, 0},    // GoldOre
    {183, 183, 183},  // IronOre
    {54, 54, 54},     // CoalOre
    {139, 69, 19},    // Log
    {34, 139, 34},    // Leaves
    {0, 0, 0},        // Bedrock
    {0, 64, 192},   // Water {64, 164, 223},   // Water
    {255, 69, 0},     // Lava
    {237, 201, 175},  // Sandstone
    {160, 160, 255},  // Clay
    {29, 29, 44},     // Obsidian

    // Plant Blocks
    {0, 255, 0},      // GrassBlock
    {124, 252, 0},    // TallGrass
    {85, 107, 47},    // Fern
    {139, 69, 19},    // DeadBush
    {255, 255, 0},    // Dandelion
    {255, 0, 0},      // Poppy
    {0, 255, 255},    // BlueOrchid
    {238, 130, 238},  // Allium
    {173, 216, 230},  // AzureBluet
    {255, 0, 0},      // RedTulip
    {255, 165, 0},    // OrangeTulip
    {255, 255, 255},  // WhiteTulip
    {255, 182, 193},  // PinkTulip
    {255, 255, 0},    // OxeyeDaisy
    {255, 255, 0},    // Sunflower
    {218, 112, 214},  // Lilac
    {255, 0, 0},      // RoseBush
    {255, 192, 203},  // Peony
    {0, 255, 0},      // LilyPad
    {0, 255, 0},      // Cactus
    {255, 255, 0},    // SugarCane

    // Manufactured Blocks
    {205, 133, 63},   // Planks
    {237, 201, 175},  // SandstoneSmooth
    {165, 42, 42},    // Bricks
    {120, 120, 120},  // StoneBricks
    {85, 107, 47},    // StoneBricksMossy
    {105, 105, 105},  // StoneBricksCracked
    {169, 169, 169},  // StoneBricksChiseled
    {189, 189, 189},  // StoneSlab
    {128, 128, 128},  // CobblestoneSlab
    {165, 42, 42},    // BrickSlab
    {120, 120, 120},  // StoneBrickSlab
    {255, 255, 255},  // QuartzSlab
    {112, 112, 112},  // NetherBrickSlab
    {112, 112, 112},  // NetherBrick
    {255, 255, 255},  // QuartzBlock
    {255, 255, 255},  // QuartzBlockChiseled
    {255, 255, 255},  // QuartzBlockPillar
    {255, 255, 255},  // Glass
    {255, 255, 0},    // Glowstone
    {255, 0, 0},      // RedstoneBlock
    {192, 192, 192},  // IronBlock
    {255, 215, 0},    // GoldBlock
    {0, 191, 255},    // DiamondBlock
    {50, 205, 50},    // EmeraldBlock

    // Utility Blocks
    {255, 140, 0},    // Torch
    {169, 169, 169},  // Furnace
    {139, 69, 19},    // CraftingTable
    {139, 69, 19},    // Chest
    {72, 61, 139},    // EnchantmentTable
    {192, 192, 192},  // Anvil
    {128, 128, 128},  // BrewingStand
    {255, 255, 0},    // Beacon
    {72, 61, 139},    // EnderChest
    {169, 169, 169},  // Dispenser
    {169, 169, 169},  // Dropper
    {169, 169, 169},  // Hopper

    // Redstone Components
    {255, 0, 0},      // RedstoneWire
    {255, 140, 0},    // RedstoneTorch
    {255, 0, 0},      // RedstoneRepeater
    {255, 0, 0},      // RedstoneComparator
    {139, 69, 19},    // Lever
    {169, 169, 169},  // Button
    {192, 192, 192},  // PressurePlate
    {169, 169, 169},  // Piston
    {169, 169, 169},  // StickyPiston
    {169, 169, 169},  // TripwireHook
    {255, 255, 255},  // Tripwire
    {169, 169, 169},  // Observer

    // Decorative Blocks
    {255, 255, 255},  // Carpet
    {255, 255, 255},  // Wool
    {255, 255, 255},  // GlassPane
    {192, 192, 192},  // IronBars
    {255, 182, 193},  // StainedGlass
    {255, 182, 193},  // StainedGlassPane
    {255, 0, 0},      // Banner
    {139, 69, 19},    // FlowerPot
    {255, 255, 255},  // Painting

    // Miscellaneous
    {255, 255, 0},    // Sponge
    {255, 255, 255},  // Web
    {255, 255, 255},  // Snow
    {173, 216, 230},  // Ice
    {173, 216, 230},  // PackedIce
    {173, 216, 230},  // SeaLantern
    {0, 191, 255},    // Prismarine
    {0, 191, 255},    // PrismarineBricks
    {0, 191, 255},    // DarkPrismarine
    {255, 0, 255},    // ShulkerBox
    {255, 255, 255},  // Barrier
    {169, 169, 169},  // EndPortalFrame

    // Nether & End Blocks
    {139, 0, 0},      // Netherrack
    {255, 255, 255},  // NetherQuartzOre
    {112, 112, 112},  // NetherBrickFence
    {255, 0, 0},      // NetherWartBlock
    {255, 69, 0},     // MagmaBlock
    {255, 255, 255},  // EndStone
    {169, 169, 169},  // EndStoneBricks
    {112, 112, 112},  // PurpurBlock
    {112, 112, 112},  // PurpurPillar
    {112, 112, 112},  // PurpurStairs
    {255, 255, 255},  // EndRod

    // Logs & Leaves Variants
    {139, 69, 19},    // OakLog
    {112, 112, 112},  // SpruceLog
    {255, 255, 255},  // BirchLog
    {169, 169, 169},  // JungleLog
    {255, 165, 0},    // AcaciaLog
    {112, 112, 112},  // DarkOakLog
    {34, 64, 34},    // OakLeaves 34,139,34 origial
    {34, 139, 34},    // SpruceLeaves
    {34, 139, 34},    // BirchLeaves
    {34, 139, 34},    // JungleLeaves
    {34, 139, 34},    // AcaciaLeaves
    {34, 139, 34},    // DarkOakLeaves

    // Fences & Gates
    {139, 69, 19},    // OakFence
    {112, 112, 112},  // SpruceFence
    {255, 255, 255},  // BirchFence
    {169, 169, 169},  // JungleFence
    {255, 165, 0},    // AcaciaFence
    {112, 112, 112},  // DarkOakFence
    {112, 112, 112},  // NetherBrickFence
    {139, 69, 19},    // OakFenceGate
    {112, 112, 112},  // SpruceFenceGate
    {255, 255, 255},  // BirchFenceGate
    {169, 169, 169},  // JungleFenceGate
    {255, 165, 0},    // AcaciaFenceGate
    {112, 112, 112},  // DarkOakFenceGate

    // Stairs
    {139, 69, 19},    // OakStairs
    {128, 128, 128},  // CobblestoneStairs
    {165, 42, 42},    // BrickStairs
    {120, 120, 120},  // StoneBrickStairs
    {112, 112, 112},  // NetherBrickStairs
    {237, 201, 175},  // SandstoneStairs
    {255, 255, 255},  // QuartzStairs
    {112, 112, 112},  // SpruceStairs
    {255, 255, 255},  // BirchStairs
    {169, 169, 169},  // JungleStairs
    {255, 165, 0},    // AcaciaStairs
    {112, 112, 112},  // DarkOakStairs

    // Miscellaneous Stones
    {189, 189, 189},  // Diorite
    {169, 169, 169},  // Andesite
    {205, 133, 63},   // Granite
    {192, 192, 192},  // PolishedDiorite
    {169, 169, 169},  // PolishedAndesite
    {205, 133, 63},   // PolishedGranite

    // Terracotta & Glazed Terracotta
    {210, 180, 140},  // Terracotta
    {255, 255, 255},  // WhiteTerracotta
    {255, 165, 0},    // OrangeTerracotta
    {255, 0, 255},    // MagentaTerracotta
    {173, 216, 230},  // LightBlueTerracotta
    {255, 255, 0},    // YellowTerracotta
    {50, 205, 50},    // LimeTerracotta
    {255, 192, 203},  // PinkTerracotta
    {128, 128, 128},  // GrayTerracotta
    {211, 211, 211},  // LightGrayTerracotta
    {0, 255, 255},    // CyanTerracotta
    {160, 32, 240},   // PurpleTerracotta
    {0, 0, 255},      // BlueTerracotta
    {139, 69, 19},    // BrownTerracotta
    {50, 205, 50},    // GreenTerracotta
    {255, 0, 0},      // RedTerracotta
    {0, 0, 0},        // BlackTerracotta
    {255, 255, 255},  // WhiteGlazedTerracotta
    {255, 165, 0},    // OrangeGlazedTerracotta
    {255, 0, 255},    // MagentaGlazedTerracotta
    {173, 216, 230},  // LightBlueGlazedTerracotta
    {255, 255, 0},    // YellowGlazedTerracotta
    {50, 205, 50},    // LimeGlazedTerracotta
    {255, 192, 203},  // PinkGlazedTerracotta
    {128, 128, 128},  // GrayGlazedTerracotta
    {211, 211, 211},  // LightGrayGlazedTerracotta
    {0, 255, 255},    // CyanGlazedTerracotta
    {160, 32, 240},   // PurpleGlazedTerracotta
    {0, 0, 255},      // BlueGlazedTerracotta
    {139, 69, 19},    // BrownGlazedTerracotta
    {50, 205, 50},    // GreenGlazedTerracotta
    {255, 0, 0},      // RedGlazedTerracotta
    {0, 0, 0},        // BlackGlazedTerracotta

    // Concrete & Concrete Powder
    {255, 255, 255},  // WhiteConcrete
    {255, 165, 0},    // OrangeConcrete
    {255, 0, 255},    // MagentaConcrete
    {173, 216, 230},  // LightBlueConcrete
    {255, 255, 0},    // YellowConcrete
    {50, 205, 50},    // LimeConcrete
    {255, 192, 203},  // PinkConcrete
    {128, 128, 128},  // GrayConcrete
    {211, 211, 211},  // LightGrayConcrete
    {0, 255, 255},    // CyanConcrete
    {160, 32, 240},   // PurpleConcrete
    {0, 0, 255},      // BlueConcrete
    {139, 69, 19},    // BrownConcrete
    {50, 205, 50},    // GreenConcrete
    {255, 0, 0},      // RedConcrete
    {0, 0, 0},        // BlackConcrete
    {255, 255, 255},  // WhiteConcretePowder
    {255, 165, 0},    // OrangeConcretePowder
    {255, 0, 255},    // MagentaConcretePowder
    {173, 216, 230},  // LightBlueConcretePowder
    {255, 255, 0},    // YellowConcretePowder
    {50, 205, 50},    // LimeConcretePowder
    {255, 192, 203},  // PinkConcretePowder
    {128, 128, 128},  // GrayConcretePowder
    {211, 211, 211},  // LightGrayConcretePowder
    {0, 255, 255},    // CyanConcretePowder
    {160, 32, 240},   // PurpleConcretePowder
    {0, 0, 255},      // BlueConcretePowder
    {139, 69, 19},    // BrownConcretePowder
    {50, 205, 50},    // GreenConcretePowder
    {255, 0, 0},      // RedConcretePowder
    {0, 0, 0},        // BlackConcretePowder

    // Miscellaneous Items
    {240, 230, 140},  // HayBlock
    {54, 54, 54},     // CoalBlock
    {144, 238, 144},  // MelonBlock
    {255, 165, 0},    // Pumpkin
    {255, 69, 0},     // JackOLantern

    // Others
    {139, 69, 19},    // Mycelium
    {139, 69, 19},    // Podzol
    {255, 255, 255},  // SnowLayer
    {34, 139, 34},    // TallSeagrass
    {34, 139, 34},    // Kelp
    {34, 139, 34},    // Bamboo
};
