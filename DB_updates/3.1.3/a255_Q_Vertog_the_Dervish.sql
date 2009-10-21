-- delete all summoning actions. this creatures should not summon anything except Vertog
DELETE FROM creature_ai_scripts WHERE creature_id IN (3275,3274,3273) AND (action1_type IN (12,32) OR action2_type IN (12,32) OR action3_type IN (12,32));
INSERT INTO `creature_ai_scripts` (`creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) VALUES
('3275','6','0','10','0','0','0','0','0','32','3395','0','3395','1','-99980','0','0','0','0','0','0','Kolkar Marauder - summon Verog the Dervish'),
('3274','6','0','10','0','0','0','0','0','32','3395','0','3395','1','-99980','0','0','0','0','0','0','Kolkar Pack Runner - summon Verog the Dervish'),
('3273','6','0','10','0','0','0','0','0','32','3395','0','3395','1','-99980','0','0','0','0','0','0','Kolkar Stormer - summon Verog the Dervish');

REPLACE INTO `creature_ai_texts` VALUES
('-99980','I am slain! Summon Verog!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'0','1','0','0','3395');
REPLACE INTO `creature_ai_summons` VALUES
('3395','-1210.4','-2722','106.7','4.7','180000','3395');

UPDATE creature_template SET AIName = 'EventAI' WHERE entry IN (3275,3274,3273);