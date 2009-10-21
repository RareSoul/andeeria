-- Timbermaw Hold reputation fix
UPDATE `quest_template` SET `RewRepValue1` = '1400' WHERE `entry` IN (8460, 8461, 8470, 8471, 8464);
UPDATE `quest_template` SET `RewRepValue1` = '100' WHERE `entry` IN (8462, 8465);
UPDATE `quest_template` SET `RewRepValue1` = '1000' WHERE `entry` IN (6031, 6032);
UPDATE `quest_template` SET `RewRepValue1` = '300' WHERE `entry` IN (8466, 8469, 8467);
UPDATE `creature_onkill_reputation` SET `MaxStanding1` = '5', `RewOnKillRepValue1` = '20' WHERE `creature_id` IN (7157, 7156, 7154, 7155, 7158, 7153, 14372, 7440, 7442, 10916, 7439, 7441, 7438);
UPDATE `creature_onkill_reputation` SET `MaxStanding1` = '7', `RewOnKillRepValue1` = '40' WHERE `creature_id` IN (10738);
UPDATE `creature_onkill_reputation` SET `MaxStanding1` = '7', `RewOnKillRepValue1` = '60' WHERE `creature_id` IN (9462, 9464);
UPDATE `creature_onkill_reputation` SET `MaxStanding1` = '7', `RewOnKillRepValue1` = '100' WHERE `creature_id` IN (14342, 10199);
-- THX to mtk666