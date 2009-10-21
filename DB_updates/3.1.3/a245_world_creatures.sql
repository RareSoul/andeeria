################### TEMPLATES #################
-- 'Ember Worg' lvl, faction & hp fix
UPDATE `creature_template` SET `minlevel` = '51', `maxlevel` = '52', `minhealth` = '2980',  `maxhealth` = '3082', `faction_A` = '38', `faction_H` = '38' WHERE `entry`= '9690';
-- Claw (UBH) immune to sleep
UPDATE creature_template SET mechanic_immune_mask = mechanic_immune_mask | 512 WHERE entry = 20165;



###################   ACIDs   #################

ToDo:
Add Outland Rars form my achive