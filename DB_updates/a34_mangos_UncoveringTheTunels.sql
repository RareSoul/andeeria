UPDATE creature_template SET AIName = 'EventAI' WHERE entry IN (26470,26469,26468);
DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (26470,26469,26468);
INSERT INTO `creature_ai_scripts` VALUES
-- East Building
('2647001','26470', '10','0','100','0', '1','5','0','0', '33','26470','6','0', '0','0','0','0', '0','0','0','0','East Building - exploration credit'),
-- North Building
('2646801','26468', '10','0','100','0', '1','5','0','0', '33','26468','6','0', '0','0','0','0', '0','0','0','0','North Building - exploration credit'),
-- South Building
('2646901','26469', '10','0','100','0', '1','5','0','0', '33','26469','6','0', '0','0','0','0', '0','0','0','0','South Building - exploration credit');