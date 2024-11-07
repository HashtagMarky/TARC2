#include "constants/abilities.h"

static const struct Ability sDeokishisuAbilitiesInfo[ABILITIES_COUNT] =
{
    [ABILITY_NONE] =
    {
        .description = COMPOUND_STRING("No special ability."),
    },
    
    [ABILITY_STENCH] =
    {
        .description = COMPOUND_STRING("Releases a stench that may cause\nan attacked foe to flinch."),
    },
    
    [ABILITY_DRIZZLE] =
    {
        .description = COMPOUND_STRING("The Pokémon makes it rain\nwhen it enters a battle."),
    },
    
    [ABILITY_SPEED_BOOST] =
    {
        .description = COMPOUND_STRING("The Pokémon's Speed stat\nis boosted every turn."),
    },
    
    [ABILITY_BATTLE_ARMOR] =
    {
        .description = COMPOUND_STRING("Hard armor protects the\nPokémon from critical hits."),
    },
    
    [ABILITY_STURDY] =
    {
        .description = COMPOUND_STRING("The Pokémon can't be knocked out\nin one hit or by one-hit KO moves."),
    },
    
    [ABILITY_DAMP] =
    {
        .description = COMPOUND_STRING("Prevents self-destruction by\ndampening its surroundings."),
    },
    
    [ABILITY_LIMBER] =
    {
        .description = COMPOUND_STRING("The Pokémon's limber body prevents\nit from being paralyzed."),
    },
    
    [ABILITY_SAND_VEIL] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's evasiveness\nin a sandstorm."),
    },
    
    [ABILITY_STATIC] =
    {
        .description = COMPOUND_STRING("The Pokémon is statically-charged.\nContact with it may cause paralysis."),
    },
    
    [ABILITY_VOLT_ABSORB] =
    {
        .description = COMPOUND_STRING("Restores HP if hit by Electric-type\nmoves instead of taking damage."),
    },
    
    [ABILITY_WATER_ABSORB] =
    {
        .description = COMPOUND_STRING("Restores HP if hit by Water-type\nmoves instead of taking damage."),
    },
    
    [ABILITY_OBLIVIOUS] =
    {
        .description = COMPOUND_STRING("Keeps the Pokémon from being\ninfatuated, taunted, or intimidated."),
    },
    
    [ABILITY_CLOUD_NINE] =
    {
        .description = COMPOUND_STRING("Eliminates the effects of weather."),
    },
    
    [ABILITY_COMPOUND_EYES] =
    {
        .description = COMPOUND_STRING("The Pokémon's compound eyes\nboost its accuracy."),
    },
    
    [ABILITY_INSOMNIA] =
    {
        .description = COMPOUND_STRING("The Pokémon's insomnia prevents it\nfrom falling asleep."),
    },
    
    [ABILITY_COLOR_CHANGE] =
    {
        .description = COMPOUND_STRING("The Pokémon's type becomes the\ntype of the move used on it."),
    },
    
    [ABILITY_IMMUNITY] =
    {
        .description = COMPOUND_STRING("The Pokémon's immune system\nprevents it from being poisoned."),
    },
    
    [ABILITY_FLASH_FIRE] =
    {
        .description = COMPOUND_STRING("Absorbs Fire-type moves to power up\nthe Pokémon's own Fire-type moves."),
    },
    
    [ABILITY_SHIELD_DUST] =
    {
        .description = COMPOUND_STRING("Protective dust shields the Pokémon\nfrom additional move effects."),
    },
    
    [ABILITY_OWN_TEMPO] =
    {
        .description = COMPOUND_STRING("This Pokémon has its own tempo, and\ncannot be confused or intimidated."),
    },
    
    [ABILITY_SUCTION_CUPS] =
    {
        .description = COMPOUND_STRING("The Pokémon is firmly anchored.\nIt cannot be forcibly switched out."),
    },
    
    [ABILITY_INTIMIDATE] =
    {
        .description = COMPOUND_STRING("The Pokémon lowers the foe's\nAttack stat upon entering battle."),
    },
    
    [ABILITY_SHADOW_TAG] =
    {
        .description = COMPOUND_STRING("The Pokémon steps on the foe's\nshadow to prevent it from escaping."),
    },
    
    [ABILITY_ROUGH_SKIN] =
    {
        .description = COMPOUND_STRING("The Pokémon's rough skin damages\nfoes making direct contact with it."),
    },
    
    [ABILITY_WONDER_GUARD] =
    {
        .description = COMPOUND_STRING("The Pokémon's mysterious power only\nlets supereffective moves hit it."),
    },
    
    [ABILITY_LEVITATE] =
    {
        .description = COMPOUND_STRING("By floating in the air, the Pokémon\nis immune to all Ground-type moves."),
    },
    
    [ABILITY_EFFECT_SPORE] =
    {
        .description = COMPOUND_STRING("Contact with the Pokémon may inflict\npoison, sleep, or paralysis."),
    },
    
    [ABILITY_SYNCHRONIZE] =
    {
        .description = COMPOUND_STRING("Passes poison, paralysis, or burn\nto the Pokémon that inflicted it."),
    },
    
    [ABILITY_CLEAR_BODY] =
    {
        .description = COMPOUND_STRING("Prevents other Pokémon's moves or\nAbilities from lowering its stats."),
    },
    
    [ABILITY_NATURAL_CURE] =
    {
        .description = COMPOUND_STRING("The Pokémon's status conditions\nare cured when it switches out."),
    },
    
    [ABILITY_LIGHTNING_ROD] =
    {
        .description = COMPOUND_STRING("Draws in Electric-type moves to\nboost Sp. Atk without getting hit."),
    },
    
    [ABILITY_SERENE_GRACE] =
    {
        .description = COMPOUND_STRING("Boosts the likelihood of additional\neffects occurring when attacking."),
    },
    
    [ABILITY_SWIFT_SWIM] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed\nstat in rain."),
    },
    
    [ABILITY_CHLOROPHYLL] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed\nstat in harsh sunlight."),
    },
    
    [ABILITY_ILLUMINATE] =
    {
        .description = COMPOUND_STRING("Illuminates its surroundings.\nIts acccuracy cannot be lowered."),
    },
    
    [ABILITY_TRACE] =
    {
        .description = COMPOUND_STRING("When it enters a battle, the Pokémon\ncopies a foe Pokémon's ability."),
    },
    
    [ABILITY_HUGE_POWER] =
    {
        .description = COMPOUND_STRING("Doubles the Pokémon's Attack stat."),
    },
    
    [ABILITY_POISON_POINT] =
    {
        .description = COMPOUND_STRING("Contact with the Pokémon\nmay poison the attacker."),
    },
    
    [ABILITY_INNER_FOCUS] =
    {
        .description = COMPOUND_STRING("The Pokémon is intensely focused and\ncannot be flinched or intimidated."),
    },
    
    [ABILITY_MAGMA_ARMOR] =
    {
        .description = COMPOUND_STRING("The Pokémon's hot magma coating\nprevents it from being frozen."),
    },
    
    [ABILITY_WATER_VEIL] =
    {
        .description = COMPOUND_STRING("The Pokémon's water veil prevents\nit from being burned."),
    },
    
    [ABILITY_MAGNET_PULL] =
    {
        .description = COMPOUND_STRING("Prevents Steel-type Pokémon from\nfleeing with its magenetic force."),
    },
    
    [ABILITY_SOUNDPROOF] =
    {
        .description = COMPOUND_STRING("Soundproofing gives the Pokémon\nimmunity to all sound-based moves."),
    },
    
    [ABILITY_RAIN_DISH] =
    {
        .description = COMPOUND_STRING("The Pokémon gradually\nregains HP in rain."),
    },
    
    [ABILITY_SAND_STREAM] =
    {
        .description = COMPOUND_STRING("The Pokémon summons a sandstorm\nwhen it enters a battle."),
    },
    
    [ABILITY_PRESSURE] =
    {
        .description = COMPOUND_STRING("Puts other Pokémon under pressure.\nThey will expend more PP to attack."),
    },
    
    [ABILITY_THICK_FAT] =
    {
        .description = COMPOUND_STRING("A layer of fat halves the damage\ntaken from Fire- and Ice-type moves."),
    },
    
    [ABILITY_EARLY_BIRD] =
    {
        .description = COMPOUND_STRING("The Pokémon awakens from sleep\ntwice as fast as other Pokémon."),
    },
    
    [ABILITY_FLAME_BODY] =
    {
        .description = COMPOUND_STRING("Contact with the Pokémon\nmay burn the attacker."),
    },
    
    [ABILITY_RUN_AWAY] =
    {
        .description = COMPOUND_STRING("Enables a sure getaway\nfrom wild Pokémon."),
    },
    
    [ABILITY_KEEN_EYE] =
    {
        .description = COMPOUND_STRING("The Pokémon's keen eyes prevent its\naccuracy from being lowered."),
    },
    
    [ABILITY_HYPER_CUTTER] =
    {
        .description = COMPOUND_STRING("The Pokémon's mighty pincers prevent\nothers from lowering its Attack."),
    },
    
    [ABILITY_PICKUP] =
    {
        .description = COMPOUND_STRING("The Pokémon may pick up an item\nboth in an out of battle."),
    },
    
    [ABILITY_TRUANT] =
    {
        .description = COMPOUND_STRING("Each time the Pokémon uses a move,\nit loafs around the next turn."),
    },
    
    [ABILITY_HUSTLE] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Attack stat\nbut lowers its accuracy."),
    },
    
    [ABILITY_CUTE_CHARM] =
    {
        .description = COMPOUND_STRING("Contact with the Pokémon\nmay cause infatuation."),
    },
    
    [ABILITY_PLUS] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat\nif an active ally has Plus or Minus."),
    },
    
    [ABILITY_MINUS] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat\nif an active ally has Plus or Minus."),
    },
    
    [ABILITY_FORECAST] =
    {
        .description = COMPOUND_STRING("Changes the Pokémon's type to Water,\nFire, or Ice dependent on weather."),
    },
    
    [ABILITY_STICKY_HOLD] =
    {
        .description = COMPOUND_STRING("The Pokémon's held items are stuck\nfast and cannot be forcibly removed."),
    },
    
    [ABILITY_SHED_SKIN] =
    {
        .description = COMPOUND_STRING("The Pokémon may cure its own status\nconditions by shedding its skin."),
    },
    
    [ABILITY_GUTS] =
    {
        .description = COMPOUND_STRING("It's so gutsy that having a status\ncondition boosts its Attack stat."),
    },
    
    [ABILITY_MARVEL_SCALE] =
    {
        .description = COMPOUND_STRING("Its scales boost its Defense stat\nif it has a status condition."),
    },
    
    [ABILITY_LIQUID_OOZE] =
    {
        .description = COMPOUND_STRING("The Pokémon's oozed liquid damages\nattackers that drain its HP."),
    },
    
    [ABILITY_OVERGROW] =
    {
        .description = COMPOUND_STRING("Powers ups Grass-type moves when\nthe Pokémon's HP is low."),
    },
    
    [ABILITY_BLAZE] =
    {
        .description = COMPOUND_STRING("Powers ups Fire-type moves when\nthe Pokémon's HP is low."),
    },
    
    [ABILITY_TORRENT] =
    {
        .description = COMPOUND_STRING("Powers ups Water-type moves when\nthe Pokémon's HP is low."),
    },
    
    [ABILITY_SWARM] =
    {
        .description = COMPOUND_STRING("Powers ups Bug-type moves when\nthe Pokémon's HP is low."),
    },
    
    [ABILITY_ROCK_HEAD] =
    {
        .description = COMPOUND_STRING("Protects the Pokémon\nfrom recoil damage."),
    },
    
    [ABILITY_DROUGHT] =
    {
        .description = COMPOUND_STRING("Turns the sunlight harsh when\nthe Pokémon enters battle."),
    },
    
    [ABILITY_ARENA_TRAP] =
    {
        .description = COMPOUND_STRING("Prevents the opposing Pokémon\nfrom fleeing battle."),
    },
    
    [ABILITY_VITAL_SPIRIT] =
    {
        .description = COMPOUND_STRING("The Pokémon's incredible vitality\nprevents it from falling asleep."),
    },
    
    [ABILITY_WHITE_SMOKE] =
    {
        .description = COMPOUND_STRING("Its protective white smoke prevents\nothers from lowering its stats."),
    },
    
    [ABILITY_PURE_POWER] =
    {
        .description = COMPOUND_STRING("Using its pure power, the Pokémon\ndoubles its Attack stat."),
    },
    
    [ABILITY_SHELL_ARMOR] =
    {
        .description = COMPOUND_STRING("A hard shell protects the Pokémon\nfrom critical hits."),
    },
    
    [ABILITY_AIR_LOCK] =
    {
        .description = COMPOUND_STRING("Eliminates the effects of weather."),
    },
    
    [ABILITY_TANGLED_FEET] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's evasiveness\nif it is confused."),
    },
    
    [ABILITY_MOTOR_DRIVE] =
    {
        .description = COMPOUND_STRING("Incoming Electric-type moves do no\ndamage and boost Speed instead."),
    },
    
    [ABILITY_RIVALRY] =
    {
        .description = COMPOUND_STRING("It does more damage to the same\ngender but less to the opposite."),
    },
    
    [ABILITY_STEADFAST] =
    {
        .description = COMPOUND_STRING("The Pokémon's determination boosts\nits Speed stat when it flinches."),
    },
    
    [ABILITY_SNOW_CLOAK] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's\nevasiveness in snow."),
    },
    
    [ABILITY_GLUTTONY] =
    {
        .description = COMPOUND_STRING("The Pokémon will eat its held Berry\nat half HP instead of at quarter HP."),
    },
    
    [ABILITY_ANGER_POINT] =
    {
        .description = COMPOUND_STRING("The Pokémon is angered when hit\ncritically, maxing its Attack stat."),
    },
    
    [ABILITY_UNBURDEN] =
    {
        .description = COMPOUND_STRING("Boosts the Speed stat if the\nPokémon's held item is used or lost."),
    },
    
    [ABILITY_HEATPROOF] =
    {
        .description = COMPOUND_STRING("With its heatproof body, it halves\ndamage taken from Fire-type moves."),
    },
    
    [ABILITY_SIMPLE] =
    {
        .description = COMPOUND_STRING("Doubles the effects of\nthe Pokémon's stat changes."),
    },
    
    [ABILITY_DRY_SKIN] =
    {
        .description = COMPOUND_STRING("Water and rain restore its HP, but\nit's damaged by sun and Fire."),
    },
    
    [ABILITY_DOWNLOAD] =
    {
        .description = COMPOUND_STRING("The Pokémon analyzes a foe to raise\nits most-effective attacking stat."),
    },
    
    [ABILITY_IRON_FIST] =
    {
        .description = COMPOUND_STRING("Powers up punching moves."),
    },
    
    [ABILITY_POISON_HEAL] =
    {
        .description = COMPOUND_STRING("If poisoned, the Pokémon has its HP\nrestored instead of taking damage."),
    },
    
    [ABILITY_ADAPTABILITY] =
    {
        .description = COMPOUND_STRING("Powers up moves of the same type\nas the Pokémon."),
    },
    
    [ABILITY_SKILL_LINK] =
    {
        .description = COMPOUND_STRING("Maximizes the number of times\na multistrike move hits."),
    },
    
    [ABILITY_HYDRATION] =
    {
        .description = COMPOUND_STRING("Cures the Pokémon's status\nconditions in rain."),
    },
    
    [ABILITY_SOLAR_POWER] =
    {
        .description = COMPOUND_STRING("Boosts the Sp. Atk stat in sunlight,\nbut HP is decreased every turn."),
    },
    
    [ABILITY_QUICK_FEET] =
    {
        .description = COMPOUND_STRING("Boosts the Speed stat if the Pokémon\nhas a status condition."),
    },
    
    [ABILITY_NORMALIZE] =
    {
        .description = COMPOUND_STRING("All its moves become Normal-type\nand their power is boosted."),
    },
    
    [ABILITY_SNIPER] =
    {
        .description = COMPOUND_STRING("If the Pokémon lands a critical hit,\nthe attack is powered up further."),
    },
    
    [ABILITY_MAGIC_GUARD] =
    {
        .description = COMPOUND_STRING("The Pokémon only takes damage\nfrom attacks."),
    },
    
    [ABILITY_NO_GUARD] =
    {
        .description = COMPOUND_STRING("The Pokémon ensures incoming\nand outgoing attacks never miss."),
    },
    
    [ABILITY_STALL] =
    {
        .description = COMPOUND_STRING("The Pokémon is always the last\nto use its moves."),
    },
    
    [ABILITY_TECHNICIAN] =
    {
        .description = COMPOUND_STRING("Powers up weak moves so the Pokémon\ncan deal more damage with them."),
    },
    
    [ABILITY_LEAF_GUARD] =
    {
        .description = COMPOUND_STRING("Prevents status conditions\nin harsh sunlight."),
    },
    
    [ABILITY_KLUTZ] =
    {
        .description = COMPOUND_STRING("The Pokémon can't use\nany held items."),
    },
    
    [ABILITY_MOLD_BREAKER] =
    {
        .description = COMPOUND_STRING("The Pokémon's moves are unimpeded\nby the Ability of the target."),
    },
    
    [ABILITY_SUPER_LUCK] =
    {
        .description = COMPOUND_STRING("The Pokémon is so lucky that its\nmoves critically hit more often."),
    },
    
    [ABILITY_AFTERMATH] =
    {
        .description = COMPOUND_STRING("Damages the attacker that knocks it\nout with a direct contact move."),
    },
    
    [ABILITY_ANTICIPATION] =
    {
        .description = COMPOUND_STRING("The Pokémon can sense an opposing\nPokémon's dangerous moves."),
    },
    
    [ABILITY_FOREWARN] =
    {
        .description = COMPOUND_STRING("The Pokémon can tell one of its foe's\nmoves when entering battle."),
    },
    
    [ABILITY_UNAWARE] =
    {
        .description = COMPOUND_STRING("When attacking, the Pokémon ignores\nthe target's stat changes."),
    },
    
    [ABILITY_TINTED_LENS] =
    {
        .description = COMPOUND_STRING("The Pokémon's “not very effective”\nmoves deal regular damage."),
    },
    
    [ABILITY_FILTER] =
    {
        .description = COMPOUND_STRING("Reduces the power of supereffective\nattacks that hit the Pokémon."),
    },
    
    [ABILITY_SLOW_START] =
    {
        .description = COMPOUND_STRING("For five turns, the Pokémon's\nAttack and Speed stats are halved."),
    },
    
    [ABILITY_SCRAPPY] =
    {
        .description = COMPOUND_STRING("Hits Ghost types with Fighting- and\nNormal-type moves. Not intimidated."),
    },
    
    [ABILITY_STORM_DRAIN] =
    {
        .description = COMPOUND_STRING("Draws in Water-type moves to\nboost Sp. Atk without getting hit."),
    },
    
    [ABILITY_ICE_BODY] =
    {
        .description = COMPOUND_STRING("The Pokémon gradually\nregains HP in snow."),
    },
    
    [ABILITY_SOLID_ROCK] =
    {
        .description = COMPOUND_STRING("Reduces the power of supereffective\nattacks that hit the Pokémon."),
    },
    
    [ABILITY_SNOW_WARNING] =
    {
        .description = COMPOUND_STRING("The Pokémon makes it snow\nwhen it enters a battle."),
    },
    
    [ABILITY_HONEY_GATHER] =
    {
        .description = COMPOUND_STRING("The Pokémon may gather Honey\nafter a battle."),
    },
    
    [ABILITY_FRISK] =
    {
        .description = COMPOUND_STRING("When it enters battle, the Pokémon\nchecks a foe's held item."),
    },
    
    [ABILITY_RECKLESS] =
    {
        .description = COMPOUND_STRING("Powers up moves that\nhave recoil damage."),
    },
    
    [ABILITY_MULTITYPE] =
    {
        .description = COMPOUND_STRING("Changes the Pokémon's type to match\nthe plate it holds."),
    },
    
    [ABILITY_FLOWER_GIFT] =
    {
        .description = COMPOUND_STRING("Boosts the Attack and Sp. Def stats\nof itself and allies in sunlight."),
    },
    
    [ABILITY_BAD_DREAMS] =
    {
        .description = COMPOUND_STRING("Damages opposing Pokémon\nthat are asleep."),
    },
    
    [ABILITY_PICKPOCKET] =
    {
        .description = COMPOUND_STRING("The Pokémon steals an attacker's\nheld item when they make contact."),
    },
    
    [ABILITY_SHEER_FORCE] =
    {
        .description = COMPOUND_STRING("Additional move effects don't occur,\nbut the moves' power is boosted."),
    },
    
    [ABILITY_CONTRARY] =
    {
        .description = COMPOUND_STRING("Inverts stat changes. Boosting stats\nlowers them instead, and vice versa."),
    },
    
    [ABILITY_UNNERVE] =
    {
        .description = COMPOUND_STRING("Unnerves opposing Pokémon and\nmakes them unable to eat Berries."),
    },
    
    [ABILITY_DEFIANT] =
    {
        .description = COMPOUND_STRING("Sharply boosts the Pokémon's Attack\nstat if a foe lowers its stats."),
    },
    
    [ABILITY_DEFEATIST] =
    {
        .description = COMPOUND_STRING("Halves the Pokémon's Attack and Sp.\nAtk stats when HP is half or less."),
    },
    
    [ABILITY_CURSED_BODY] =
    {
        .description = COMPOUND_STRING("May disable a move that has dealt\ndamage to the Pokémon."),
    },
    
    [ABILITY_HEALER] =
    {
        .description = COMPOUND_STRING("Sometimes cures the status\nconditions of the Pokémon's allies."),
    },
    
    [ABILITY_FRIEND_GUARD] =
    {
        .description = COMPOUND_STRING("Reduces damage dealt to allies."),
    },
    
    [ABILITY_WEAK_ARMOR] =
    {
        .description = COMPOUND_STRING("When physically hit, its Defense stat\nfalls and Speed is sharply boosted."),
    },
    
    [ABILITY_HEAVY_METAL] =
    {
        .description = COMPOUND_STRING("Doubles the Pokémon's weight."),
    },
    
    [ABILITY_LIGHT_METAL] =
    {
        .description = COMPOUND_STRING("Halves the Pokémon's weight."),
    },
    
    [ABILITY_MULTISCALE] =
    {
        .description = COMPOUND_STRING("Reduces the amount of damage the\nPokémon takes while its HP is full."),
    },
    
    [ABILITY_TOXIC_BOOST] =
    {
        .description = COMPOUND_STRING("Powers up physical moves when\nthe Pokémon is poisoned."),
    },
    
    [ABILITY_FLARE_BOOST] =
    {
        .description = COMPOUND_STRING("Powers up special moves when\nthe Pokémon is burned."),
    },
    
    [ABILITY_HARVEST] =
    {
        .description = COMPOUND_STRING("May create another Berry\nafter one is used."),
    },
    
    [ABILITY_TELEPATHY] =
    {
        .description = COMPOUND_STRING("The Pokémon anticipates and dodges\nthe attacks of its allies."),
    },
    
    [ABILITY_MOODY] =
    {
        .description = COMPOUND_STRING("Every turn, one stat will be sharply\nboosted and another lowered."),
    },
    
    [ABILITY_OVERCOAT] =
    {
        .description = COMPOUND_STRING("The Pokémon is protected from\nsandstorms, powders, and spores."),
    },
    
    [ABILITY_POISON_TOUCH] =
    {
        .description = COMPOUND_STRING("May poison a target when\nthe Pokémon makes contact."),
    },
    
    [ABILITY_REGENERATOR] =
    {
        .description = COMPOUND_STRING("The Pokémon has a little of its HP\nrestored when withdrawn from battle."),
    },
    
    [ABILITY_BIG_PECKS] =
    {
        .description = COMPOUND_STRING("Prevents the Pokémon from having\nits Defense stat lowered."),
    },
    
    [ABILITY_SAND_RUSH] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Speed stat\nin a sandstorm."),
    },
    
    [ABILITY_WONDER_SKIN] =
    {
        .description = COMPOUND_STRING("Makes status moves more likely\nto miss the Pokémon."),
    },
    
    [ABILITY_ANALYTIC] =
    {
        .description = COMPOUND_STRING("Boosts the power of the Pokémon's\nmove if it acts last that turn."),
    },
    
    [ABILITY_ILLUSION] =
    {
        .description = COMPOUND_STRING("Comes out disguised as the Pokémon\nin the party's last spot."),
    },
    
    [ABILITY_IMPOSTER] =
    {
        .description = COMPOUND_STRING("The Pokémon transforms itself\ninto the Pokémon it's facing."),
    },
    
    [ABILITY_INFILTRATOR] =
    {
        .description = COMPOUND_STRING("The Pokémon's moves penetrate the\ntarget's barriers, substitutes, etc."),
    },
    
    [ABILITY_MUMMY] =
    {
        .description = COMPOUND_STRING("Contact with the Pokémon changes\nthe attacker's Ability to Mummy."),
    },
    
    [ABILITY_MOXIE] =
    {
        .description = COMPOUND_STRING("Knocking out a Pokémon shows moxie,\nand that boosts its Attack stat."),
    },
    
    [ABILITY_JUSTIFIED] =
    {
        .description = COMPOUND_STRING("When hit by a Dark-type move, its\nAttack stat is boosted for justice."),
    },
    
    [ABILITY_RATTLED] =
    {
        .description = COMPOUND_STRING("Ups Speed when intimidated or hit by\nBug-, Ghost-, or Dark-type moves."),
    },
    
    [ABILITY_MAGIC_BOUNCE] =
    {
        .description = COMPOUND_STRING("The Pokémon reflects status moves\ninstead of getting hit by them."),
    },
    
    [ABILITY_SAP_SIPPER] =
    {
        .description = COMPOUND_STRING("Incoming Grass-type moves do no\ndamage and boost Attack instead."),
    },
    
    [ABILITY_PRANKSTER] =
    {
        .description = COMPOUND_STRING("Gives priority to the Pokémon's\nstatus moves."),
    },
    
    [ABILITY_SAND_FORCE] =
    {
        .description = COMPOUND_STRING("Boosts the power of Rock-, Ground-,\nand Steel-type moves in a sandstorm."),
    },
    
    [ABILITY_IRON_BARBS] =
    {
        .description = COMPOUND_STRING("The Pokémon's iron barbs damage\nfoes making direct contact with it."),
    },
    
    [ABILITY_ZEN_MODE] =
    {
        .description = COMPOUND_STRING("Changes the Pokémon's shape when\nits HP drops to half or less."),
    },
    
    [ABILITY_VICTORY_STAR] =
    {
        .description = COMPOUND_STRING("Boosts the accuracy of the Pokémon\nand its allies."),
    },
    
    [ABILITY_TURBOBLAZE] =
    {
        .description = COMPOUND_STRING("The Pokémon's moves are unimpeded\nby the Ability of the target."),
    },
    
    [ABILITY_TERAVOLT] =
    {
        .description = COMPOUND_STRING("The Pokémon's moves are unimpeded\nby the Ability of the target."),
    },
    
    [ABILITY_AROMA_VEIL] =
    {
        .description = COMPOUND_STRING("Protects the Pokémon and its allies\nfrom effects that prevent move use."),
    },
    
    [ABILITY_FLOWER_VEIL] =
    {
        .description = COMPOUND_STRING("Ally Grass types are protected from\nstatus conditions and stat lowering."),
    },
    
    [ABILITY_CHEEK_POUCH] =
    {
        .description = COMPOUND_STRING("Eating a Berry restores HP\nin addition to its usual effect."),
    },
    
    [ABILITY_PROTEAN] =
    {
        .description = COMPOUND_STRING("Changes its type to that of a queued\nmove once per battle entrance."),
    },
    
    [ABILITY_FUR_COAT] =
    {
        .description = COMPOUND_STRING("Halves the damage from\nphysical moves."),
    },
    
    [ABILITY_MAGICIAN] =
    {
        .description = COMPOUND_STRING("The Pokémon steals the held item\nfrom any target it hits with a move."),
    },
    
    [ABILITY_BULLETPROOF] =
    {
        .description = COMPOUND_STRING("Protects the Pokémon from\nball and bomb moves."),
    },
    
    [ABILITY_COMPETITIVE] =
    {
        .description = COMPOUND_STRING("Sharply boosts the Pokémon's Sp. Atk\nstat if a foe lowers its stats."),
    },
    
    [ABILITY_STRONG_JAW] =
    {
        .description = COMPOUND_STRING("The Pokémon's strong jaw boosts\nthe power of its biting moves."),
    },
    
    [ABILITY_REFRIGERATE] =
    {
        .description = COMPOUND_STRING("Normal-type moves become Ice-type\nand their power is boosted."),
    },
    
    [ABILITY_SWEET_VEIL] =
    {
        .description = COMPOUND_STRING("Prevents the Pokémon and its allies\nfrom falling asleep."),
    },
    
    [ABILITY_STANCE_CHANGE] =
    {
        .description = COMPOUND_STRING("Changes to Blade Forme by attacking,\nand Shield Forme with King's Shield."),
    },
    
    [ABILITY_GALE_WINGS] =
    {
        .description = COMPOUND_STRING("Gives priority to Flying-type\nmoves while its HP is full."),
    },
    
    [ABILITY_MEGA_LAUNCHER] =
    {
        .description = COMPOUND_STRING("Powers up pulse moves."),
    },
    
    [ABILITY_GRASS_PELT] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Defense\nstat on Grassy Terrain."),
    },
    
    [ABILITY_SYMBIOSIS] =
    {
        .description = COMPOUND_STRING("The Pokémon passes its item to\nan ally that has used up an item."),
    },
    
    [ABILITY_TOUGH_CLAWS] =
    {
        .description = COMPOUND_STRING("Powers up moves that\nmake direct contact."),
    },
    
    [ABILITY_PIXILATE] =
    {
        .description = COMPOUND_STRING("Normal-type moves become Fairy-\ntype and their power is boosted."),
    },
    
    [ABILITY_GOOEY] =
    {
        .description = COMPOUND_STRING("Contact with the Pokémon lowers\nthe attacker's Speed stat."),
    },
    
    [ABILITY_AERILATE] =
    {
        .description = COMPOUND_STRING("Normal-type moves become Flying-\ntype and their power is boosted."),
    },
    
    [ABILITY_PARENTAL_BOND] =
    {
        .description = COMPOUND_STRING("The parent and child attack\none after the other."),
    },
    
    [ABILITY_DARK_AURA] =
    {
        .description = COMPOUND_STRING("Powers up the Dark-type moves\nof all Pokémon on the field."),
    },
    
    [ABILITY_FAIRY_AURA] =
    {
        .description = COMPOUND_STRING("Powers up the Fairy-type moves\nof all Pokémon on the field."),
    },
    
    [ABILITY_AURA_BREAK] =
    {
        .description = COMPOUND_STRING("The effects of “Aura” Abilities are\nreversed, weakening affected moves."),
    },
    
    [ABILITY_PRIMORDIAL_SEA] =
    {
        .description = COMPOUND_STRING("The Pokémon changes the weather to\nnullify Fire-type attacks."),
    },
    
    [ABILITY_DESOLATE_LAND] =
    {
        .description = COMPOUND_STRING("The Pokémon changes the weather to\nnullify Water-type attacks."),
    },
    
    [ABILITY_DELTA_STREAM] =
    {
        .description = COMPOUND_STRING("The Pokémon changes the weather so\nFlying-types have no weaknesses."),
    },
    
    [ABILITY_STAMINA] =
    {
        .description = COMPOUND_STRING("Boosts the Defense stat when the\nPokémon is hit by an attack."),
    },
    
    [ABILITY_WIMP_OUT] =
    {
        .description = COMPOUND_STRING("The Pokémon cowardly switches out\nwhen its HP drops to half or less."),
    },
    
    [ABILITY_EMERGENCY_EXIT] =
    {
        .description = COMPOUND_STRING("It, sensing danger, switches out\nwhen its HP drops to half or less."),
    },
    
    [ABILITY_WATER_COMPACTION] =
    {
        .description = COMPOUND_STRING("Boosts the Defense stat sharply\nwhen hit by a Water-type move."),
    },
    
    [ABILITY_MERCILESS] =
    {
        .description = COMPOUND_STRING("The Pokémon's attacks hit critically\nif the target is poisoned."),
    },
    
    [ABILITY_SHIELDS_DOWN] =
    {
        .description = COMPOUND_STRING("At half HP or less, its shell breaks\nand it becomes aggressive."),
    },
    
    [ABILITY_STAKEOUT] =
    {
        .description = COMPOUND_STRING("Doubles the damage dealt to a target\nthat has just switched into battle."),
    },
    
    [ABILITY_WATER_BUBBLE] =
    {
        .description = COMPOUND_STRING("Halves Fire damage, prevents burns,\nand doubles Water-type move power."),
    },
    
    [ABILITY_STEELWORKER] =
    {
        .description = COMPOUND_STRING("Powers up Steel-type moves."),
    },
    
    [ABILITY_BERSERK] =
    {
        .description = COMPOUND_STRING("Boosts its Sp. Atk stat when a hit\ndrops its HP to half or less."),
    },
    
    [ABILITY_SLUSH_RUSH] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's\nSpeed stat in snow."),
    },
    
    [ABILITY_LONG_REACH] =
    {
        .description = COMPOUND_STRING("The Pokémon uses its moves without\nmaking contact with the target."),
    },
    
    [ABILITY_LIQUID_VOICE] =
    {
        .description = COMPOUND_STRING("Sound-based moves become\nWater-type moves."),
    },
    
    [ABILITY_TRIAGE] =
    {
        .description = COMPOUND_STRING("Gives priority to the\nPokémon's healing moves."),
    },
    
    [ABILITY_GALVANIZE] =
    {
        .description = COMPOUND_STRING("Normal-type moves become Electric-\ntype and their power is boosted."),
    },
    
    [ABILITY_SURGE_SURFER] =
    {
        .description = COMPOUND_STRING("Doubles the Pokémon's Speed\nstat on Electric Terrain."),
    },
    
    [ABILITY_SCHOOLING] =
    {
        .description = COMPOUND_STRING("With high HP, forms a mighty school.\nIt stops schooling with low HP."),
    },
    
    [ABILITY_DISGUISE] =
    {
        .description = COMPOUND_STRING("Once per battle, the shroud covering\nit can protect it from an attack."),
    },
    
    [ABILITY_BATTLE_BOND] =
    {
        .description = COMPOUND_STRING("When it KOs a target, its Attack, Sp.\nAtk, and Speed stats are boosted."),
    },
    
    [ABILITY_POWER_CONSTRUCT] =
    {
        .description = COMPOUND_STRING("When its HP drops to half or less,\ncells change it into Complete Forme."),
    },
    
    [ABILITY_CORROSION] =
    {
        .description = COMPOUND_STRING("The Pokémon can poison the target\neven if it's a Steel or Poison type."),
    },
    
    [ABILITY_COMATOSE] =
    {
        .description = COMPOUND_STRING("The Pokémon is always drowsing. It\ncan still attack without ever waking."),
    },
    
    [ABILITY_QUEENLY_MAJESTY] =
    {
        .description = COMPOUND_STRING("The Pokémon's majesty makes foes\nunable to hit with priority moves."),
    },
    
    [ABILITY_INNARDS_OUT] =
    {
        .description = COMPOUND_STRING("When knocked out, does damage equal\nto its remaining HP to the attacker."),
    },
    
    [ABILITY_DANCER] =
    {
        .description = COMPOUND_STRING("When another Pokémon dances, it will\ncopy the move and use it next."),
    },
    
    [ABILITY_BATTERY] =
    {
        .description = COMPOUND_STRING("Powers up ally Pokémon's\nspecial moves."),
    },
    
    [ABILITY_FLUFFY] =
    {
        .description = COMPOUND_STRING("Halves the damage taken from contact\nmoves, but doubles Fire-type damage."),
    },
    
    [ABILITY_DAZZLING] =
    {
        .description = COMPOUND_STRING("The Pokémon dazzles its foes so\nthey can't hit with priority moves."),
    },
    
    [ABILITY_SOUL_HEART] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Sp. Atk stat\nevery time another Pokémon faints."),
    },
    
    [ABILITY_TANGLING_HAIR] =
    {
        .description = COMPOUND_STRING("Contact with the Pokémon lowers\nthe attacker's Speed stat."),
    },
    
    [ABILITY_RECEIVER] =
    {
        .description = COMPOUND_STRING("The Pokémon copies the Ability\nof a defeated ally."),
    },
    
    [ABILITY_POWER_OF_ALCHEMY] =
    {
        .description = COMPOUND_STRING("The Pokémon copies the Ability\nof a defeated ally."),
    },
    
    [ABILITY_BEAST_BOOST] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's most proficient\nstat every time it KOs a target."),
    },
    
    [ABILITY_RKS_SYSTEM] =
    {
        .description = COMPOUND_STRING("Changes the Pokémon's type to match\nthe memory disc it holds."),
    },
    
    [ABILITY_ELECTRIC_SURGE] =
    {
        .description = COMPOUND_STRING("Turns the field into Electric Terrain\nwhen the Pokémon enters battle."),
    },
    
    [ABILITY_PSYCHIC_SURGE] =
    {
        .description = COMPOUND_STRING("Turns the field into Psychic Terrain\nwhen the Pokémon enters battle."),
    },
    
    [ABILITY_MISTY_SURGE] =
    {
        .description = COMPOUND_STRING("Turns the field into Misty Terrain\nwhen the Pokémon enters battle."),
    },
    
    [ABILITY_GRASSY_SURGE] =
    {
        .description = COMPOUND_STRING("Turns the field into Grassy Terrain\nwhen the Pokémon enters battle."),
    },
    
    [ABILITY_FULL_METAL_BODY] =
    {
        .description = COMPOUND_STRING("Prevents other Pokémon's moves or\nAbilities from lowering its stats."),
    },
    
    [ABILITY_SHADOW_SHIELD] =
    {
        .description = COMPOUND_STRING("Reduces the amount of damage the\nPokémon takes while its HP is full."),
    },
    
    [ABILITY_PRISM_ARMOR] =
    {
        .description = COMPOUND_STRING("Reduces the power of supereffective\nattacks that hit the Pokémon."),
    },
    
    [ABILITY_NEUROFORCE] =
    {
        .description = COMPOUND_STRING("Powers up the Pokémon's\nsupereffective moves even further."),
    },
    
    [ABILITY_INTREPID_SWORD] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Attack stat\nthe first time it enters battle."),
    },
    
    [ABILITY_DAUNTLESS_SHIELD] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's Defense stat\nthe first time it enters battle."),
    },
    
    [ABILITY_LIBERO] =
    {
        .description = COMPOUND_STRING("Changes its type to that of a queued\nmove once per battle entrance."),
    },
    
    [ABILITY_BALL_FETCH] =
    {
        .description = COMPOUND_STRING("It will fetch a failed Poké Ball if\nit is not already holding an item."),
    },
    
    [ABILITY_COTTON_DOWN] =
    {
        .description = COMPOUND_STRING("Cotton fluff is scattered when hit,\nlowering all others' Speed stats."),
    },
    
    [ABILITY_PROPELLER_TAIL] =
    {
        .description = COMPOUND_STRING("Ignores redirection effects from\nthe foe's Abilities and moves."),
    },
    
    [ABILITY_MIRROR_ARMOR] =
    {
        .description = COMPOUND_STRING("Bounces back only the stat-lowering\neffects that the Pokémon receives."),
    },
    
    [ABILITY_GULP_MISSILE] =
    {
        .description = COMPOUND_STRING("Finds prey with Surf or Dive. When\nhit, it spits the prey out at the foe."),
    },
    
    [ABILITY_STALWART] =
    {
        .description = COMPOUND_STRING("Ignores redirection effects from\nthe foe's Abilities and moves."),
    },
    
    [ABILITY_STEAM_ENGINE] =
    {
        .description = COMPOUND_STRING("Drastically boosts Speed if it's hit\nby Fire- or Water-type moves."),
    },
    
    [ABILITY_PUNK_ROCK] =
    {
        .description = COMPOUND_STRING("Boosts the power of and takes half\ndamage from sound-based moves."),
    },
    
    [ABILITY_SAND_SPIT] =
    {
        .description = COMPOUND_STRING("The Pokémon creates a sandstorm\nif it's hit by an attack."),
    },
    
    [ABILITY_ICE_SCALES] =
    {
        .description = COMPOUND_STRING("The Pokémon's ice scales halve\ndamage taken from special moves."),
    },
    
    [ABILITY_RIPEN] =
    {
        .description = COMPOUND_STRING("Ripens Berries and doubles\ntheir effect."),
    },
    
    [ABILITY_ICE_FACE] =
    {
        .description = COMPOUND_STRING("Its ice head acts as a substitute.\nThe ice is restored when it snows."),
    },
    
    [ABILITY_POWER_SPOT] =
    {
        .description = COMPOUND_STRING("Just being next to the\nPokémon powers up moves."),
    },
    
    [ABILITY_MIMICRY] =
    {
        .description = COMPOUND_STRING("Changes the Pokémon's type\ndepending on the terrain."),
    },
    
    [ABILITY_SCREEN_CLEANER] =
    {
        .description = COMPOUND_STRING("When it enters battle, Light Screen\nReflect, and Aurora Veil all end."),
    },
    
    [ABILITY_STEELY_SPIRIT] =
    {
        .description = COMPOUND_STRING("Powers up the Steel-type moves\nof the Pokémon and its allies."),
    },
    
    [ABILITY_PERISH_BODY] =
    {
        .description = COMPOUND_STRING("If the foe makes direct contact, it\nand the foe will faint in three turns."),
    },
    
    [ABILITY_WANDERING_SPIRIT] =
    {
        .description = COMPOUND_STRING("The Pokémon trades Abilities with a\nfoe that makes direct contact."),
    },
    
    [ABILITY_GORILLA_TACTICS] =
    {
        .description = COMPOUND_STRING("Boosts its Attack stat but only\nthe first move selected is usable."),
    },
    
    [ABILITY_NEUTRALIZING_GAS] =
    {
        .description = COMPOUND_STRING("While the Pokémon is in battle, all\nother Abilities are nullified."),
    },
    
    [ABILITY_PASTEL_VEIL] =
    {
        .description = COMPOUND_STRING("Prevents the Pokémon and its allies\nfrom being poisoned."),
    },
    
    [ABILITY_HUNGER_SWITCH] =
    {
        .description = COMPOUND_STRING("It oscillates between its Full\nBelly and Hangry Modes each turn."),
    },
    
    [ABILITY_QUICK_DRAW] =
    {
        .description = COMPOUND_STRING("Enables the Pokémon to\nmove first occasionally."),
    },
    
    [ABILITY_UNSEEN_FIST] =
    {
        .description = COMPOUND_STRING("The Pokémon's direct contact moves\nbypass the foe's protection effects."),
    },
    
    [ABILITY_CURIOUS_MEDICINE] =
    {
        .description = COMPOUND_STRING("When the Pokémon enters battle, all\nallies' stat changes are removed."),
    },
    
    [ABILITY_TRANSISTOR] =
    {
        .description = COMPOUND_STRING("Powers up Electric-type moves."),
    },
    
    [ABILITY_DRAGONS_MAW] =
    {
        .description = COMPOUND_STRING("Powers up Dragon-type moves."),
    },
    
    [ABILITY_CHILLING_NEIGH] =
    {
        .description = COMPOUND_STRING("Utters a chilling neigh when KOing\na foe, boosting its Attack stat."),
    },
    
    [ABILITY_GRIM_NEIGH] =
    {
        .description = COMPOUND_STRING("Utters a terrifying neigh when KOing\na foe, boosting its Sp. Atk stat."),
    },
    
    [ABILITY_AS_ONE_ICE_RIDER] =
    {
        .description = COMPOUND_STRING("KOing a foe boosts its Attack stat.\nThe foe cannot eat Berries."),
    },
    
    [ABILITY_AS_ONE_SHADOW_RIDER] =
    {
        .description = COMPOUND_STRING("KOing a foe boosts its Sp. Atk stat.\nThe foe cannot eat Berries."),
    },
    
    [ABILITY_LINGERING_AROMA] =
    {
        .description = COMPOUND_STRING("The attacker's Ability changes to\nLingering Aroma when making contact."),
    },
    
    [ABILITY_SEED_SOWER] =
    {
        .description = COMPOUND_STRING("When hit by an attack, turns the\nfield into Grassy Terrain."),
    },
    
    [ABILITY_THERMAL_EXCHANGE] =
    {
        .description = COMPOUND_STRING("Boosts its Attack when hit by Fire-\ntype moves. It cannot be burned."),
    },
    
    [ABILITY_ANGER_SHELL] =
    {
        .description = COMPOUND_STRING("At half HP or less, its defenses\nfall while offenses and Speed rise."),
    },
    
    [ABILITY_PURIFYING_SALT] =
    {
        .description = COMPOUND_STRING("It's immune to status conditions.\nGhost-type moves do half damage."),
    },
    
    [ABILITY_WELL_BAKED_BODY] =
    {
        .description = COMPOUND_STRING("Incoming Fire-type moves do no\ndamage and sharply boost Defense."),
    },
    
    [ABILITY_WIND_RIDER] =
    {
        .description = COMPOUND_STRING("Incoming wind moves do no damage\nand boost its Attack stat instead."),
    },
    
    [ABILITY_GUARD_DOG] =
    {
        .description = COMPOUND_STRING("If intimidated, its Attack stat is\nboosted. Cannot be forced to flee."),
    },
    
    [ABILITY_ROCKY_PAYLOAD] =
    {
        .description = COMPOUND_STRING("Powers up Rock-type moves."),
    },
    
    [ABILITY_WIND_POWER] =
    {
        .description = COMPOUND_STRING("Being hit by a wind move charges\nits next Electric-type move."),
    },
    
    [ABILITY_ZERO_TO_HERO] =
    {
        .description = COMPOUND_STRING("The Pokémon transforms into its\nHero Form when it switches out."),
    },
    
    [ABILITY_COMMANDER] =
    {
        .description = COMPOUND_STRING("When it enters battle, it goes into an\nally Dondozo's mouth to command it."),
    },
    
    [ABILITY_ELECTROMORPHOSIS] =
    {
        .description = COMPOUND_STRING("Taking damage charges it, boosting\nits next Electric-type move."),
    },
    
    [ABILITY_PROTOSYNTHESIS] =
    {
        .description = COMPOUND_STRING("Boosts the Pokémon's most proficient\nstat in sun or with a Booster Energy."),
    },
    
    [ABILITY_QUARK_DRIVE] =
    {
        .description = COMPOUND_STRING("Boosts its best stat on Electric\nTerrain or with a Booster Energy."),
    },
    
    [ABILITY_GOOD_AS_GOLD] =
    {
        .description = COMPOUND_STRING("A body of pure gold grants immunity\nto other Pokémon's status moves."),
    },
    
    [ABILITY_VESSEL_OF_RUIN] =
    {
        .description = COMPOUND_STRING("Lowers all other Pokémon's Sp. Atk\nstats with its ruinous vessel."),
    },
    
    [ABILITY_SWORD_OF_RUIN] =
    {
        .description = COMPOUND_STRING("Lowers all other Pokémon's Defense\nstats with its ruinous sword."),
    },
    
    [ABILITY_TABLETS_OF_RUIN] =
    {
        .description = COMPOUND_STRING("Lowers all other Pokémon's Attack\nstats with its ruinous tablets."),
    },
    
    [ABILITY_BEADS_OF_RUIN] =
    {
        .description = COMPOUND_STRING("Lowers all other Pokémon's Sp. Def\nstats with its ruinous beads."),
    },
    
    [ABILITY_ORICHALCUM_PULSE] =
    {
        .description = COMPOUND_STRING("Summons harsh sunlight when entering\nbattle. Sunlight raises its Attack."),
    },
    
    [ABILITY_HADRON_ENGINE] =
    {
        .description = COMPOUND_STRING("Sets Electric Terrain when entering\nbattle. That terrain ups its Sp. Atk."),
    },
    
    [ABILITY_OPPORTUNIST] =
    {
        .description = COMPOUND_STRING("If a foe's stat is boosted, it\nboosts the same stat for itself."),
    },
    
    [ABILITY_CUD_CHEW] =
    {
        .description = COMPOUND_STRING("An eaten Berry will be regurgitated\nand eaten once more on the next turn."),
    },
    
    [ABILITY_SHARPNESS] =
    {
        .description = COMPOUND_STRING("Powers up slicing moves."),
    },
    
    [ABILITY_SUPREME_OVERLORD] =
    {
        .description = COMPOUND_STRING("Each fainted party member slightly\nboosts the Pokémon's offensive stats."),
    },
    
    [ABILITY_COSTAR] =
    {
        .description = COMPOUND_STRING("When the Pokémon enters a battle,\nit copies an ally's stat changes."),
    },
    
    [ABILITY_TOXIC_DEBRIS] =
    {
        .description = COMPOUND_STRING("When hit by a physical move, lays\nToxic Spikes at the feet of the foe."),
    },
    
    [ABILITY_ARMOR_TAIL] =
    {
        .description = COMPOUND_STRING("The Pokémon's odd tail makes foes\nunable to hit with priority moves."),
    },
    
    [ABILITY_EARTH_EATER] =
    {
        .description = COMPOUND_STRING("Restores HP if hit by Ground-type\nmoves instead of taking damage."),
    },
    
    [ABILITY_MYCELIUM_MIGHT] =
    {
        .description = COMPOUND_STRING("The Pokémon's status moves are slow,\nbut can't be blocked by Abilities."),
    },
    
    [ABILITY_HOSPITALITY] =
    {
        .description = COMPOUND_STRING("Restores a quarter of an ally's HP\nwhen the Pokémon enters battle."),
    },
    
    [ABILITY_MINDS_EYE] =
    {
        .description = COMPOUND_STRING("Its accuracy can't drop. Ignores\nevasiveness and Ghost immunities."),
    },
    
    [ABILITY_EMBODY_ASPECT_TEAL_MASK] =
    {
        .description = COMPOUND_STRING("Its Teal Mask shines,\nboosting the Pokémon's Speed stat."),
    },
    
    [ABILITY_EMBODY_ASPECT_HEARTHFLAME_MASK] =
    {
        .description = COMPOUND_STRING("Its Hearthflame Mask shines,\nboosting the Pokémon's Attack stat."),
    },
    
    [ABILITY_EMBODY_ASPECT_WELLSPRING_MASK] =
    {
        .description = COMPOUND_STRING("Its Wellspring Mask shines,\nboosting the Pokémon's Sp. Def stat."),
    },
    
    [ABILITY_EMBODY_ASPECT_CORNERSTONE_MASK] =
    {
        .description = COMPOUND_STRING("Its Cornerstone Mask shines,\nboosting the Pokémon's Defense stat."),
    },
    
    [ABILITY_TOXIC_CHAIN] =
    {
        .description = COMPOUND_STRING("The Pokémon's toxic chain may badly\npoison foes hit by its moves."),
    },
    
    [ABILITY_SUPERSWEET_SYRUP] =
    {
        .description = COMPOUND_STRING("Once per battle, the Pokémon emits\na scent that lowers evasiveness."),
    },
    
    [ABILITY_TERA_SHIFT] =
    {
        .description = COMPOUND_STRING("When entering battle, it absorbs\nenergy to assume its Terastal Form."),
    },
    
    [ABILITY_TERA_SHELL] =
    {
        .description = COMPOUND_STRING("When at full HP, all damage-dealing\nmoves it's hit by are resisted."),
    },
    
    [ABILITY_TERAFORM_ZERO] =
    {
        .description = COMPOUND_STRING("When assuming its Stellar Form, it\neliminates weather and terrain."),
    },
    
    [ABILITY_POISON_PUPPETEER] =
    {
        .description = COMPOUND_STRING("Pokémon poisoned by the Pokémon's\nmoves will also become confused."),
    },
};