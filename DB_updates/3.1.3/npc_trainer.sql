-- Missing spell 'Seal of Corruption' fix
DELETE FROM `npc_trainer` WHERE `spell` = '53736';

INSERT into `npc_trainer` (`entry`, `spell`, `spellcost`, `reqlevel`) VALUES (16681, 53736, 100000, 66),
(20406, 53736, 100000, 66),
(16680, 53736, 100000, 66),
(23128, 53736, 100000, 66),
(16275, 53736, 100000, 66),
(16679, 53736, 100000, 66);

-- Missing spell 'Seal of Blood' fix
DELETE FROM `npc_trainer` WHERE `spell` = '31892';

INSERT into `npc_trainer` (`entry`, `spell`, `spellcost`, `reqlevel`) VALUES (16679, 31892, 63000, 64),
(16681, 31892, 63000, 64),
(16680, 31892, 63000, 64),
(16275, 31892, 63000, 64),
(20406, 31892, 63000, 64),
(23128, 31892, 63000, 64);

-- Summon Warhorse (horda)
DELETE FROM npc_trainer WHERE spell = 34768;
INSERT INTO npc_trainer VALUES
(20406,34768,10000,0,0,30),
(23128,34768,10000,0,0,30),
(16275,34768,10000,0,0,30),
(15280,34768,10000,0,0,30),
(16679,34768,10000,0,0,30),
(16680,34768,10000,0,0,30),
(16681,34768,10000,0,0,30);