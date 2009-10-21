DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 23216 ;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 23523 ;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 23318 ;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 23524 ;



INSERT INTO `creature_ai_scripts` (`id` ,`creature_id` ,`event_type` ,`event_inverse_phase_mask` ,`event_chance` ,`event_flags` ,`event_param1` ,`event_param2` ,`event_param3` ,`event_param4` ,`action1_type` ,`action1_param1` ,`action1_param2` ,`action1_param3` ,`action2_type` ,`action2_param1` ,`action2_param2` ,`action2_param3` ,`action3_type` ,`action3_param1` ,`action3_param2` ,`action3_param3` ,`comment`)
VALUES (
NULL , '23216', '0', '0', '70', '1', '1000', '5000', '10000', '20000', '11', '41975', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Defender - Heroic Strike'
), (
NULL , '23216', '0', '0', '50', '1', '5000', '15000', '5000', '25000', '11', '41180', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Defender -  Shield Bash '
), (
NULL , '23216', '0', '0', '50', '1', '5000', '10000', '10000', '15000', '11', '41178', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Defender - Debilitating Strike'
);

INSERT INTO `creature_ai_scripts` (`id` ,`creature_id` ,`event_type` ,`event_inverse_phase_mask` ,`event_chance` ,`event_flags` ,`event_param1` ,`event_param2` ,`event_param3` ,`event_param4` ,`action1_type` ,`action1_param1` ,`action1_param2` ,`action1_param3` ,`action2_type` ,`action2_param1` ,`action2_param2` ,`action2_param3` ,`action3_type` ,`action3_param1` ,`action3_param2` ,`action3_param3` ,`comment`)
VALUES (
NULL , '23523', '0', '0', '100', '1', '1000', '5000', '1000', '5000', '11', '42024', '4', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Elementalist - Lightning Bolt'
), (
NULL , '23523', '0', '0', '50', '1', '10000', '25000', '10000', '40000', '11', '42023', '4', '4', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Elementalist - Rain of Fire'
);

INSERT INTO `creature_ai_scripts` (`id` ,`creature_id` ,`event_type` ,`event_inverse_phase_mask` ,`event_chance` ,`event_flags` ,`event_param1` ,`event_param2` ,`event_param3` ,`event_param4` ,`action1_type` ,`action1_param1` ,`action1_param2` ,`action1_param3` ,`action2_type` ,`action2_param1` ,`action2_param2` ,`action2_param3` ,`action3_type` ,`action3_param1` ,`action3_param2` ,`action3_param3` ,`comment`)
VALUES (
NULL , '23318', '0', '0', '100', '1', '1000', '5000', '5000', '10000', '11', '41978', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Rogue - Debilitating Poison'
), (
NULL , '23318', '0', '0', '70', '1', '1000', '5000', '5000', '10000', '11', '41177', '1', '4', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Rogue - Eviscerate'
);

INSERT INTO `creature_ai_scripts` (`id` ,`creature_id` ,`event_type` ,`event_inverse_phase_mask` ,`event_chance` ,`event_flags` ,`event_param1` ,`event_param2` ,`event_param3` ,`event_param4` ,`action1_type` ,`action1_param1` ,`action1_param2` ,`action1_param3` ,`action2_type` ,`action2_param1` ,`action2_param2` ,`action2_param3` ,`action3_type` ,`action3_param1` ,`action3_param2` ,`action3_param3` ,`comment`)
VALUES (
NULL , '23524', '0', '0', '50', '1', '5000', '5000', '10000', '10000', '11', '42025', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Spiritbinder - Spirit Mend'
), (
NULL , '23524', '0', '0', '100', '1', '5000', '5000', '5000', '10000', '11', '42027', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Spiritbinder - Chain Heal'
), (
NULL , '23524', '0', '0', '100', '1', '10000', '15000', '15000', '50000', '11', '42318', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Ashtongue Spiritbinder - Heal'
);

UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` =23216 LIMIT 1 ;
UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` =23523 LIMIT 1 ;
UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` =23318 LIMIT 1 ;
UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` =23524 LIMIT 1 ;

UPDATE `mangos`.`creature_template` SET `minhealth` = '1517500',
`maxhealth` = '1517500' WHERE `creature_template`.`entry` =22952 LIMIT 1 ;