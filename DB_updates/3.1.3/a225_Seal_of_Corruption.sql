-- Missing spell 'Seal of Corruption' fix
DELETE FROM `npc_trainer` WHERE `spell` = '53736';

INSERT into `npc_trainer` (`entry`, `spell`, `spellcost`, `reqlevel`) VALUES (16681, 53736, 100000, 66),
(20406, 53736, 100000, 66),
(16680, 53736, 100000, 66),
(23128, 53736, 100000, 66),
(16275, 53736, 100000, 66),
(16679, 53736, 100000, 66);
-- thx to MTK666